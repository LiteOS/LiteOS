/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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


#ifdef __cplusplus
     extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "map_util.h"

typedef struct Map_elem_t
{
    List_Elem  node;
    const void *pKey;
    const void *pValue;
}Map_elem;

static uint32_t Map_HashString(const void *Key);

static uint32_t Map_HashInt32(const void *Key);

static uint32_t Map_ComapreString(const void* lhs, const void* rhs);

static uint32_t Map_ComapreInt(const void *lhs, const void* rhs);

static void Map_DeleteChain(List_List *pListHandle);

static bool Map_CheckDup(Map* const pMap_Handle, const int index, const void* Key);

const Map_Attr MapAttrKeyTypeStr =
{
     .pCompare_function = &Map_ComapreString,
     .pHash_function = &Map_HashString
};

const Map_Attr MapAttrKeyTypeInteger =
{
     .pCompare_function = &Map_ComapreInt,
     .pHash_function = &Map_HashInt32
};

/*
 *  ======== MapCalcIndex ========
 *  Calculate Index according to hashed key.
 */
static inline uint32_t Map_CalcIndex(Map* const pMap_Handle, const void *pKey)
{
    return(pMap_Handle->pHash_keys(pKey) & (HASH_TABLE_SIZE - 1));
}

/*
 *  ======== MapCreate ========
 *  Returns a handle to a map Object.
 */
Map *Map_Create(const Map_Attr* MapAttribute)
{
    Map *pMap_Handle = (Map*)(malloc(sizeof(Map)));

    if(!pMap_Handle)
    {
        return NULL;
    }

    memset(pMap_Handle, 0x0, sizeof(Map));

    if((MapAttribute->pCompare_function == NULL) || (MapAttribute->pHash_function == NULL))
    {
        free(pMap_Handle);
        return NULL;
    }

    pMap_Handle->pHash_keys    = MapAttribute->pHash_function;
    pMap_Handle->pCompare_keys = MapAttribute->pCompare_function;

    return pMap_Handle;
}

/*
 *  ======== MapDestroy ========
 *  Release all allotted resources for a given map handle.
 */
void Map_Destroy(Map* pMap_Handle)
{
    int32_t i;
    
    assert(pMap_Handle != NULL);

    for(i = 0 ; i < HASH_TABLE_SIZE; i++)
    {
        Map_DeleteChain(&pMap_Handle->HashTable[i]);
    }
    
    free(pMap_Handle);
    pMap_Handle = NULL;
}

/*
 *  ======== MapDeleteChain ========
 *  Delete entire chain of map elements.
 */
static void Map_DeleteChain(List_List *pListHandle)
{
Map_elem *itr = (Map_elem *)(List_tail(pListHandle));
    Map_elem *prev_itr;

    while(itr != NULL)
    {
        prev_itr = (Map_elem *)(List_prev(&itr->node));
        free(itr);
        itr = prev_itr;
    }
}

/*
 *  ======== MapInsert ========
 *  Inserts a <Key, Value> pair to a Map object.
 */
int32_t Map_Insert(Map* const pMap_Handle, const void* pKey, const void *pVal)
{
    int32_t index;
    Map_elem *elem;

    assert(pMap_Handle != NULL);
    assert(pKey != NULL);

    index = Map_CalcIndex(pMap_Handle, pKey);

    if(Map_CheckDup(pMap_Handle, index, pKey))
    {
        return MAP_ERROR_DUP_KEY;
    }

    elem = (Map_elem *)(calloc(1, sizeof(Map_elem)));

    if(!elem)
    {
        return MAP_ERROR_ALLOC_FAILED;
    }

    elem->pValue = pVal;
    elem->pKey   = pKey;
    List_put(&pMap_Handle->HashTable[index], &elem->node);

    return MAP_SUCCSESS;
}

/*
 *  ======== Map_CheckDup ========
 *  Checks if Key already exist in map.
 */
static bool Map_CheckDup(Map* const pMap_Handle, const int index, const void* pKey)
{
    Map_elem *itr = (Map_elem *)(List_head(&pMap_Handle->HashTable[index]));

    while(itr != NULL)
    {
        if(pMap_Handle->pCompare_keys(itr->pKey, pKey) != 0)
        {
           /* We have a duplicate value, return true */
           return true;
        }

        itr = (Map_elem *)(List_next(&itr->node));
    }

    /* if we made it to here, we have no duplicate key collision, return false */
    return false;
}

/*
 *  ======== MapRemove ========
 *  Remove a <Key, Value> pair from a Map object.
 */
int32_t Map_Remove(Map* const pMap_Handle, const void* pKey)
{
    int32_t index;
    Map_elem *itr;

    assert(pMap_Handle != NULL);
    assert(pKey != NULL);

    index = Map_CalcIndex(pMap_Handle, pKey);
    itr = (Map_elem *)(List_head(&pMap_Handle->HashTable[index]));
    
    while(itr != NULL)
    {
        if(pMap_Handle->pCompare_keys(itr->pKey, pKey) != 0)
        {
            List_remove((&pMap_Handle->HashTable[index]), &itr->node);
            free(itr);
            return MAP_SUCCSESS;
        }

        itr = (Map_elem *)(List_next(&itr->node));
    }

    return MAP_ERROR_ELEM_NOT_FOUND;
}

/*
 *  ======== MapGetValue ========
 *  Returns a Value given Key.
 */
void *Map_GetValue(Map* const pMap_Handle, const void *pKey)
{
    uint32_t index;
    Map_elem *itr;

    assert(pMap_Handle != NULL);
    assert(pKey != NULL);

    index = Map_CalcIndex(pMap_Handle, pKey);
    itr = (Map_elem *)(List_head(&pMap_Handle->HashTable[index]));

    while(itr != NULL)
    {
        if(pMap_Handle->pCompare_keys(itr->pKey, pKey) != 0)
        {
            return (void *)(itr->pValue);
        }

        itr = (Map_elem *)(List_next(&itr->node));
    }

    return NULL;
}

/*
 *  ======== MapHashString ========
 *  Hash function for String keys.
 */
static uint32_t Map_HashString(const void *key)
{
    register const uint8_t *key_str = (const uint8_t *)key;
    register uint32_t hash = 0;

    for (; *key_str; ++key_str)
    {
        hash += *(key_str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

/*
 *  ======== MapComapreString ========
 *  Returns true if lhs string equals to rhs string.
 */
static uint32_t Map_ComapreString(const void* lhs, const void* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);

    return(!strcmp((const char*)lhs, (const char*)rhs));
}

/*
 *  ======== MapHashInt32 ========
 *  Hash function for integer keys.
 */
static uint32_t Map_HashInt32(const void *Key)
{
    register const uint32_t *key_int = (const uint32_t *)Key;
    register uint32_t hash = 0;

    hash += *(key_int);

    hash += ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash += ((hash >> 16) ^ hash) * 0x45d9f3b;
    hash += ((hash >> 16) ^ hash);

    return hash;
}

/*
 *  ======== MapIsInt32Equals ========
 *  Returns true if lhs integer equals to rhs integer.
 */
static uint32_t Map_ComapreInt(const void *lhs, const void* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    return ((*(int32_t *)(lhs)) == (*(int32_t *)(rhs)));
}

#ifdef __cplusplus
}
#endif
