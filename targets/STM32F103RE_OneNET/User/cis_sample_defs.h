#ifndef _OBJECT_DEFS_H_
#define _OBJECT_DEFS_H_

#include "cis_api.h"


#define SAMPLE_OBJECT_MAX       2

#define SAMPLE_OID_A	        (3301)
#define SAMPLE_OID_B		    (3200)

#define SAMPLE_A_INSTANCE_COUNT        3
#define SAMPLE_A_INSTANCE_BITMAP      "001"

#define SAMPLE_B_INSTANCE_COUNT        5
#define SAMPLE_B_INSTANCE_BITMAP      "00001"

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
    int32_t intValue;
    float   floatValue;
    bool    boolValue;
    char    strValue[512];


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
    attributeA_intValue	    = 1,
    attributeA_floatValue	= 2,
    attributeA_boolValue 	= 3,
    attributeA_stringValue	= 4,
};

enum{
    actionA_1	        = 100,
};


static const cis_rid_t const_AttrIds_a[] = {
    attributeA_intValue,
    attributeA_floatValue,
    attributeA_boolValue,
    attributeA_stringValue,
};


static const cis_rid_t const_ActIds_a[] = {
    actionA_1,
};


//////////////////////////////////////////////////////////////////////////
//b object

typedef struct st_object_b
{
    int32_t intValue;
    float   floatValue;
    bool    boolValue;
    char    strValue[1024];

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
    attributeB_intValue	= 5503,
    attributeB_floatValue	= 8881,
    attributeB_stringValue	= 5750,
};

enum{
    actionB_1	        = 5505,
};


static const cis_rid_t const_AttrIds_b[] = {
    attributeB_intValue,
    attributeB_floatValue,
    attributeB_stringValue,
};


static const cis_rid_t const_ActIds_b[] = {
    actionB_1,
};


    
    

#endif//_OBJECT_DEFS_H_