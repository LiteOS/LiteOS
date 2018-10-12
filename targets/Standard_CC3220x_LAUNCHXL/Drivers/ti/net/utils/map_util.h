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
 /** ============================================================================
 *  @file       map_util.h
 *
 *  @brief      Map utility for SimpleLink SDK
 *
 *  This module provides a simple <Key, Value> pair data structure.
 *  User can store a <Key, Value> pair and get a proper value by providing it's corresponding Key.
 *
 *  structures:
 *     - ::Map_Attr: This structure holds both a hash function pointer for key hashing and a compare
 *                   function pointer used to compare keys. For convenience, two constant attribute structures
 *                   are predefined for hashing string keys 'MAP_ATTR_KEY_TYPE_STR' and integer keys 'MAP_ATTR_KEY_TYPR_INT'.
 *
 *     - ::Map: The structure that holds a chained hash table and user's 'compare keys' and 'hash keys'
 *              function pointers. According to user's choice, the created map instance can
 *              hash string type keys or 32-bit wide integer type keys by passing the corresponding Map_Attr structure.
 *              Both are using an internal hash functions, provided by this module.
 *
 *              User who wish to use it's own key type or hash function, could do so, by passing a map attribute structure
 *              Containing the user's function pointers.
 *
 *  For example, creating a hash table for string type keys and int32 type keys:
 *  @code
 *  Map *Map_StrKey;
 *  Map *Map_IntKey;
 *
 *  Map_StrKey = Map_Create(MAP_ATTR_KEY_TYPE_STR);
 *  Map_IntKey = Map_Create(MAP_ATTR_KEY_TYPR_INT);
 *
 *  @endcode
 *
 *  For example, creating a map for user-defined keys and custom functions:
 *  @code
 *  Map *Map_handle;
 *  Map_Attr my_map_Attr;
 *
 *  my_map_Attr.pCompare_function = My_Compare_function;
 *  my_map_Attr.pHash_function = My_Hash_function;
 *
 *  Map_handle = Map_Create(&my_map_Attr);
 *
 *  @endcode
 *
 *  The following shows how to create a Map with three elements, removing one, and getting all the rest.
 *
 *  @code
 *  Map *myMap_Hanlde = Map_Create(MAP_ATTR_KEY_TYPE_STR);
 *
 *  Status = Map_Insert(myMap_Hanlde, "foo", &123);
 *  Status = Map_Insert(myMap_Hanlde, "bar", &456);
 *  Status = Map_Insert(myMap_Hanlde, "foobar", "someValue");
 *
 *  Status = Map_Remove(myMap_Hanlde, "foobar");
 *  value = (int *)Map_GetValue(myMap_Hanlde, "bar");
 *  value = (int *)Map_GetValue(myMap_Hanlde, "foo");
 *
 *  @endcode
 *
 *  @note
 *  The APIs ::Map_Insert, ::Map_GetValue, and ::Map_Remove are not atomic.
 *  It is up to the application to provide thread-safety (e.g. Mutex lock / unlock).
 *
 *  Map utility cannot store two identical keys. Any attempt to insert an already
 *  existing key, would result in an error MAP_ERROR_DUP_KEY (-5000).
 *
 *  Map utility doesn't copy data, rather it keeps a pointer to key and value pair.
 *  It is up to the user to maintain and allocate a memory for each key and value.
 *
 *  ============================================================================
 */

/*! @cond*/

#ifndef _MAP_UTIL_H_
#define _MAP_UTIL_H_

#ifdef __cplusplus
     extern "C" {
#endif

#include <stdint.h>
#include <ti/drivers/utils/List.h>

#define HASH_TABLE_SIZE               (64)

#define MAP_SUCCSESS                  (0)
#define MAP_ERROR_DUP_KEY             (-5000)
#define MAP_ERROR_ALLOC_FAILED        (-5001)
#define MAP_ERROR_ELEM_NOT_FOUND      (-5002)

#define MAP_ATTR_KEY_TYPR_INT         &MapAttrKeyTypeInteger
#define MAP_ATTR_KEY_TYPE_STR         &MapAttrKeyTypeStr

typedef uint32_t(*Map_HashFunction)(const void *);
typedef uint32_t(*Map_CompareFunction)(const void *lhs, const void *rhs);

typedef struct Map_t
{
    Map_HashFunction    pHash_keys;
    Map_CompareFunction pCompare_keys;
    List_List           HashTable[HASH_TABLE_SIZE];
}Map;

typedef struct Map_Attr_t
{
    Map_HashFunction    pHash_function;
    Map_CompareFunction pCompare_function;
}Map_Attr;

extern const Map_Attr MapAttrKeyTypeInteger;
extern const Map_Attr MapAttrKeyTypeStr;

/*! @endcond*/

/*!
 *  @brief  Function which initialize the contents of a map structure.
 *
 *  @param  MapAttribute: Pointer to a map attribute structure.
 *
 *  @return On success: Handle to the allotted map structure, NULL otherwise.
 */
extern Map *Map_Create(const Map_Attr* MapAttribute);

/*!
 *  @brief  Function which releases all resources of a map structure.
 *
 *  @param  pMap_Handle : Handle to the allotted map structure.
 */
extern void Map_Destroy(Map* pMap_Handle);

/*!
 *  @brief  Inserts a <Key, Value> pair to a map structure.
 *
 *  @param  pMap_Handle: Handle to the allotted map structure.
 *
 *  @param  pKey: Pointer to a key.
 *
 *  @param  pVal: Pointer to a value associated with provided key.
 *
 *  @return On success: MAP_SUCCSESS (0), negative error code otherwise.
 */
extern int32_t Map_Insert(Map* const pMap_Handle, const void* pKey, const void *pVal);

/*!
 *  @brief  Removes a <Key, Value> pair from a map structure.
 *
 *  @param  pMap_Handle: Handle to the allotted map structure.
 *
 *  @param  Key: Pointer to a key.
 *
 *  @return On success: MAP_SUCCSESS (0), in case of an error MAP_ERROR_ELEM_NOT_FOUND (-5002).
 */
extern int32_t Map_Remove(Map* const pMap_Handle, const void* pKey);

/*!
 *  @brief  Gets a value by key.
 *
 *  @param  pMap_Handle: Handle to the allotted map structure.
 *
 *  @param  Key: Pointer to a key.
 *
 *  @return On success: pointer to requested value, NULL otherwise.
 */
void *Map_GetValue(Map* const pMap_Handle, const void *pKey);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _MAP_UTIL_H_ */
