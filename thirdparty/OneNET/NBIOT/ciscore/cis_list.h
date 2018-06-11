/*******************************************************************************
 *
 * Copyright (c) 2013 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    David Navarro, Intel Corporation - initial API and implementation
 *    Baijie & Longrong, China Mobile - Please refer to git log
 *    
 *******************************************************************************/
#ifndef _CIS_LIST_H_
#define _CIS_LIST_H_

#include "cis_def.h"
/*
 * Utility functions for sorted linked list
 */
typedef uint32_t     cis_listid_t;
typedef struct _cis_list_t
{
    struct _cis_list_t * next;
    cis_listid_t         id; 
} cis_list_t;

// defined in list.c
// Add 'node' to the list 'head' and return the new list
CIS_API cis_list_t *  cis_list_add(cis_list_t * head, cis_list_t * node);
// Return the node with ID 'id' from the list 'head' or NULL if not found
CIS_API cis_list_t *  cis_list_find(cis_list_t * head, cis_listid_t id);
CIS_API cis_list_t *  cis_list_find_u16(cis_list_t * head,uint16_t id);
// Remove the node with ID 'id' from the list 'head' and return the new list
CIS_API cis_list_t *  cis_list_remove(cis_list_t * head, cis_listid_t id, cis_list_t ** nodeP);
// Return the lowest unused ID in the list 'head'
CIS_API cis_listid_t      cis_list_newId(cis_list_t * head);
// Free a list. Do not use if nodes contain allocated pointers as it calls lwm2m_free on nodes only.
// If the nodes of the list need to do more than just "free()" their instances, don't use lwm2m_list_free().
CIS_API void              cis_list_free(cis_list_t * head);

CIS_API cis_listid_t      cis_list_count(cis_list_t * head);

#define CIS_LIST_ADD(H,N)   cis_list_add((cis_list_t *)H, (cis_list_t *)N);
#define CIS_LIST_RM(H,I,N)  cis_list_remove((cis_list_t *)H, I, (cis_list_t **)N);
#define CIS_LIST_FIND(H,I)  cis_list_find((cis_list_t *)H, I)
#define CIS_LIST_FIND_U16(H,I)  cis_list_find_u16((cis_list_t *)H, I)
#define CIS_LIST_FREE(H)    cis_list_free((cis_list_t *)H)
#define CIS_LIST_COUNT(H)   cis_list_count((cis_list_t *)H);

#endif //_CIS_LIST_H_
