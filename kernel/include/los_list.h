/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
 * Description: Doubly linked list
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

/**
 * @defgroup los_list Doubly linked list
 * @ingroup kernel
 */

#ifndef _LOS_LIST_H
#define _LOS_LIST_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_list
 * Structure of a node in a doubly linked list.
 */
typedef struct LOS_DL_LIST {
    struct LOS_DL_LIST *pstPrev; /**< Current node's pointer to the previous node */
    struct LOS_DL_LIST *pstNext; /**< Current node's pointer to the next node */
} LOS_DL_LIST;

/**
 * @ingroup los_list
 * @brief Initialize the input node to a doubly linked list.
 *
 * @par Description:
 * This API is used to initialize the input node (the first parameter list) to
 * a doubly linked list.
 * @attention
 * The parameter passed in should be a legal pointer.
 *
 * @param list    [IN] A node in a doubly linked list.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_HEAD
 * @since Huawei LiteOS V100R001C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListInit(LOS_DL_LIST *list)
{
    list->pstNext = list;
    list->pstPrev = list;
}

/**
 * @ingroup los_list
 * @brief Point to the next node of the current node.
 *
 * @par Description:
 * This API is used to point to the next node of the current node.
 * @attention
 * None.
 *
 * @param object  [IN] Type #LOS_DL_LIST *  The node in the doubly linked list.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_LAST
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_DL_LIST_FIRST(object) ((object)->pstNext)

/**
 * @ingroup los_list
 * @brief Point to the previous node of the current node.
 *
 * @par Description:
 * This API is used to point to the previous node of the current node.
 * @attention
 * None.
 *
 * @param object  [IN] Type #LOS_DL_LIST *  The node in the doubly linked list.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_FIRST
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_DL_LIST_LAST(object) ((object)->pstPrev)

/**
 * @ingroup los_list
 * @brief Insert a new node to a doubly linked list.
 *
 * @par Description:
 * This API is used to insert a new node after the list node to a doubly linked list.
 * @attention
 * The parameters passed in should be legal pointers.
 *
 * @param list    [IN] Doubly linked list which the new node will be inserted in.
 * @param node    [IN] The new node to be inserted.
 *
 * @retval None
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ListDelete | LOS_ListTailInsert | LOS_ListHeadInsert
 * @since Huawei LiteOS V100R001C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListAdd(LOS_DL_LIST *list, LOS_DL_LIST *node)
{
    node->pstNext = list->pstNext;
    node->pstPrev = list;
    list->pstNext->pstPrev = node;
    list->pstNext = node;
}

/**
 * @ingroup los_list
 * @brief Insert a node to a doubly linked list.
 *
 * @par Description:
 * This API is used to insert a new node before the list node to a doubly linked list.
 * @attention
 * The parameters passed in should be legal pointers.
 *
 * @param list     [IN] Doubly linked list which the new node will be inserted in.
 * @param node     [IN] The new node to be inserted.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ListAdd | LOS_ListHeadInsert
 * @since Huawei LiteOS V100R001C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListTailInsert(LOS_DL_LIST *list, LOS_DL_LIST *node)
{
    LOS_ListAdd(list->pstPrev, node);
}

/**
 * @ingroup los_list
 * @brief Insert a node to a doubly linked list.
 *
 * @par Description:
 * This API is used to insert a new node after the list node to a doubly linked list.
 *  It is same with #LOS_ListAdd.
 * @attention
 * The parameters passed in should be legal pointers.
 *
 * @param list     [IN] Doubly linked list which the new node will be inserted in.
 * @param node     [IN] The new node to be inserted.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ListAdd | LOS_ListTailInsert
 * @since Huawei LiteOS V100R001C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListHeadInsert(LOS_DL_LIST *list, LOS_DL_LIST *node)
{
    LOS_ListAdd(list, node);
}

/**
 * @ingroup los_list
 * @brief Delete a specified node from a doubly linked list.
 *
 * @par Description:
 * This API is used to delete a specified node from a doubly linked list.
 * @attention
 * The parameter passed in should be a legal pointer.
 *
 * @param node    [IN] Node to be deleted.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ListAdd
 * @since Huawei LiteOS V100R001C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListDelete(LOS_DL_LIST *node)
{
    node->pstNext->pstPrev = node->pstPrev;
    node->pstPrev->pstNext = node->pstNext;
    node->pstNext = NULL;
    node->pstPrev = NULL;
}

/**
 * @ingroup los_list
 * @brief Identify whether a specified doubly linked list is empty or not.
 *
 * @par Description:
 * This API is used to judge whether a doubly linked list is empty or not. It
 * returns a Boolean value.
 * @attention
 * The parameter passed in should be a legal pointer.
 *
 * @param list  [IN] Doubly linked list.
 *
 * @retval #TRUE  The doubly linked list is empty.
 * @retval #FALSE The doubly linked list is not empty.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE BOOL LOS_ListEmpty(LOS_DL_LIST *list)
{
    return (BOOL)(list->pstNext == list);
}

/**
 * @brief Obtain the offset of a field relative to the structure start address.
 *
 * @par  Description:
 * This API is used to obtain the offset of the structure member (field) relative to
 * the start address of the structure (type). And return the offset of #UINTPTR type.
 * @attention
 * None.
 *
 * @param type   [IN] Structure name.
 * @param field  [IN] The structure member name which needs to measure the offset.
 *
 * @retval #UINTPTR Offset of the field relative to the structure start address.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
#define OFFSET_OF_FIELD(type, field) ((UINTPTR)&((type *)0)->field)

/**
 * @ingroup los_list
 * @brief Obtain the offset of a structure member relative to the structure start address.
 *
 * @par  Description:
 * This API is used to obtain the offset of the structure member (member) relative to
 * the start address of the structure (type). And return the offset of #UINTPTR type.
 * @attention
 * None.
 *
 * @param type   [IN] Structure name.
 * @param member [IN] The structure member name which needs to measure the offset.
 *
 * @retval #UINTPTR Offset of the member relative to the structure start address.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_OFF_SET_OF(type, member) ((UINTPTR)&((type *)0)->member)

/**
 * @ingroup los_list
 * @brief Obtain the pointer to a structure that contains a doubly linked list.
 *
 * @par Description:
 * This API is used to obtain the pointer to a structure that contains the doubly
 * linked list which the first parameter item specified.
 * @attention
 * None.
 *
 * @param item    [IN] Type #LOS_DL_LIST *  The node of the doubly linked list.
 * @param type    [IN] Structure name.
 * @param member  [IN] The doubly linked list name in the structure.
 *
 * @retval The pointer to the structure that contains the doubly linked list. And
 * the doubly linked list has the node of the first parameter item.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_FOR_EACH_ENTRY | LOS_DL_LIST_FOR_EACH_ENTRY_SAFE
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_DL_LIST_ENTRY(item, type, member) \
    ((type *)(VOID *)((CHAR *)(item) - LOS_OFF_SET_OF(type, member)))

/**
 * @ingroup los_list
 * @brief Traverse a doubly linked list which is included in a given type structure.
 *
 * @par Description:
 * This API is used to traverse a doubly linked list which is included in a given type
 * structure. The API is a loop. The start node of the doubly linked list is the second
 * parameter list. And in each loop, the obtained pointer to a structure that contains
 * the list is outputted in the first parameter item.
 * @attention
 * None.
 *
 * @param item    [IN/OUT] The pointer to the structure that contains the doubly linked list.
 * @param list    [IN] Type #LOS_DL_LIST *  The start node of the doubly linked list to
 *                                          be traversed.
 * @param type    [IN] Structure name.
 * @param member  [IN] The doubly linked list name in the structure.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_ENTRY | LOS_DL_LIST_FOR_EACH_ENTRY_SAFE | LOS_DL_LIST_FOR_EACH
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_DL_LIST_FOR_EACH_ENTRY(item, list, type, member)             \
    for (item = LOS_DL_LIST_ENTRY((list)->pstNext, type, member);        \
         &(item)->member != (list);                                      \
         item = LOS_DL_LIST_ENTRY((item)->member.pstNext, type, member))

/**
 * @ingroup los_list
 * @brief Traverse a doubly linked list which is included in a given type structure. And
 * it is safe against removal of list entry.
 *
 * @par Description:
 * This API is used to traverse a doubly linked list which is included in a given type
 * structure. The API is a loop. The start node of the doubly linked list is the third
 * parameter list. And in each loop, the obtained pointer to a structure that contains
 * the list is outputted in the first parameter item. And the next node is outputted in
 * the second parameter next.
 * @attention
 * None.
 *
 * @param item    [IN/OUT] The pointer to the structure that contains the doubly linked list.
 * @param next    [IN/OUT] The pointer to the structure that contains the next node of the
 *                         doubly linked list.
 * @param list    [IN] Type #LOS_DL_LIST *  The start node of the doubly linked list to
 *                                          be traversed.
 * @param type    [IN] Structure name.
 * @param member  [IN] The doubly linked list name in the structure.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_ENTRY | LOS_DL_LIST_FOR_EACH_ENTRY | LOS_DL_LIST_FOR_EACH_SAFE
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_DL_LIST_FOR_EACH_ENTRY_SAFE(item, next, list, type, member)               \
    for (item = LOS_DL_LIST_ENTRY((list)->pstNext, type, member),                     \
         next = LOS_DL_LIST_ENTRY((item)->member->pstNext, type, member);             \
         &(item)->member != (list);                                                   \
         item = next, next = LOS_DL_LIST_ENTRY((item)->member.pstNext, type, member))

/**
 * @ingroup los_list
 * @brief Iterate over a doubly linked list of given type, and call hook for any extra procedures every time.
 *
 * @par Description:
 * This API is used to iterate over a doubly linked list of given type,
 * and call hook for any extra procedures every time.
 * @attention
 * None.
 *
 * @param item           [IN/OUT] Pointer to the structure that contains the doubly linked list that is to be traversed.
 * @param list           [IN] Pointer to the doubly linked list to be traversed.
 * @param type           [IN] Structure name.
 * @param member         [IN] Member name of the doubly linked list in the structure.
 * @param hook           [IN] Hook for extra procedures which will be called every time when dev is fetched.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_ENTRY | LOS_DL_LIST_FOR_EACH_ENTRY
 * @since Huawei LiteOS V200R005C10
 */
#define LOS_DL_LIST_FOR_EACH_ENTRY_HOOK(item, list, type, member, hook)  \
    for (item = LOS_DL_LIST_ENTRY((list)->pstNext, type, member), hook;  \
         &(item)->member != (list);                                      \
         item = LOS_DL_LIST_ENTRY((item)->member.pstNext, type, member), hook)

/**
 * @ingroup los_list
 * @brief Delete a specified node from a doubly linked list and reinitialize the node.
 *
 * @par Description:
 * This API is used to delete a specified node (the first parameter list) from the doubly
 * linked list. And reinitialize the deleted node to a doubly linked list.
 *
 * @attention
 * The parameter passed in should be a legal pointer.
 *
 * @param list    [IN] Node to be deleted and reinitialize to a doubly linked list.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ListInit | LOS_ListDelete
 * @since Huawei LiteOS V100R001C00
 */
LITE_OS_SEC_ALW_INLINE STATIC INLINE VOID LOS_ListDelInit(LOS_DL_LIST *list)
{
    list->pstNext->pstPrev = list->pstPrev;
    list->pstPrev->pstNext = list->pstNext;
    LOS_ListInit(list);
}

/**
 * @ingroup los_list
 * @brief Traverse a doubly linked list.
 *
 * @par Description:
 * This API is used to traverse a doubly linked list. The API is a loop. The start node of the
 * doubly linked list is the second parameter list. And in each loop, the obtained pointer to
 * the next node of the doubly linked list is outputted in the first parameter item.
 * @attention
 * None.
 *
 * @param item        [IN/OUT] Type #LOS_DL_LIST *  The pointer to the next node in the doubly
 *                                                  linked list.
 * @param list        [IN] Type #LOS_DL_LIST *   The pointer to the node of the doubly linked
 *                                               list to be traversed.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_FOR_EACH_SAFE | LOS_DL_LIST_FOR_EACH_ENTRY
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_DL_LIST_FOR_EACH(item, list) \
    for (item = (list)->pstNext;         \
         (item) != (list);               \
         item = (item)->pstNext)

/**
 * @ingroup los_list
 * @brief Traverse a doubly linked list safe against removal of list entry.
 *
 * @par Description:
 * This API is used to traverse a doubly linked list safe against removal of list entry. The
 * API is a loop. The start node of the doubly linked list is the third parameter list. And
 * in each loop, the obtained pointer to the next node of the doubly linked list is outputted
 * in the first parameter item. And the next node of the the node specified by first parameter
 * item is outputted in the second parameter next.
 * @attention
 * None.
 *
 * @param item        [IN/OUT] Type #LOS_DL_LIST *  The pointer to the next node in the doubly
 *                                                  linked list.
 * @param next        [IN/OUT] Type #LOS_DL_LIST *  The pointer to the next node of the the node
 *                                                  specified by first parameter item.
 * @param list        [IN]     Type #LOS_DL_LIST *  The pointer to the node of the doubly linked
 *                                                  list to be traversed.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_DL_LIST_FOR_EACH | LOS_DL_LIST_FOR_EACH_ENTRY_SAFE
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_DL_LIST_FOR_EACH_SAFE(item, next, list)      \
    for (item = (list)->pstNext, next = (item)->pstNext; \
         (item) != (list);                               \
         item = next, next = (item)->pstNext)

/**
 * @ingroup los_list
 * @brief Initialize a double linked list.
 *
 * @par Description:
 * This API is used to initialize the input node (the parameter list) to a double linked
 * list. The difference with LOS_ListInit is that the parameter list is not a pointer while
 * in LOS_ListInit it is a pointer.
 * @attention
 * None.
 *
 * @param list    [IN] Type #LOS_DL_LIST  A node to be initialized to a doubly linked list.
 *
 * @retval None.
 * @par Dependency:
 * <ul><li>los_list.h: the header file that contains the API declaration.</li></ul>
 * @see LOS_ListInit
 * @since Huawei LiteOS V100R001C00
 */
#define LOS_DL_LIST_HEAD(list) LOS_DL_LIST list = { &(list), &(list) }

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _LOS_LIST_H */
