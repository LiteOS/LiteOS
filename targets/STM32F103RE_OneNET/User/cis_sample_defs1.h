/*
 Copyright (c) 2017 Chinamobile

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

*/


#ifndef _OBJECT_DEFS_H_
#define _OBJECT_DEFS_H_

#include "cis_api.h"


#define SAMPLE_OBJECT_MAX           2   /* 定义2个对象 */

#define SAMPLE_OID_A                (3201)  /* digital output */
#define SAMPLE_OID_B                (3303)  /* temperature sensor */

#define SAMPLE_A_INSTANCE_COUNT     1  /* 定义1个实例 */
#define SAMPLE_A_INSTANCE_BITMAP   "1"

#define SAMPLE_B_INSTANCE_COUNT     1
#define SAMPLE_B_INSTANCE_BITMAP   "1"

typedef struct st_sample_object
{
    cis_oid_t         oid;
    cis_instcount_t   instCount;
    const char*       instBitmap;
    const cis_rid_t*  attrListPtr;
    uint16_t          attrCount;
    const cis_rid_t*  actListPtr;
    uint16_t          actCount;
}st_sample_object;


//a object

typedef struct st_object_a{
    bool    boolValue;
    char    strValue[128];

    //flag;
    uint8_t update;
}st_object_a;


typedef struct st_instance_a
{
    cis_iid_t   instId;
    bool        enabled;
    st_object_a instance;
}st_instance_a;


enum{
    attributeA_boolValue    = 5550,
    attributeA_stringValue  = 5750,
};


static const cis_rid_t const_AttrIds_a[] = {
    attributeA_boolValue,
    attributeA_stringValue,
};


//b object

typedef struct st_object_b
{
    float   floatValue;
    char    strValue[128];

    //flag;
    uint8_t update;
}st_object_b;

typedef struct st_instance_b
{
    cis_iid_t instId;
    bool    enabled;
    st_object_b instance;
}st_instance_b;

enum{
    attributeB_floatValue   = 5700,
    attributeB_stringValue  = 5701,
};

enum{
    actionB_1   = 5605,
};

static const cis_rid_t const_AttrIds_b[] = {
    attributeB_floatValue,
    attributeB_stringValue,
};

static const cis_rid_t const_ActIds_b[] = {
    actionB_1,
};


#endif//_OBJECT_DEFS_H_