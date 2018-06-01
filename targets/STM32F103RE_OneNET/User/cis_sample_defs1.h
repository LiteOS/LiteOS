#ifndef _OBJECT_DEFS_H_
#define _OBJECT_DEFS_H_

#include "cis_api.h"


#define SAMPLE_OBJECT_MAX       2			/* 定义2个对象 */

#define SAMPLE_OID_A	        (3201)  /* digital output */
#define SAMPLE_OID_B		    	(3303)  /* temperature sensor */

#define SAMPLE_A_INSTANCE_COUNT        1  /* 定义1个实例 */
#define SAMPLE_A_INSTANCE_BITMAP      "1"

#define SAMPLE_B_INSTANCE_COUNT        1
#define SAMPLE_B_INSTANCE_BITMAP      "1"

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


//////////////////////////////////////////////////////////////////////////
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
    attributeA_boolValue 	= 5550,
    attributeA_stringValue 	= 5750,
};


static const cis_rid_t const_AttrIds_a[] = {
    attributeA_boolValue,
    attributeA_stringValue,
};


//////////////////////////////////////////////////////////////////////////
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
    attributeB_floatValue	= 5700,
    attributeB_stringValue	= 5701,
};

enum{
    actionB_1	        = 5605,
};


static const cis_rid_t const_AttrIds_b[] = {
    attributeB_floatValue,
    attributeB_stringValue,
};


static const cis_rid_t const_ActIds_b[] = {
    actionB_1,
};


    
    

#endif//_OBJECT_DEFS_H_