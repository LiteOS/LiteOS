/*
 * Copyright (C) 2016-2018, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "client_mgmt.h"
#include "server_util.h"
#include "server_pkts.h"
#include "server_plug.h"
#include "server_core.h"

/* Definition for the maximum length of the sub
   topic name */
#ifndef CFG_SR_MAX_SUBTOP_LEN
#define MAX_SUBTOP_LEN 64
#else
#define MAX_SUBTOP_LEN CFG_SR_MAX_SUBTOP_LEN
#endif

/* Definition for the maximum topic nodes of the
   topic node tree.
   Each will message, subscription message
   and publish message from each client creates
   nodes that decrease the available topic nodes */
#ifndef CFG_SR_MAX_TOPIC_NODE
#define MAX_TOP_NODES  128
#else
#define MAX_TOP_NODES  CFG_SR_MAX_TOPIC_NODE
#endif

/* Definition for the maximum stack size for the
   node stack array, this stack is used in LIFO
   state in order to enter to the lowest down
   link hierarchy and delete the nodes from the
   lowest and up */
#ifndef CFG_SR_MAX_STACK_NODE
#define MAX_STACK_NODES  32
#else
#define MAX_STACK_NODES  CFG_SR_MAX_STACK_NODE
#endif

#define TNODE_PROP_RETAIN_DATA  0x04
#define WBUF_LEN   MQP_SERVER_RX_LEN /* Assignment to ease implementation */
#define NODE_DATA_RESET_PARAMS  NULL, 0, 0, false

#define CONN_FLAGS_WQID_GET(connFlags) ((connFlags >> 3) & QID_VMASK) /* WILL QOS VAL */

#define MK_QOS_ENUM(qid) ((MQTT_QOS)(qid & QID_VMASK))

/* A topic name (or string or tree or hierarchy) is handled as a series of nodes.
 A 'topic node' refers to a level in the topic tree or hierarchy and it stores
 the associated part of the topic string at that level. For example, a topic
 having a name "Home/Living_room/Window" will have three nodes for each of the
 subtopics, viz, "Home/", "Living_room/" and "Window". Further, the association
 between the nodes is indicated  as following.

                         ---------
                         |       |
                         |"Home/"|
                         |       |
                         ---------
                          |     ^
                          |     |
      down link hierarchy |     | up link hierarchy
                          |     |
                          V     |
                      ----------------
                      |              |
                      |"Living_room/"|
                      |              |
                      ----------------
                          |     ^
                          |     |
      down link hierarchy |     | up link hierarchy
                          |     |
                          V     |
                         ----------
                         |        |
                         |"Window"|
                         |        |
                         ----------


 To extend this example, another topic having a name "Home/Kitchen/Door" will be managed
 as following.
                    ---------
                    |       |
                    |"Home/"|
                    |       |
                    ---------
                     |     ^
                     |     |
 down link hierarchy |     | up link hierarchy
                     |     |
                     V     |
                 ----------------                                    ------------
                 |              | <------ up link neighbor --------- |          |
                 |"Living_room/"|                                    |"Kitchen/"|
                 |              | ------- down link neighbor ------> |          |
                 ----------------                                    ------------
                     |     ^                                            |    ^
                     |     |                                            |    |
 down link hierarchy |     | up link hierarchy      down link hierarchy |    | up link hierarchy
                     |     |                                            |    |
                     V     |                                            V    |
                    ----------                                         --------
                    |        |                                         |      |
                    |"Window"|  <-------- !!!Not neighbors!!! -------> |"Door"|
                    |        |                                         |      |
                    ----------                                         --------

 To reduce wasted in byte alignments, the structure has been hand packed.
 */
typedef struct _MQTTServerCore_TopicNode_t_
{
    struct _MQTTServerCore_TopicNode_t_ *dnNhbr; /* Down Link Neighbor node */
    struct _MQTTServerCore_TopicNode_t_ *upNhbr; /* Up   Link Neighbor node */

    struct _MQTTServerCore_TopicNode_t_ *dnHier; /* Down Link Hierarchy node */
    struct _MQTTServerCore_TopicNode_t_ *upHier; /* Up   Link Hierarchy node */

    uint32_t clMap[3]; /* Subscribers for each QOS */

    uint8_t *myData;   /* Leaf node: retained data */
    uint32_t myDlen;

    uint8_t  willCl;   /* counts the connected clients which has a WILL for the Leaf */

    uint8_t  myProp;   /* Bits 0,1 for QOS and rest are flags */

    uint8_t  pgMap;    /* Map: application plugins */

    uint16_t toplen;   /* Length of node sub-topic */
    char    *subtop;   /* NULL terminated sub-topic */

}MQTTServerCore_TopicNode_t;

/*
 Housekeeping to manage subtopics (nodes) at run-time.
 */
typedef struct _MQTTServerCore_NodeStack_t_
{
    MQTTServerCore_TopicNode_t *node;
    uint32_t val1;
    uint32_t val2;

}MQTTServerCore_NodeStack_t;

typedef struct _MQTTServerCore_TopbufDesc_t_
{
    char *buffer;
    uint16_t maxlen;
    uint16_t offset;

}MQTTServerCore_TopbufDesc_t;

/*
 Data structure which holds will data message.
 */
typedef struct _MQTTServerCore_WillParams_t_
{
    MQTT_UTF8String_t willMsg;   /**< Will message */
    uint8_t           willQoS;   /**< Will QoS     */
    bool              retain;    /**< Retain Flag  */
    MQTTServerCore_TopicNode_t *leafWithClientWill; /**< leaf corresponding to will Topic */

}MQTTServerCore_WillParams_t;

/* Stack Index */
static int32_t MQTTServerCore_stackIdx = 0;

/* Counter for how many nodes are being used.
   Each will message, subscription message
   and publish message from each client creates
   nodes that increase the NodesInUse counter */
static int32_t MQTTServerCore_NodesInUse = 0;

/* Global variable that holds the root of the
   node tree */
static MQTTServerCore_TopicNode_t *MQTTServerCore_rootNode = NULL;

static char MQTTServerCore_workBuf[WBUF_LEN];

static MQTTServerCore_NodeStack_t node_stack[MAX_STACK_NODES];

static void try_node_delete(MQTTServerCore_TopicNode_t *node);

//*****************************************************************************
// MQTT Server Core Internal Routines
//*****************************************************************************

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void stack_add(MQTTServerCore_TopicNode_t *node, uint32_t val1, uint32_t val2)
{
    node_stack[MQTTServerCore_stackIdx].node = node;
    node_stack[MQTTServerCore_stackIdx].val1 = val1;
    node_stack[MQTTServerCore_stackIdx].val2 = val2;

    MQTTServerCore_stackIdx++;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline MQTTServerCore_NodeStack_t *stack_pop(void)
{
    if (0 == MQTTServerCore_stackIdx)
    {
        return NULL;
    }
    return (node_stack + (--MQTTServerCore_stackIdx));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool is_node_retain(const MQTTServerCore_TopicNode_t *node)
{
    return ((node->myProp & TNODE_PROP_RETAIN_DATA) ? true : false);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline void node_retain_set(MQTTServerCore_TopicNode_t *node, bool retain)
{
    if (retain)
    {
        node->myProp |= TNODE_PROP_RETAIN_DATA;
    }
    else
    {
        node->myProp &= ~TNODE_PROP_RETAIN_DATA;
    }
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline void node_qid_set(MQTTServerCore_TopicNode_t *node, uint8_t qid)
{
    node->myProp &= ~QID_VMASK;
    node->myProp |= qid;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline uint8_t node_qid_get(MQTTServerCore_TopicNode_t *node)
{
    return (node->myProp & QID_VMASK);
}

//*****************************************************************************
//
//! \brief Utility function which check if the node has at least one connected
//         client which has a WILL for the Leaf
//
//*****************************************************************************
static inline bool is_node_willed(const MQTTServerCore_TopicNode_t *node)
{
    return ((0 != node->willCl) ? true : false);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool enrolls_plugin(const MQTTServerCore_TopicNode_t *node)
{
    return ((PG_MAP_ALL_DFLTS != node->pgMap) ? true : false);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void node_reset(MQTTServerCore_TopicNode_t *node)
{
    node->dnNhbr = NULL;
    node->upNhbr = NULL;
    node->dnHier = NULL;
    node->upHier = NULL;

    node->clMap[0] = node->clMap[1] = node->clMap[2] = 0;
    node->myData = NULL;
    node->myDlen = 0;
    node->willCl = 0;
    node->myProp = QID_VMASK;

    node->pgMap = PG_MAP_ALL_DFLTS;
    node->toplen = 0;
}

//*****************************************************************************
//
//! \brief Allocate memory for new topic node (subtopic)
//
//*****************************************************************************
static MQTTServerCore_TopicNode_t *alloc_topic_node(void)
{

    MQTTServerCore_TopicNode_t *node = NULL;

    /* Check for free space for node */
    if ( MQTTServerCore_NodesInUse < MAX_TOP_NODES )
    {
        /* Allocate memory in order to create new nodes */
        node = (MQTTServerCore_TopicNode_t *)malloc(sizeof(MQTTServerCore_TopicNode_t));

        /* Check if memory allocation was successful */
        if (NULL != node)
        {
            /* Reset the node to the default values */
            node_reset(node);

            /* Increase the counter of how many nodes are in use */
            MQTTServerCore_NodesInUse++;
        }
    }
    /* else - Warning - Maximum space for topic nodes reached */

    return node;
}

//*****************************************************************************
//
//! \brief free the memory of topic node (subtopic)
//
//*****************************************************************************
static void free_node(MQTTServerCore_TopicNode_t *node)
{
    /* Free the allocated memory for retain message*/
    if (NULL != node->myData)
    {
        free(node->myData);
    }

    /* Free the allocated subtop memory */
    free(node->subtop);

    /* Free the allocated node memory */
    free(node);

    /* Decrease the counter of how many nodes are in use */
    MQTTServerCore_NodesInUse--;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool is_end_of_subtop(const char *topstr, char const **next_subtop)
{
    bool rv = false;

    if ('\0' == *topstr)
    {
        *next_subtop = NULL; /* Reached end of topstr */
        rv = true;
    }

    if ('/' == *topstr)
    {
        /* Next sub-topic is NULL, if a '\0' follows '/' */
        *next_subtop = *(topstr + 1) ? topstr + 1 : NULL;
        rv = true;
    }

    return rv;
}

//*****************************************************************************
//
//! \brief The function receive a complete topic, separate it to sub topics and
//! copy the sub topics into the topic nodes(subtop_buf)
//
//*****************************************************************************
static int32_t subtop_read(const char *topstr, char **subtop_buf, uint16_t buf_length, char const **next_subtop)
{
    int32_t idx = 0;
    int32_t len = buf_length;
    char    topic_buffer[MAX_SUBTOP_LEN] = { 0 };

    /* Continue if subtop_buf and topstr inputs are not NULL */
    if ((NULL == subtop_buf) || (NULL == topstr))
    {
        /* Memory allocation failed */
        return -1;
    }

    *next_subtop = topstr;

    /* Copy the characters from topstr to topic_buffer until null terminartion
       or '/' character which represent sub topic */
    while (idx < (len - 1))
    {

        topic_buffer[idx++] = *topstr;

        if (is_end_of_subtop(topstr, next_subtop))
        {
            break;
        }
        topstr++;
    }

    if ( (idx == len) || (0 == idx) )
    {
        /* Fatal, insufficient buffer for sub-str */
        return -1; /* zero or insufficient buffer provided */
    }

    /* NULL terminated buffer: unless '\0' ended copy, make one */
    if ('\0' == topic_buffer[idx - 1])
    {
        idx--; /* A fix up */
    }
    else
    {
        topic_buffer[idx] = '\0';
    }

    /* Allocate memory for the sub topic buffer */
    *subtop_buf = (char *)malloc(sizeof(char) * idx + 1);

    if (NULL == *subtop_buf)
    {
        /* Memory allocation failed */
        return -1;
    }
    /* Copy the string from the static topic_buffer to dynamic memory
       subtop_buf */
    strncpy(*subtop_buf, topic_buffer, idx + 1);

    return idx;
}

//*****************************************************************************
//
//! \brief Return node pointer when topic node allocation was successful, NULL
//! when allocation fails
//
//*****************************************************************************
static MQTTServerCore_TopicNode_t *alloc_node_subtop(const char *topstr, char const **next_subtop)
{
    uint16_t len = 0;
    MQTTServerCore_TopicNode_t *node = alloc_topic_node();

    if (NULL == node)
    {
        return NULL;
    }
    len = subtop_read(topstr, &(node->subtop), MAX_SUBTOP_LEN, next_subtop);
    if (len <= 0)
    {
        free_node(node);
        return NULL;
    }

    node->toplen = len;

    return node;
}

//*****************************************************************************
//
//! \brief Returns true if string 'subtop' is part of string 'topstr' otherwise false.
//! Additionally, on success, pointer to next subtopic in 'topstr' is provided.
//
//*****************************************************************************
bool is_first_subtop(const char *subtop, const char *topstr, char const **next_subtop)
{
    while (*subtop == *topstr)
    {
        if (is_end_of_subtop(topstr, next_subtop))
        {
            return true;
        }
        subtop++;
        topstr++;
    }

    return false;
}

//*****************************************************************************
//
//! \brief Find a topic node in neighbor list that matches first subtopic in 'topstr'.
//! Additionally, on success, pointer to next subtopic in 'topstr' is provided.
//
//*****************************************************************************
MQTTServerCore_TopicNode_t *subtop_nhbr_node_find(const MQTTServerCore_TopicNode_t *root_nh,
                                                const char *topstr, char const **next_subtop)
{
    /* This routine does not make use of 'next_subtop' */
    while (root_nh)
    {
        if (true == is_first_subtop(root_nh->subtop, topstr, next_subtop))
        {
            break;
        }
        root_nh = root_nh->dnNhbr;
    }

    return (MQTTServerCore_TopicNode_t *)root_nh;/* Bad: const from pointer removed */
}

//*****************************************************************************
//
//! \brief Find a topic node in neighbor list that matches the given 'subtop' string
//
//*****************************************************************************
MQTTServerCore_TopicNode_t *nhbr_node_find(const MQTTServerCore_TopicNode_t *root_nh, const char *subtop)
{
    const char *next_subtop = NULL;

    return subtop_nhbr_node_find(root_nh, subtop, &next_subtop);
}

//*****************************************************************************
//
//! \brief Find leaf node of branch-combo that matches complete 'topstr'.
//! Modus Operandi:  For each sub topic in 'topstr', go across neighbor list,
//! then for matching neighbor node, make its child node as root of neighbor
//! list for another iteration for next sub topic.
//
//*****************************************************************************
static MQTTServerCore_TopicNode_t *lowest_node_find(const char *topstr, bool *flag_nh, char const **next_subtop)
{
    MQTTServerCore_TopicNode_t *root_nh = MQTTServerCore_rootNode, *node = NULL;

    *next_subtop = topstr;
    *flag_nh = false;

    while (root_nh)
    {
        node = subtop_nhbr_node_find(root_nh, topstr, next_subtop);
        if (NULL == node)
        { /* Partial or no match */
            *flag_nh = true;
            node = root_nh;
            break;
        }

        /* NULL '*next_subtop' indicates a complete match */
        if (NULL == (*next_subtop))
        {
            break;
        }
        root_nh = node->dnHier;
        topstr = *next_subtop;
    }

    return node;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
MQTTServerCore_TopicNode_t *leaf_node_find(const char *topstr)
{
    const char *next_subtop;
    bool flag_nh;
    MQTTServerCore_TopicNode_t *node = lowest_node_find(topstr, &flag_nh, &next_subtop);

    return ((NULL == next_subtop) ? node : NULL);
}

//*****************************************************************************
//
//! \brief Create 'child only' hierarchy of nodes to hold all sub topics in 'topstr'.
//! The routine returns a start node of hierarchy and also provides leaf node.
//
//*****************************************************************************
static MQTTServerCore_TopicNode_t *hier_nodes_create(const char *topstr, MQTTServerCore_TopicNode_t **leaf)
{
    MQTTServerCore_TopicNode_t *base = NULL;
    MQTTServerCore_TopicNode_t *node = NULL;
    MQTTServerCore_TopicNode_t *prev = NULL;
    const char *next_subtop = NULL;

    do
    {
        node = alloc_node_subtop(topstr, &next_subtop);

        if (NULL == node)
        {
            /* Allocation of a node failed, free up
             the previous allocations, if any, in
             the hierarchy that was being created */
            if (prev)
            {
                try_node_delete(prev);
            }
            base = NULL;
            break;
        }

        if (NULL == prev)
        {
            base = node; /* First node of hierarchy */
        }
        else
        {
            prev->dnHier = node;
            node->upHier = prev;
        }

        prev = node;
        topstr = next_subtop;

    } while (next_subtop);

    *leaf = node;

    return base;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void install_nhbr_node(MQTTServerCore_TopicNode_t *base, MQTTServerCore_TopicNode_t *node)
{
    while (base->dnNhbr)
    {
        base = base->dnNhbr;
    }

    base->dnNhbr = node;
    node->upNhbr = base;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void set_up_hier_nodes(MQTTServerCore_TopicNode_t *upHier, MQTTServerCore_TopicNode_t *dnHier)
{
    upHier->dnHier = dnHier;
    dnHier->upHier = upHier;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline void install_topic_root_node(MQTTServerCore_TopicNode_t *node)
{
    MQTTServerCore_rootNode = node;
}

//*****************************************************************************
//
//! \brief  Create or update tree to create branch-combo to refer to 'topstr'
//
//*****************************************************************************
MQTTServerCore_TopicNode_t *topic_node_create(const char *topstr)
{
    MQTTServerCore_TopicNode_t *base = NULL;
    MQTTServerCore_TopicNode_t *node = NULL;
    MQTTServerCore_TopicNode_t *leaf = NULL;
    const char *next_subtop = topstr;
    bool flag_nh;

    base = lowest_node_find(topstr, &flag_nh, &next_subtop);
    if (NULL == next_subtop)
    {
        return base; /* Found exact match */
    }
    /* Control reaches here, if either no or partial branch-combo has been
     found for 'topstr'. Now, let's create remaining branches for string
     'next_subtop' and assign them to appropriately to topic tree.
     */
    node = hier_nodes_create(next_subtop, &leaf);
    if (node)
    {
        if (NULL == base)
        {
            install_topic_root_node(node);
        }
        else if (flag_nh)
        {
            install_nhbr_node(base, node);
        }
        else
        {
            set_up_hier_nodes(base, node);
        }
        return leaf;
    }

    return NULL;
}

//*****************************************************************************
//
//! \brief Return true when the node doesn't has any links that can destroy the
//! node tree if the node will be deleted and false when it has links
//
//*****************************************************************************
static bool can_delete_node(const MQTTServerCore_TopicNode_t *node)
{
    if (node->dnHier)
    {
        /* fatal, node w/ dn-hier */
        return false;
    }
    return true;
}

//*****************************************************************************
//
//! \brief Delete and free the allocated memory of the node and re-arrange the
//! node tree if needed. The return value is the next node to work on.
//! The return value is NULL when the node cannot be deleted or when the node
//! has up link neighbor, that means that the topic could be deleted only until
//! this point.
//
//*****************************************************************************
static MQTTServerCore_TopicNode_t *node_delete(MQTTServerCore_TopicNode_t *node)
{
    MQTTServerCore_TopicNode_t *next = NULL;

    if (false == can_delete_node(node))
    {
        return NULL;
    }
    if (node->upNhbr)
    {
        if ( node->dnNhbr )
        {
            /* Deletion of a node that has up and down
               link neighbor will disconnect the node
               tree, bypass the node that will be deleted */
            (node->upNhbr)->dnNhbr = node->dnNhbr;
            (node->dnNhbr)->upNhbr = node->upNhbr;
        }
        else
        {
            /* the node has only up link neighbor that will
               be return as the next node, so the node can
               be deleted */
            (node->upNhbr)->dnNhbr = NULL;
        }
        /* Since the next node should be up link neighbor
           which contain different topic, no more node
           cannot be deleted */
        next = NULL;
    }
    else if (node->upHier)
    {
        /* Deletion of a node that has up link hierarchy
           and down link neighbor will disconnect the node
           tree, bypass the node that will be deleted */
        if ( node->dnNhbr )
        {
            (node->upHier)->dnHier = node->dnNhbr;
            (node->dnNhbr)->upNhbr = NULL;
            (node->dnNhbr)->upHier = node->upHier;
        }
        else
        {
            /* the node has only up link hierarchy that
               will be return as the next node, so the
               node can be deleted */
            (node->upHier)->dnHier = NULL;
        }
        /* When not having down link neighbor or hierarchy
           the next node will be the up link hierarchy */
        next = node->upHier;
    }
    else if ( node->dnNhbr )
    {
        /* Deletion of a node that has only down link
           neighbor will disconnect the down link neighbor
           from the base node (root node), that means that
           the new root node will be the down link neighbor */
        (node->dnNhbr)->upHier = NULL;
        MQTTServerCore_rootNode = node->dnNhbr;
    }
    else
    {
        /* A node that doesn't has any links means that it
           is the root node, when deleting this node, the
           global variable for the root node needs to be
           initialized as well */
        MQTTServerCore_rootNode = NULL;
    }

    /* free the memory of topic node (subtopic) */
    free_node(node);

    return next;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool topbuf_add(MQTTServerCore_TopbufDesc_t *buf_desc, const MQTTServerCore_TopicNode_t *node)
{
    const char *next_subtop;
    char *buf = buf_desc->buffer + buf_desc->offset;
    char *allocated_buf = NULL;
    uint16_t len = buf_desc->maxlen - buf_desc->offset;
    int32_t rv = subtop_read(node->subtop, &allocated_buf, len, &next_subtop);

    if (rv < 0)
    {
        return false;
    }
    if (NULL != next_subtop)
    {
        free(allocated_buf);
        /* topstr_add fatal, bad subtop */
        return false;
    }

    /* Coping the allocated memory and free the buffer */
    strncpy(buf, allocated_buf, rv + 1);
    free(allocated_buf);
    buf_desc->offset += rv;

    return true;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool topbuf_cpy(MQTTServerCore_TopbufDesc_t *buf_desc, const char *subtop)
{
    const char *next_subtop;
    char *buf = buf_desc->buffer + buf_desc->offset;
    char *allocated_buf = NULL;
    uint16_t len = buf_desc->maxlen - buf_desc->offset;
    int32_t rv = subtop_read(subtop, &allocated_buf, len, &next_subtop);

    if (rv < 0)
    {
        return false;
    }
    if (NULL != next_subtop)
    {
        free(allocated_buf);
        /* topstr_copy fatal, bad subtop */
        return false;
    }
    /* Coping the allocated memory and free the buffer */
    strncpy(buf, allocated_buf, rv + 1);
    free(allocated_buf);
    buf_desc->offset += rv;

    return true;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool has_a_wildcard(const MQTTServerCore_TopicNode_t *node)
{
    const char *str = node->subtop;

    while ('\0' != *str)
    {
        if (('+' == *str) || ('#' == *str))
        {
            return true;
        }
        str++;
    }

    return false;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool is_node_SUB_subtop(const MQTTServerCore_TopicNode_t *node, const char *subtop)
{
    if (false == has_a_wildcard(node))
    {
        return (((0 == strcmp(node->subtop, subtop)) || (node->dnHier && (0 == strcmp("+/", subtop))) ||
                (!node->dnHier && (0 == strcmp("+", subtop)))) ?
        true : false);
    }
    return false;
}

//*****************************************************************************
//
//! \brief Search node tree, created by PUB retention, for the branch combo, whose
//! absolute sub-topic sequence 'matches', in entirety, the topic, which is
//! being subscribed. The 'match' criteria is met either through the
//! wildcard sub-topics or exact compare.
//! The hierarchical search starts at the specified 'base' node and ends at
//! the leaf node. If the sequence of 'base-to-leaf' sub-topics 'match' the
//! 'topSUB', then the leaf node is returned, otherwise a NULL is returned.
//! As part of hierarchical search, neighboring nodes, if any, are logged
//! for subsequent iteration of the routine
//
//*****************************************************************************
static MQTTServerCore_TopicNode_t *pub_hier_search(const char *topSUB, const MQTTServerCore_TopicNode_t *base,
                                                MQTTServerCore_TopbufDesc_t *mk_pubtop)
{
    const MQTTServerCore_TopicNode_t *node = base;
    const MQTTServerCore_TopicNode_t *prev = NULL;
    const char *next_subtop = topSUB;
    char  *subtop = NULL;

    while (next_subtop && node)
    {
        if (subtop_read(topSUB, &subtop, MAX_SUBTOP_LEN, &next_subtop) < 0)
        {
            break;
        }
        if (node->dnNhbr)
        {
            stack_add(node->dnNhbr, (uint32_t) topSUB, mk_pubtop->offset);
        }
        if (false == is_node_SUB_subtop(node, subtop))
        {
            free(subtop);
            break; /* Node doesn't form part of published topic */
        }
        if (false == topbuf_add(mk_pubtop, node))
        {
            free(subtop);
            break;
        }
        prev = node;
        node = node->dnHier;

        topSUB = next_subtop;
        free(subtop);
    }

    if (NULL != next_subtop)
    {
        node = NULL;
    }
    else
    {
        node = prev;
    }

    return (MQTTServerCore_TopicNode_t *)node;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool is_node_PUB_subtop(const MQTTServerCore_TopicNode_t *node, const char *subtop, bool endtop)
{
    /* Assumes that subtop hasn't got any wildcard character */
    return (((0 == strcmp(subtop, node->subtop)) || (!endtop && (0 == strcmp("+/", node->subtop))) ||
            (endtop && (0 == strcmp("+", node->subtop)))) ?
    true : false);
}

//*****************************************************************************
//
//! \brief Search node tree, created by subscriptions, for the branch combo, whose
//! sub-topic sequence 'matches', in entirety, the absolute topic, to which
//! data has been published. The 'match' criteria is met either through the
//! wildcard sub-topics or exact compare.
//! The hierarchical search starts at the specified 'base' node and ends at
//! the leaf node. If the sequence of 'base-to-leaf' sub-topics 'match' the
//! 'topPUB', then the leaf node is returned, otherwise a NULL is returned.
//! As part of hierarchical search, neighboring nodes, if any, are logged
//! for subsequent iteration of the routine
//
//*****************************************************************************
static MQTTServerCore_TopicNode_t *SUB_leaf_search(const char *topPUB, const MQTTServerCore_TopicNode_t *base)
{
    const MQTTServerCore_TopicNode_t *node = base;
    const MQTTServerCore_TopicNode_t *prev = NULL;
    const char *next_subtop = topPUB;
    char *subtop = NULL;

    while (next_subtop && node)
    {
        if (subtop_read(topPUB, &subtop, MAX_SUBTOP_LEN, &next_subtop) < 0)
        {
            break;
        }
        if (node->dnNhbr)
        {
            stack_add(node->dnNhbr, (uint32_t) topPUB, 0);
        }
        if (0 == strcmp("#", node->subtop))
        {
            free(subtop);
            return (MQTTServerCore_TopicNode_t *)node;
        }

        if (false == is_node_PUB_subtop(node, subtop, !next_subtop))
        {
            free(subtop);
            break; /* Node doesn't form part of published topic */
        }
        prev = node;
        node = node->dnHier;

        topPUB = next_subtop;
        free(subtop);
    }

    if (NULL != next_subtop)
    {
        node = NULL;
    }
    else
    {
        node = prev;
    }
    return (MQTTServerCore_TopicNode_t *)node; // Bad
}

/*-------------------------------------------------------------------------
 * MQTT Routines
 *-------------------------------------------------------------------------
 */

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void try_node_delete(MQTTServerCore_TopicNode_t *node)
{
    while (node)
    {
        if (is_node_retain(node) || is_node_willed(node) || enrolls_plugin(node) ||
            node->clMap[0] || node->clMap[1] || node->clMap[2])
        {
            break;
        }
        node = node_delete(node);
    }
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void pub_msg_send(const MQTT_UTF8String_t *topic, const uint8_t *dataBuf,
                        uint32_t dataLen, uint8_t fh_flags, uint32_t clMap)
{
    MQTT_QOS qos = MQTT_FH_BYTE1_QOS(fh_flags);
    MQTT_Packet_t *mqp = NULL;

    mqp = MQTTServerUtil_mqpAlloc(MQTT_PUBLISH, 2 + topic->length + 2 + dataLen);
    if (NULL == mqp)
    {
        return;
    }
    if ((0 > MQTT_packetPubAppendTopic(mqp, topic, qos ? MQTTServerUtil_setMsgID() : 0)) ||
        (dataLen && (0 > MQTT_packetPubAppendData(mqp, dataBuf, dataLen))))
    {
        MQTT_packetFree(mqp);
        return;
    }

    MQTT_packetPrepFh(mqp, fh_flags);

    if (clMap)
    {
        MQTTClientMgmt_pubDispatch(clMap, mqp);
    }
    else
    {
        MQTT_packetFree(mqp);
    }
    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static MQTTServerCore_TopicNode_t *SUB_node_create(const char *topSUB, uint8_t qid, void *usrCl)
{
    MQTTServerCore_TopicNode_t *leaf = topic_node_create(topSUB);
    uint8_t j;
    uint32_t map;

    if (leaf)
    {
        j = 0;
        map = MQTTClientMgmt_bmapGet(usrCl);

        for (j = 0; j < 3; j++)
        {
            /* Client: clear QOS of existing sub, if any */
            leaf->clMap[j] &= ~map;
        }
        leaf->clMap[qid] |= map;

        MQTTClientMgmt_subCountAdd(usrCl);
    }

    return leaf;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint8_t proc_pub_leaf(MQTTServerCore_TopicNode_t *leaf, const MQTT_UTF8String_t *topic, MQTT_QOS qos, void *usrCl)
{
    uint8_t qid = QOS_VALUE(qos);

    if (is_node_retain(leaf))
    {
        /* If it is an earlier created topic w/ retained
         data, then pick lower of the two QOS(s)  */
        qid = MQTT_MIN(node_qid_get(leaf), qid);

        /* Publish the retained data to this client */
        pub_msg_send(topic, leaf->myData, leaf->myDlen, MQTT_MAKE_FH_FLAGS(false, qid, true), MQTTClientMgmt_bmapGet(usrCl));
    }

    return qid;
}

//*****************************************************************************
//
//! \brief  Multi-level wild-card subscription - search of all of the tree i.e.
//! "no topic" ('no_top') in particular and publish it to client, if
//! the hierarchy has no wild-card node
//
//*****************************************************************************
static uint8_t proc_pub_hier_no_top(MQTTServerCore_TopicNode_t *base, MQTTServerCore_TopbufDesc_t *mk_pubtop, MQTT_QOS qos, void *usrCl)
{
    MQTTServerCore_TopicNode_t *node = base;
    MQTTServerCore_TopicNode_t *leaf = NULL;
    uint8_t ack = QOS_VALUE(qos);
    MQTT_UTF8String_t topic;

    /* 1. Find the leaf node of a non wildcard branch-combo */
    while (node)
    {
        if (node->dnNhbr)
        {
            stack_add(node->dnNhbr, 0, mk_pubtop->offset);
        }
        if (has_a_wildcard(node))
        {
            break;
        }
        if (false == topbuf_add(mk_pubtop, node))
        {
            break;
        }
        leaf = node;
        node = node->dnHier;
    }

    /* A non NULL value of 'node' would indicate a hierarchy with a
     wildcard (sub-)topic (the 'node') - not suitable for PUB. */

    if (NULL == node)
    {
        /* 2. Send retained data, if any, to SUB Client */
        topic.buffer = mk_pubtop->buffer;
        topic.length = mk_pubtop->offset;

        /* In this version, at this juncture, the 'leaf'
         will not be NULL. Nevertheless a check (for
         the sake of static analytical tools).........*/
        if (leaf)
        {
            ack = proc_pub_leaf(leaf, &topic, qos, usrCl);
        }
    }

    return ack;
}

//*****************************************************************************
//
//! \brief Multi-level wild-card subscription - search of all of the tree i.e.
//! "no topic" ('no_top') in particular and publish it to client, if
//! a hierarchy in the tree has no wild-card node
//
//*****************************************************************************
static uint8_t proc_pub_tree_no_top(MQTTServerCore_TopicNode_t *base, MQTTServerCore_TopbufDesc_t *mk_pubtop, MQTT_QOS qos, void *usrCl)
{
    uint32_t stack_ref = MQTTServerCore_stackIdx;
    uint8_t min = QOS_VALUE(qos);
    MQTTServerCore_NodeStack_t *stack;
    uint8_t ack;

    if (base != NULL)
    {
        stack_add(base, 0, mk_pubtop->offset);
    }
    while (stack_ref < MQTTServerCore_stackIdx)
    {
        stack = stack_pop();

        mk_pubtop->offset = (uint16_t) stack->val2;

        ack = proc_pub_hier_no_top(stack->node, mk_pubtop, qos, usrCl);
        if (ack < min)
        {
            min = ack;
        }
    }

    return min;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint8_t proc_pub_hier_SUBtop(const char *topSUB, const MQTTServerCore_TopicNode_t *base, MQTTServerCore_TopbufDesc_t *mk_pubtop, MQTT_QOS qos, void *usrCl)
{
    MQTTServerCore_TopicNode_t *leaf = pub_hier_search(topSUB, base, mk_pubtop);
    uint8_t min = QOS_VALUE(qos);
    MQTT_UTF8String_t topic;

    if (leaf)
    {
        topic.buffer = mk_pubtop->buffer;
        topic.length = mk_pubtop->offset;

        min = proc_pub_leaf(leaf, &topic, qos, usrCl);
    }

    return min;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint8_t proc_pub_tree_SUBtop(const char *topSUB, MQTTServerCore_TopicNode_t *base, MQTTServerCore_TopbufDesc_t *mk_pubtop, MQTT_QOS qos, void *usrCl)
{
    uint32_t stack_ref = MQTTServerCore_stackIdx;
    uint8_t min = QOS_VALUE(qos);
    MQTTServerCore_NodeStack_t *stack;
    uint8_t ack;

    if (NULL != base)
    {
        stack_add(base, (uint32_t) topSUB, mk_pubtop->offset);
    }
    while (stack_ref < MQTTServerCore_stackIdx)
    {
        stack = stack_pop();
        mk_pubtop->offset = stack->val2;
        ack = proc_pub_hier_SUBtop((char *)stack->val1, stack->node, mk_pubtop, qos, usrCl);

        if (ack < min)
        {
            min = ack;
        }
    }

    return min;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint8_t proc_sub_ml_wc_hier(const char *grandpa_topSUB, char *parent_subtop, MQTTServerCore_TopicNode_t *base,
                                MQTTServerCore_TopbufDesc_t *mk_pubtop, MQTT_QOS qos, void *usrCl)
{
    uint8_t min = QOS_VALUE(qos), ack = QFL_VALUE;
    char *subtop = NULL;
    MQTTServerCore_TopicNode_t *leaf;
    uint16_t offset;
    uint16_t sublen;

    /* 1. Search hier node for 'grandpa' and if found, get to parent level */
    if ('\0' != grandpa_topSUB[0])
    {
        leaf = pub_hier_search(grandpa_topSUB, base, mk_pubtop);
        if (NULL == leaf)
        {
            return min;
        }
        base = leaf->dnHier; /* nhbr root at parent level */
    }

    /* 2. If present, process parent as a leaf and get its down hierarchy */
    subtop = parent_subtop;
    if (('\0' != subtop[0]) && ('+' != subtop[0]) && ('/' != subtop[0]))
    {
        offset = mk_pubtop->offset; /* Refer to grandpa's pubtop */
        sublen = 0;

        while (subtop[sublen])
        {
            sublen++;
        }

        ack = proc_pub_tree_SUBtop(subtop, base, mk_pubtop, qos, usrCl);
        mk_pubtop->offset = offset; /* Restores grandpa's pubtop */

        subtop[sublen] = '/'; /* Make parent's hier subtop */

        base = nhbr_node_find(base, subtop);
        if (base)
        {
            base = topbuf_cpy(mk_pubtop, subtop) ? base->dnHier : NULL;
        }
        subtop[sublen] = '\0'; /* Get back, original subtop */
    }

    min = MQTT_MIN(min, ack);
    /* 3. Process '#' WC by walking through entire sub-tree of parent 'base' */
    if (NULL != base)
    {
        ack = proc_pub_tree_no_top(base, mk_pubtop, qos, usrCl);
    }

    return MQTT_MIN(min, ack);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint8_t proc_sub_ml_wc_tree(char *grandpa_topSUB, char *parent_subtop, MQTTServerCore_TopicNode_t *base, MQTT_QOS qos, void *usrCl)
{
    MQTTServerCore_TopbufDesc_t mk_pubtop = { MQTTServerCore_workBuf, WBUF_LEN, 0 /* offset */};
    uint32_t stack_ref = MQTTServerCore_stackIdx;
    uint8_t min = QOS_VALUE(qos);
    MQTTServerCore_NodeStack_t *stack;
    uint8_t ack;

    if (NULL != base)
    {
        stack_add(base, (uint32_t) grandpa_topSUB, mk_pubtop.offset);
    }
    while (stack_ref < MQTTServerCore_stackIdx)
    {
        stack = stack_pop();

        mk_pubtop.offset = stack->val2;
        ack = proc_sub_ml_wc_hier((char *)stack->val1, parent_subtop, stack->node, &mk_pubtop, qos, usrCl);
        if (ack < min)
        {
            min = ack;
        }
    }

    return min;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint8_t ml_wc_nodes_create(char *parent_topSUB, uint16_t toplen, uint8_t qid, void *usrCl)
{
    MQTTServerCore_TopicNode_t *parent_leaf = NULL;

    if ('\0' != parent_topSUB[0])
    {
        parent_leaf = SUB_node_create(parent_topSUB, qid, usrCl);
        if (NULL == parent_leaf)
        {
            return QFL_VALUE;
        }
    }

    /* Get the topic SUB to it's original state */
    if (toplen > 1)
    {
        parent_topSUB[toplen - 2] = '/';
    }
    parent_topSUB[toplen - 1] = '#';

    if (NULL == SUB_node_create(parent_topSUB, qid, usrCl))
    {
        /* Failed to create WC topic, so delete parent as well.
         In this revision, 'parent_leaf' will not be a 'NULL'
         at this juncture, nevertheless a check (for tools) */
        if (parent_leaf)
        {
            node_delete(parent_leaf);
        }
        return QFL_VALUE;
    }

    return qid;
}

//*****************************************************************************
//
//! \brief Process Multi-level Wildcard Topic SUBSCRIBE
//
//*****************************************************************************
static uint8_t proc_sub_ml_wildcard(char *topSUB, uint16_t toplen, MQTT_QOS qos, void *usrCl)
{
    uint16_t len = 0;
    uint16_t limit = MQTT_MIN(toplen, MAX_SUBTOP_LEN);
    char subtop[MAX_SUBTOP_LEN];
    char *ptr = NULL;
    uint8_t min = QOS_VALUE(qos);

    /* 'topSUB': Need to create grandpa topic and parent-subtopic */
    topSUB[toplen - 1] = '\0'; /* Remove '#' */
    if (toplen > 1) /* Remove '/' */
    {
        topSUB[toplen - 2] = '\0';
    }
    do
    { /* Do processing to get parent sub-topic into buffer */
        if ('/' == topSUB[toplen - len - 1])
        {
            break; /* found '/' */
        }
        len++; /* Copy parent characters */
        subtop[MAX_SUBTOP_LEN - len] = topSUB[toplen - len];
    } while (len < limit);

    if ((toplen > len) && ('/' != topSUB[toplen - len - 1]))
    {
        return QFL_VALUE; /* Bad Length */
    }
    topSUB[toplen - len] = '\0'; /* End of grand-pa's topic name */
    ptr = subtop + MAX_SUBTOP_LEN - len; /* Parent's leaf subtop */

    /* Continue if leaf exists */
    if ( NULL == ptr )
    {
        return QFL_VALUE; /* Bad Length */
    }
    min = proc_sub_ml_wc_tree(topSUB, ptr, MQTTServerCore_rootNode, qos, usrCl);

    /* Make branch-combo to complete processing of parent' topic */
    strcpy(topSUB + toplen - len, ptr); // topSUB[toplen - len] = *ptr;

    /* Create nodes for multi-level wildcard topic & it's parent */
    min = ml_wc_nodes_create(topSUB, toplen, min, usrCl);

    return min;
}

//*****************************************************************************
//
//! \brief Process Single-level Wildcard or No Wild Card Topic SUBSCRIBE
//
//*****************************************************************************
static uint8_t proc_sub_sl_or_no_wc(const char *topSUB, MQTT_QOS qos, void *usrCl)
{
    MQTTServerCore_TopbufDesc_t mk_pubtop = { MQTTServerCore_workBuf, WBUF_LEN, 0 /* offset */};
    uint8_t min = QOS_VALUE(qos);

    /* For single level wildcard or absolute topic, find PUB nodes */
    min = proc_pub_tree_SUBtop(topSUB, MQTTServerCore_rootNode, &mk_pubtop, qos, usrCl);

    if (NULL == SUB_node_create(topSUB, min, usrCl))
    {
        min = QFL_VALUE;
    }
    return min;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint16_t proc_forward_slash(char *buf, uint16_t len)
{
    uint16_t i;
    uint16_t j;
    char curr;

    for (i = 1, j = 1; i < len; i++)
    {
        curr = buf[i];
        if (('/' == curr) && (buf[i - 1] == curr))
        {
            continue; /* Drop consecutive '/' */
        }
        buf[j++] = curr;
    }

    if ((1 != j) && ('/' == buf[j - 1]))
    {
        j--; /* Topic can not end with a '/' */
    }
    buf[j] = '\0';

    return j;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool is_valid_char_order(char prev, char curr)
{
    return (((('/' != prev) && ('+' == curr)) || (('+' == prev) && ('/' != curr)) ||
            (('/' != prev) && ('#' == curr)) || (('#' == prev))) ? false : true);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool is_valid_SUB_top(const char *buf, uint16_t len)
{
    char prev;
    char curr;
    uint16_t i = 0;

    if ((0 == len) || ('\0' == *buf))
    {
        return false;
    }
    curr = buf[0];
    for (i = 1; (i < len) && ('\0' != curr); i++)
    {
        prev = curr;
        curr = buf[i];

        if (false == is_valid_char_order(prev, curr))
        {
            break;
        }
    }

    return ((i == len) ? true : false);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool proc_sub_msg_rx(void *usrCl, const MQTT_UTF8StrQOS_t *qosTopics,
                            uint32_t nTopics, uint16_t msgID, uint8_t *ack)
{
    int32_t i = 0;
    const MQTT_UTF8StrQOS_t *qos_top;
    MQTT_QOS qos;
    char *buf;
    uint16_t len;

    for (i = 0; i < nTopics; i++)
    {
        qos_top = qosTopics + i;
        qos = qos_top->qosreq;
        buf = (char *)qos_top->buffer;
        len = qos_top->length;

        /* Remove zero-topics and trailing '/' from SUB top */
        len = proc_forward_slash(buf, len);
        ack[i] = QFL_VALUE;
        if (false == is_valid_SUB_top(buf, len))
        {
            continue;
        }
        buf[len] = '\0'; /* Dirty trick, cheeky one */

        ack[i] = ('#' == buf[len - 1]) ? proc_sub_ml_wildcard(buf, len, qos, usrCl) :
                                        proc_sub_sl_or_no_wc(buf, qos, usrCl);

    }

    return true; /* Send SUB-ACK and do not close network */
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool proc_sub_msg_rx_locked(void *usrCl, const MQTT_UTF8StrQOS_t *qosTopics,
                                uint32_t nTopics, uint16_t msgID, uint8_t *ack)
{
    return (proc_sub_msg_rx(usrCl, qosTopics, nTopics, msgID, ack));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void leaf_un_sub(MQTTServerCore_TopicNode_t *leaf, void *usrCl)
{
    uint8_t j = 0;
    uint32_t map = MQTTClientMgmt_bmapGet(usrCl);

    for (j = 0; j < 3; j++)
    {
        /* Client: clear QOS of existing sub, if any */
        if (0 == (leaf->clMap[j] & map))
        {
            continue;
        }
        leaf->clMap[j] &= ~map;
        MQTTClientMgmt_subCountDel(usrCl);

        try_node_delete(leaf);

        break;
    }
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool proc_un_sub_msg(void *usrCl, const MQTT_UTF8String_t *topics, uint32_t nTopics, uint16_t msgID)
{
    uint32_t i = 0;
    const MQTT_UTF8String_t *topic;
    MQTTServerCore_TopicNode_t *leaf;
    uint16_t len;

    for (i = 0; i < nTopics; i++)
    {
        topic = topics + i;
        leaf = NULL;
        len = topic->length;

        /* The maximum length of 'MQTTServerCore_workBuf' is same as that of RX buffer
         in the PKT-LIB. Therefore, the WBUF_LEN is not being checked
         against the length of the topic (a constituent of RX buffer).
         */
        strncpy(MQTTServerCore_workBuf, topic->buffer, topic->length);
        MQTTServerCore_workBuf[len] = '\0';

        if ('#' == MQTTServerCore_workBuf[len - 1])
        { /* Multi-level Wildcard */
            MQTTServerCore_workBuf[len - 1] = '\0';
            if (len > 1)
            {
                MQTTServerCore_workBuf[len - 2] = '\0';
            }
            leaf = leaf_node_find(MQTTServerCore_workBuf);
            if (leaf)
            {
                leaf_un_sub(leaf, usrCl);
            }
            if (len > 1)
            {
                MQTTServerCore_workBuf[len - 2] = '/';
            }
            MQTTServerCore_workBuf[len - 1] = '#';
        }

        leaf = leaf_node_find(MQTTServerCore_workBuf);
        if (leaf)
        {
            leaf_un_sub(leaf, usrCl);
        }
    }

    return true; /* Do not close network */
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool proc_un_sub_msg_locked(void *usrCl, const MQTT_UTF8String_t *topics, uint32_t nTopics, uint16_t msgID)
{
    return (proc_un_sub_msg(usrCl, topics, nTopics, msgID));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void leaf_msg_send(  const MQTTServerCore_TopicNode_t *leaf, const MQTT_UTF8String_t *topic, const uint8_t *dataBuf,
                            uint32_t dataLen, bool dup, MQTT_QOS qos, bool retain)
{
    uint8_t  qid = 0;
    uint8_t  fh_fgs = 0;
    uint32_t map;

    for (qid = 0; qid < 3; qid++)
    {
        map = leaf->clMap[qid];
        fh_fgs = MQTT_MAKE_FH_FLAGS(dup, MQTT_MIN(qid, QOS_VALUE(qos)), retain);

        if (map)
        {
            pub_msg_send(topic, dataBuf, dataLen, fh_fgs, map);
        }
    }

    if (enrolls_plugin(leaf))
    {
        MQTTServerPlug_publish(leaf->pgMap, topic, dataBuf, dataLen, dup, qos, retain);
    }
    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void node_data_set(MQTTServerCore_TopicNode_t *node, uint8_t *data, uint32_t dlen, uint8_t qid, bool retain)
{
    node->myData = data;
    node->myDlen = dlen;

    node_qid_set(node, qid);
    node_retain_set(node, retain);

    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool node_data_update(MQTTServerCore_TopicNode_t *node, bool drop_qid0, const uint8_t *dataBuf, uint32_t dataLen, uint8_t qid, bool retain)
{

    /* Assumes that caller has provided either reset or valid params */

    uint8_t *data = NULL;

    if (!(drop_qid0 && (0 == qid)) && dataBuf)
    {
        data = malloc(dataLen);
        if (!data)
        {
            return false;
        }
    }

    if (node->myDlen)
    {
        free(node->myData);
        node->myData = NULL;
        node->myDlen = 0;
    }

    if (data)
    {
        MQTT_bufWrNbytes(data, dataBuf, dataLen);
        node_data_set(node, data, dataLen, qid, retain);
    }
    else
    {
        node_data_set(node, NODE_DATA_RESET_PARAMS);
    }

    return true;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static inline bool is_wildcard_char(char c)
{
    return ((('+' == c) || ('#' == c)) ? true : false);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static int32_t pub_topic_read(const MQTT_UTF8String_t *topic, char *buf, uint32_t len)
{
    uint32_t i = 0;
    uint32_t toplen = topic->length;
    char c;

    if (len < (toplen + 1))
    {
        return -1;
    }
    for (i = 0; i < toplen; i++)
    {
        c = topic->buffer[i];
        if (is_wildcard_char(c))
        {
            return -1; /* Invalid: wildcard in PUB topic */
        }
        if ('\0' == c)
        {
            return -1; /* Invalid: NUL char in PUB topic */
        }
        buf[i] = c;
    }

    buf[i] = '\0';

    return i;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void proc_sub_tree_topPUB(   const char *topPUB, const MQTT_UTF8String_t *topic, const uint8_t *dataBuf,
                                    uint32_t dataLen, MQTT_QOS qos, bool retain)
{
    MQTTServerCore_TopicNode_t *leaf = NULL;
    uint32_t stack_ref = MQTTServerCore_stackIdx;
    MQTTServerCore_NodeStack_t *stack;

    if (NULL != MQTTServerCore_rootNode)
    {
        stack_add(MQTTServerCore_rootNode, (uint32_t) topPUB, 0 /* Not used */);
    }
    while (stack_ref < MQTTServerCore_stackIdx)
    {
        stack = stack_pop();

        /* Find leaf node of SUB that matches the PUB topic */
        leaf = SUB_leaf_search((char *)stack->val1, stack->node);
        if (leaf)
        {
            leaf_msg_send(leaf, topic, dataBuf, dataLen, false, qos, retain);
        }
    }
}

//*****************************************************************************
//
//! \brief Forwarding the message to all subscribers of the topic and stores
//! the message if the retain parameter is set to true
//
//*****************************************************************************
static bool _proc_pub_msg_rx(void *usrCl, const MQTT_UTF8String_t *topic, const uint8_t *dataBuf,
                            uint32_t dataLen, uint8_t msgID, MQTT_QOS qos, bool retain)
{
    int32_t err = -1;

    /* Prior to msg processing, check for topic or buffer errors */
    if ((pub_topic_read(topic, MQTTServerCore_workBuf, WBUF_LEN) > 0) &&
        (proc_forward_slash(MQTTServerCore_workBuf, topic->length) > 0))
    {
        /* If a valid MSG ID is specified for a QOS2 pkt, track it */
        err = -2;
        if ((msgID == 0) || (MQTT_QOS2 != qos) || (true == MQTTClientMgmt_qos2PubRxUpdate(usrCl, msgID)))
        {
            /* Forward data to all subscribers of PUB topic in server */
            proc_sub_tree_topPUB(MQTTServerCore_workBuf, topic, dataBuf, dataLen, qos, false);

            err = 0;
            if (retain)
            {
                MQTTServerCore_TopicNode_t *leaf = topic_node_create(MQTTServerCore_workBuf);
                if ((NULL == leaf) ||
                    (false == node_data_update(leaf, false /*true*/, dataBuf, dataLen, QOS_VALUE(qos), retain)))
                {
                    err = -3; /* Resources no more available */
                }
                if (leaf)
                {
                    try_node_delete(leaf);
                }
            }
        }
    }
    return ((err < 0) ? false : true);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool proc_pub_msg_rx_locked( void *usrCl, const MQTT_UTF8String_t *topic, const uint8_t *dataBuf, uint32_t dataLen,
                                    uint16_t msgID, bool dup, MQTT_QOS qos, bool retain)
{
    return (_proc_pub_msg_rx(usrCl, topic, dataBuf, dataLen, msgID, qos, retain));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static int32_t utf8_str_rd(const MQTT_UTF8String_t *utf8, char *buf, uint32_t len)
{
    if ((NULL == utf8) || (utf8->length > (len - 1)))
    {
        return -1;
    }
    MQTT_bufWrNbytes((uint8_t *)buf, (uint8_t *)utf8->buffer, utf8->length);
    buf[utf8->length] = '\0';

    return utf8->length;
}

//*****************************************************************************
//
//! \brief Utility function which uses dynamic memory allocation for the input
//! parameter buf, and stores the new allocated memory and the buffer length in
//! the input parameter utf8
//
//*****************************************************************************
static bool utf8_str_set(MQTT_UTF8String_t *utf8, const char *buf, const uint32_t len)
{
    char * allocatedStringMemory = NULL;

    /* Check that all the input parameters are valid */
    if ((NULL == utf8) || (NULL == buf) || (0 == len))
    {
        return false;
    }

    /* Allocate memory that will store the buf data */
    allocatedStringMemory = malloc(len);

    /* Check if the memory allocated successfully */
    if (NULL == allocatedStringMemory)
    {
        return false;
    }

    /* Copy the data from buf to the allocated memory */
    strncpy(allocatedStringMemory, buf, len);
    /* Copy the allocated memory address to the utf8 buffer parameter
       and copy the len to the utf8 len parameter */
    utf8->buffer = allocatedStringMemory;
    utf8->length = len;

    return true;
}

//*****************************************************************************
//
//! \brief Extract the will params from the connection package and store them
//
//*****************************************************************************

static bool will_message_construct(MQTTServerCore_WillParams_t *willParam, MQTTServerCore_TopicNode_t *leaf, MQTT_UTF8String_t *willMsgData, uint8_t QoS, bool messageRetain)
{
    bool ret_val = true;

    /* Extract and store the will message data and length */
    ret_val = utf8_str_set(&(willParam->willMsg), MQ_CONN_UTF8_BUF(willMsgData), (MQ_CONN_UTF8_LEN(willMsgData)));
    /* Check if extraction was success */
    if (ret_val == false)
    {
        return ret_val;
    }

    /* Extract and store the QoS parameter */
    willParam->willQoS = QoS;
    /* Extract and store the retain parameter */
    willParam->retain = messageRetain;
    /* Store corresponding leaf */
    willParam->leafWithClientWill = leaf;

    return ret_val;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint16_t proc_connect_rx(void *ctxCl, uint8_t connFlags, MQTT_UTF8String_t * const *utf8Vec, void **usrCl)
{
    MQTTServerCore_TopicNode_t *leaf = NULL;
    MQTT_UTF8String_t *utf8 = NULL;
    MQTTServerCore_WillParams_t * willParams = NULL;
    void *appCl = NULL;
    uint16_t utf8_len = 0;
    uint16_t rv = MQTTServerPlug_connect(MQC_UTF8_CLIENTID(utf8Vec), MQC_UTF8_USERNAME(utf8Vec), MQC_UTF8_PASSWORD(utf8Vec), &appCl);

    if (rv == 0)
    {
        rv = MQTT_CONNACK_RC_SVR_UNAVBL; /* Server (resource) unavailable */

        /* De-construct the will topic part of the utf8vec */
        utf8 = MQC_UTF8_WILL_TOP(utf8Vec);
        /* Check the will topic exists */
        if (utf8 && utf8->length)
        {
            utf8_str_rd(utf8, MQTTServerCore_workBuf, WBUF_LEN);

            leaf = topic_node_create(MQTTServerCore_workBuf);
            if (NULL == leaf)
            {
                MQTTServerPlug_disconn(appCl, true);
                return rv;
            }

            /* create Will Params parameter */
            willParams = malloc(sizeof(MQTTServerCore_WillParams_t));

            /* Check if the will allocation was successful */
            if (NULL == willParams)
            {
                /* Will allocation failed, free memory */
                try_node_delete(leaf);
                MQTTServerPlug_disconn(appCl, true);
                return rv;
            }
            /* Extract the will params and store them */
            if (false == will_message_construct(willParams, leaf, MQC_UTF8_WILL_MSG(utf8Vec),
                         CONN_FLAGS_WQID_GET(connFlags), connFlags & MQTT_CONN_MSG_FLAG_WILL_RETAIN))
            {
                /* Will construction failed, free memory */
                free(willParams);
                try_node_delete(leaf);
                MQTTServerPlug_disconn(appCl, true);
                return rv;
            }
        }

        utf8 = MQC_UTF8_CLIENTID(utf8Vec);
        if (utf8)
        {
            utf8_len = utf8_str_rd(utf8, MQTTServerCore_workBuf, WBUF_LEN);
        }
        /* Create usr parameter for the client and update his data */
        rv = MQTTClientMgmt_connectRx(ctxCl, (connFlags & MQTT_CONN_MSG_FLAG_CLEAN_START) ? true : false,
                        utf8_len ? MQTTServerCore_workBuf : NULL, appCl, willParams, usrCl);

        if (MQTT_CONNACK_RC_REQ_ACCEPT == (rv & 0xFF))
        {
            if (leaf)
            {
                /* Increase the counter which tells how many connected clients has a WILL in this leaf  */
                (leaf->willCl)++;
            }
            return rv; /* Connection successful */
        }

        if (leaf)
        {
            node_data_update(leaf, true, NODE_DATA_RESET_PARAMS);
        }
        try_node_delete(leaf);
        MQTTServerPlug_disconn(appCl, true);
    }
    return rv;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static uint16_t proc_connect_rx_locked(void *ctxCl, uint8_t connFlags, MQTT_UTF8String_t * const *utf8Vec, void **usrCl)
{
    return (proc_connect_rx(ctxCl, connFlags, utf8Vec, usrCl));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static void session_hier_delete(MQTTServerCore_TopicNode_t *node, void *usrCl)
{
    MQTTServerCore_TopicNode_t *prev = NULL;
    uint32_t clMap = MQTTClientMgmt_bmapGet(usrCl);
    int32_t i;

    while (node)
    {
        i = 0;
        for (i = 0; i < 3; i++)
        {
            if (node->clMap[i] & clMap)
            {
                node->clMap[i] &= ~clMap;
                MQTTClientMgmt_subCountDel(usrCl);
                /* Client/Topic/QID 1-to-1 map */
                break;
            }
        }

        if (node->dnNhbr)
        {
            stack_add(node->dnNhbr, 0, 0);
        }
        prev = node;
        node = node->dnHier;
    }

    if (prev)
    {
        try_node_delete(prev);
    }
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
void session_tree_delete(void *usrCl)
{
    uint32_t stack_ref = MQTTServerCore_stackIdx;
    MQTTServerCore_NodeStack_t *stack;

    if (NULL != MQTTServerCore_rootNode)
    {
        stack_add(MQTTServerCore_rootNode, 0, 0);
    }
    while (stack_ref < MQTTServerCore_stackIdx)
    {
        stack = stack_pop();
        session_hier_delete(stack->node, usrCl);
    }
}

//*****************************************************************************
//
//! \brief Utility function which send the will and store it if the retain
//! parameter is set to true
//
//*****************************************************************************
static void proc_client_will(MQTTServerCore_TopicNode_t *leaf, MQTTServerCore_WillParams_t *willParams)
{
    uint32_t wbuf_len = WBUF_LEN - 1; /* Make space for '\0' in wbuf */
    uint32_t offset = wbuf_len;
    MQTT_UTF8String_t topic;
    MQTTServerCore_TopicNode_t *node;

    MQTTServerCore_workBuf[offset] = '\0'; /* Ensures wbuf is NULL terminated */
    node = leaf;

    /* Prepare a topic string by walking back from leaf to root */
    do
    {
        if (offset < node->toplen)
        {
            return;
        }
        offset -= node->toplen;
        strncpy(MQTTServerCore_workBuf + offset, node->subtop, node->toplen);

        while (node->upNhbr)
        {
            node = node->upNhbr;
        }
        node = node->upHier;

    } while (node);

    topic.buffer = MQTTServerCore_workBuf + offset;
    topic.length = wbuf_len - offset;

    /* Forward data to all subscribers of PUB topic in server */
    proc_sub_tree_topPUB((const char *)topic.buffer, &topic, (const uint8_t *) willParams->willMsg.buffer,
                         willParams->willMsg.length, MK_QOS_ENUM(willParams->willQoS), willParams->retain);

    /* If retain parameter of the will message is set to
       true, store the retained message */
    if (willParams->retain)
    {
         node_data_update(leaf, false, (const uint8_t *)willParams->willMsg.buffer,
                          willParams->willMsg.length, willParams->willQoS, willParams->retain);
    }

}

//*****************************************************************************
//
//! \brief Check if the client has a will and send it, and then close and clear
//! the usr parameter (of client context)
//
//*****************************************************************************
static void onClNetClose(void *usrCl, bool due2err)
{
    MQTTServerCore_TopicNode_t *leaf = NULL;
    MQTTServerCore_WillParams_t *willParams = NULL;
    void *appCl = NULL;

    /* See if client has a WILL that it intends to broadcast */
    willParams = (MQTTServerCore_WillParams_t *)MQTTClientMgmt_willHndlGet(usrCl);
    if (NULL != willParams)
    {
        /* Copy the leaf location for ease of use */
        leaf = willParams->leafWithClientWill;

        /* Check if no wills are stored and a client
           intend to broadcast one */
        if (0 == leaf->willCl)
        {
            return; /* Mismatch: should never happen */
        }
        /* When the disconnection occurs due to an error
           send the will message */
        if (due2err)
        {
            proc_client_will(leaf, willParams); /* broadcast the will*/
        }
        /* Decrease the counter which tells how many connected clients has a WILL in this leaf  */
        (leaf->willCl)--;
        /* Network is closing */
        try_node_delete(leaf);
    }

    /* If not needed for future, delete session info */
    if (MQTTClientMgmt_canSessionDelete(usrCl))
    {
        session_tree_delete(usrCl);
    }
    /* Inform app that client has been disconnected  */
    appCl = MQTTClientMgmt_appHndlGet(usrCl);
    MQTTServerPlug_disconn(appCl, due2err);

    /* Free user data unless the user is connected
       with clean session disabled */
    MQTTClientMgmt_onNetClose(usrCl);
}

//*****************************************************************************
//
//! \brief Notify that network connection to client has been closed.
//! This routine is invoked by the LIB to declare to the application that
//! the network connection to a particular client has been terminated and
//! follow-up, if needed, can now commence. If configured, removal of the
//! client session and / or dispatch of the WILL message, will be typical
//! aspects, among others, to follow-up. The routine aids the application
//! by indicating if an error condition had caused the closure.
//!
//! This routine is invoked by the LIB irrespective of the source entity,
//! server or client, that has caused the closure of the network.
//!
//! @param[in] usrCl handle to connection context in the application
//! @param[in] due2err has the connection been closed due to an error
//
//*****************************************************************************
static void on_cl_net_close_locked(void *usrCl, bool due2err)
{
    onClNetClose(usrCl, due2err);
    return;
}

//*****************************************************************************
//
//! \brief Notify that CONNACK has been sent to the specified client.
//! This routine is invoked by the LIB to enable the application to make
//! progress and follow-up on the session information for the particular
//! client. Specifically, this routine facilitates the application to
//! either delete the session or re-send / sync-up the pending messages
//! associated with the client.
//
//*****************************************************************************
static void onConnackSend(void *usrCl, bool cleanSession)
{
    /* If asserted, then need to start w/ clean state */
    if (cleanSession)
    {
        session_tree_delete(usrCl);
    }
    MQTTClientMgmt_onConnackSend(usrCl, cleanSession);

    return;
}

//*****************************************************************************
//
//! \brief Notify that CONNACK has been sent to the specified client.
//! This routine is invoked by the LIB to enable the application to make
//! progress and follow-up on the session information for the particular
//! client. Specifically, this routine facilitates the application to
//! either delete the session or re-send / sync-up the pending messages
//! associated with the client.
//
//*****************************************************************************
static void on_connack_send_locked(void *usrCl, bool cleanSession)
{
    onConnackSend(usrCl, cleanSession);
    return;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static bool proc_notify_ack_locked(void *usrCl, uint8_t msgType, uint16_t msgID)
{
    return (MQTTClientMgmt_notifyAck(usrCl, msgType, msgID));
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static int32_t proc_topic_enroll(uint8_t pgID, const MQTT_UTF8String_t *topic, MQTT_QOS qos)
{
    MQTTServerCore_TopicNode_t *leaf = NULL;
    uint16_t len = 0;

    if ((NULL == topic) || (NULL == topic->buffer) || (0 == topic->length))
    {
        return -1;
    }
    if (WBUF_LEN < (topic->length + 1))
    {
        return -2;
    }
    len = topic->length;
    strncpy(MQTTServerCore_workBuf, topic->buffer, len);
    MQTTServerCore_workBuf[len] = '\0';

    leaf = topic_node_create(MQTTServerCore_workBuf);
    if (NULL == leaf)
    {
        return -3;
    }
    PG_MAP_VAL_SETUP(leaf->pgMap, QOS_VALUE(qos), pgID);

    return 0;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static int32_t proc_topic_enroll_locked(uint8_t pgID, const MQTT_UTF8String_t *topic, MQTT_QOS qos)
{
    int32_t rv = 0;

    MUTEX_LOCKIN();
    rv = proc_topic_enroll(pgID, topic, qos);
    MUTEX_UNLOCK();

    return rv;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static int32_t proc_topic_cancel(uint8_t pgID, const MQTT_UTF8String_t *topic)
{
    MQTTServerCore_TopicNode_t *leaf = NULL;
    uint16_t len = 0;

    if (NULL == topic)
    {
        return -1;
    }
    if (WBUF_LEN < (topic->length + 1))
    {
        return -2;
    }
    len = topic->length;
    strncpy(MQTTServerCore_workBuf, topic->buffer, len);
    MQTTServerCore_workBuf[len] = '\0';

    leaf = leaf_node_find(MQTTServerCore_workBuf);
    if (NULL == leaf)
    {
        return -2;
    }
    PG_MAP_VAL_RESET(leaf->pgMap, pgID);

    try_node_delete(leaf);

    return 0;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static int32_t proc_topic_cancel_locked(uint8_t pgID, const MQTT_UTF8String_t *topic)
{
    int32_t rv = 0;

    MUTEX_LOCKIN();
    rv = proc_topic_cancel(pgID, topic);
    MUTEX_UNLOCK();

    return rv;
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
static int32_t proc_app_pub_send_locked(const MQTT_UTF8String_t *topic, const uint8_t *dataBuf, uint32_t dataLen, MQTT_QOS qos, bool retain)
{
    bool rv;

    MUTEX_LOCKIN();
    /* Received from application, process topic for distribution */
    rv = _proc_pub_msg_rx(NULL, topic, dataBuf, dataLen, 0x00, qos, retain);
    MUTEX_UNLOCK();

    return (rv ? (int32_t) dataLen : -1);
}

//*****************************************************************************
//
//! \brief
//
//*****************************************************************************
int32_t MQTTServerCore_init(const MQTTServerPkts_LibCfg_t *libCfg, const MQTTServerCore_AppCfg_t *appCfg)
{
    /* If mutex is specified, then the following set of callbacks
     are invoked in the locked state - enumerated by 'locked' */
    MQTTServerPkts_MsgCBs_t pkts_cbs = {
            proc_connect_rx_locked, proc_sub_msg_rx_locked,
            proc_un_sub_msg_locked, proc_pub_msg_rx_locked,
            proc_notify_ack_locked, on_cl_net_close_locked,
            on_connack_send_locked };

    MQTTServerPlug_CBs_t core_cbs = {
            proc_topic_enroll_locked,
            proc_topic_cancel_locked,
            proc_app_pub_send_locked };

    MQTTServerCore_stackIdx = 0;
    MQTTServerCore_NodesInUse = 0;
    MQTTServerCore_rootNode = NULL;

    MQTTServerUtil_setParams(*(libCfg->mutex), libCfg->mutexLockin, libCfg->mutexUnlock);

    MQTTClientMgmt_init();

    MQTTServerPlug_init(&core_cbs);

    MQTTServerPkts_libInit(libCfg, &pkts_cbs);

    return 0;
}


//*****************************************************************************
//
//! \brief Clean and free all the allocated memory for the topic nodes
//
//*****************************************************************************
void MQTTServerCore_topicNodeExit(void)
{
    MQTTServerCore_TopicNode_t *node = MQTTServerCore_rootNode;

    /* Run over all the nodes and free them */
    while(NULL != node)
    {
        /* Search for the last down link hierarchy node */
        while ( NULL != node->dnHier )
        {
            node = node->dnHier;
        }

        /* Search for the last down link neighbor node */
        while ( NULL != node->dnNhbr )
        {
            node = node->dnNhbr;
        }

        /* If node doesn't have any down link neighbor or hierarchy */
        if ( ( NULL == node->dnNhbr ) && ( NULL == node->dnHier ) )
        {
            /* Check if the node has up link hierarchy */
            if (NULL != node->upHier)
            {
                /* Change the node to his upper link hierarchy */
                node = node->upHier;
                /* Free the down link hierarchy (the node that
                   we wanted to free */
                free_node(node->dnHier);
                /* Reset to NULL the pointer of the down link
                   hierarchy node that we freed */
                node->dnHier = NULL;
            }
            /* Check if the node has up link neighbor */
            else if (NULL != node->upNhbr)
            {
                /* Change the node to his upper link neighbor */
                node = node->upNhbr;
                /* Free the down link neighbor (the node that
                   we wanted to free */
                free_node(node->dnNhbr);
                /* Reset to NULL the pointer of the down link
                   neighbor node that we freed */
                node->dnNhbr = NULL;
            }
            /* Node that doesn't has any links is the root */
            else
            {
                free_node(node);
                node = NULL;
                MQTTServerCore_rootNode = NULL;
            }
        }
    }

    return;
}
