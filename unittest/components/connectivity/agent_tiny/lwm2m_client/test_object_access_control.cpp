#include <cpptest.h>
#include <iostream>
#include <fstream>
#include <memory>
#include "test_object_access_control.h"

#define MAX_DATA_VAL 65535

typedef struct acc_ctrl_ri_s
{   // linked list:
    struct acc_ctrl_ri_s*   next;       // matches lwm2m_list_t::next
    uint16_t                resInstId;  // matches lwm2m_list_t::id, ..serverID
    // resource data:
    uint16_t                accCtrlValue;
} acc_ctrl_ri_t;

typedef struct acc_ctrl_oi_s
{   //linked list:
    struct acc_ctrl_oi_s*   next;       // matches lwm2m_list_t::next
    uint16_t                objInstId;  // matches lwm2m_list_t::id
    // resources
    uint16_t                objectId;
    uint16_t                objectInstId;
    uint16_t                accCtrlOwner;
    acc_ctrl_ri_t*          accCtrlValList;
} acc_ctrl_oi_t;

void TestObjectAccessControl::test_acc_ctrl_create_object()
{
	lwm2m_object_t *pAccCtrlObject = NULL;
	pAccCtrlObject = acc_ctrl_create_object();
	TEST_ASSERT_MSG((pAccCtrlObject != NULL), "acc_ctrl_create_object() failed");
	acl_ctrl_free_object(pAccCtrlObject);
}

void TestObjectAccessControl::test_acl_ctrl_free_object()
{
	lwm2m_object_t *test_objectP = NULL;
	test_objectP = acc_ctrl_create_object();

	test_objectP->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(acc_ctrl_oi_t));
	if (NULL != test_objectP->instanceList)
	{
		memset(test_objectP->instanceList, 0, sizeof(acc_ctrl_oi_t));
	}
	acl_ctrl_free_object(test_objectP);
	
	test_objectP = NULL;
}

void TestObjectAccessControl::test_acc_ctrl_obj_add_inst()
{
	lwm2m_object_t * test_accCtrlObjP = NULL;
	bool ret = acc_ctrl_obj_add_inst(test_accCtrlObjP,0,0,0,0);
	TEST_ASSERT_MSG((ret == false),"acc_ctrl_obj_add_inst() failed");

	test_accCtrlObjP = acc_ctrl_create_object();
	ret = acc_ctrl_obj_add_inst(test_accCtrlObjP,0,0,0,0);
	TEST_ASSERT_MSG((ret == true),"acc_ctrl_obj_add_inst() failed");
	acl_ctrl_free_object(test_accCtrlObjP);
}

void TestObjectAccessControl::test_acc_ctrl_oi_add_ac_val()
{
	lwm2m_object_t * test_accCtrlObjP = NULL;
	test_accCtrlObjP = acc_ctrl_create_object();
	test_accCtrlObjP->instanceList = (lwm2m_list_t*)lwm2m_malloc(sizeof(acc_ctrl_oi_t));
	if(NULL != test_accCtrlObjP->instanceList)
		{
			memset(test_accCtrlObjP->instanceList,0,sizeof(acc_ctrl_oi_t));
		}
	test_accCtrlObjP->instanceList->next = NULL;
	test_accCtrlObjP->instanceList->id = 1;
	bool ret = false;
	ret = acc_ctrl_oi_add_ac_val(test_accCtrlObjP,1,0,0);
	TEST_ASSERT_MSG((ret == true),"test_prv_add_ac_val() failed");
	test_accCtrlObjP->deleteFunc(1,test_accCtrlObjP);
	acl_ctrl_free_object(test_accCtrlObjP);
}

void TestObjectAccessControl::test_prv_delete()
{
	lwm2m_object_t * test_accCtrlObjP = NULL;
	test_accCtrlObjP = acc_ctrl_create_object();
	acc_ctrl_obj_add_inst(test_accCtrlObjP,0,0,0,0);
	uint8_t ret = test_accCtrlObjP->deleteFunc(0,test_accCtrlObjP);
	TEST_ASSERT_MSG((ret == COAP_202_DELETED),"test_prv_delete() failed");
	
	acl_ctrl_free_object(test_accCtrlObjP);
}

void TestObjectAccessControl::test_prv_read()
{
	uint8_t ret = 0;
	lwm2m_object_t *test_object = NULL;
	test_object = acc_ctrl_create_object();
	uint16_t test_instanceId = 0;
	int test_numDataP = 0;
	lwm2m_data_t * test_dataArrayP = NULL;
	lwm2m_data_cfg_t * test_dataCfg = NULL;
	ret = test_object->readFunc(test_instanceId,&test_numDataP,&test_dataArrayP,test_dataCfg,test_object);
	TEST_ASSERT_MSG((ret == COAP_404_NOT_FOUND),"test_prv_delete() failed");

	acc_ctrl_obj_add_inst(test_object,0,0,0,0);
	ret = test_object->readFunc(test_instanceId,&test_numDataP,&test_dataArrayP,test_dataCfg,test_object);
	TEST_ASSERT_MSG((ret == COAP_205_CONTENT),"test_prv_delete() failed");

//	acc_ctrl_oi_add_ac_val(test_object,0,0,0);
//	acc_ctrl_oi_add_ac_val(test_object,0,1,1);
	acc_ctrl_oi_s * test_instance = (acc_ctrl_oi_s*)lwm2m_malloc(sizeof(acc_ctrl_oi_s));
	if(NULL != test_instance)
	{
		memset(test_instance,0,sizeof(acc_ctrl_oi_s));
	}
	test_instance->objectId = 0;
	test_instance->objectInstId = 0;
	test_instance->objInstId = 0;
#if 1
	test_instance->accCtrlValList = (acc_ctrl_ri_t*)lwm2m_malloc(sizeof(acc_ctrl_ri_t));
	if(NULL != test_instance->accCtrlValList)
	{
		memset(test_instance->accCtrlValList,0,sizeof(acc_ctrl_ri_t));
	}
	test_instance->accCtrlValList->accCtrlValue = 0;
	test_instance->accCtrlValList->resInstId = 0;
#else	
	acc_ctrl_ri_s* test_accCtrlValList = NULL;
	test_accCtrlValList = (acc_ctrl_ri_t*)lwm2m_malloc(sizeof(acc_ctrl_ri_t));
	if(NULL != test_accCtrlValList)
		{
			memset(test_accCtrlValList,0,sizeof(acc_ctrl_ri_t));
		}
	test_accCtrlValList->accCtrlValue = 0;
	test_accCtrlValList->resInstId = 0;
	test_instance->accCtrlValList = test_accCtrlValList;
#endif	
	lwm2m_free(test_dataArrayP);

	test_instance->next = NULL;
	test_object->instanceList->next = (lwm2m_list_t*)test_instance;
	test_dataArrayP = (lwm2m_data_t*)lwm2m_malloc(sizeof(lwm2m_data_t));
	if(NULL != test_dataArrayP)
		{
			memset(test_dataArrayP,0,sizeof(lwm2m_data_t));
		}
	test_numDataP = 1;
	test_dataArrayP->id = 2;
	ret = test_object->readFunc(test_instanceId,&test_numDataP,&test_dataArrayP,test_dataCfg,test_object);

	//lwm2m_free(test_accCtrlValList);
	//lwm2m_free(test_instance->accCtrlValList);
	//lwm2m_free(test_instance);
	lwm2m_free(test_dataArrayP);
	acl_ctrl_free_object(test_object);
}

int test_lwm2m_data_decode_int(const lwm2m_data_t * dataP,int64_t * valueP)
{
	*valueP = 1;
	return 0;
}
/*
stubInfo si_lwm2m_data_decode_int;
setStub((void *)lwm2m_data_decode_int,(void *)test_lwm2m_data_decode_int,&si_lwm2m_data_decode_int);
cleanStub(&si_lwm2m_data_decode_int);
*/
void TestObjectAccessControl::test_prv_create()
{
/*	uint8_t ret = 0;
	uint16_t test_instanceId = 1;
	int test_numDataP = 0;
	lwm2m_data_t * test_dataArrayP;
	lwm2m_data_cfg_t * tet_dataCfg;
	lwm2m_object_t *test_object = NULL;
	test_object = acc_ctrl_create_object();
	test_object->readFunc(test_instanceId,&test_numDataP,&test_dataArrayP,tet_dataCfg,test_object);
	ret = test_object->createFunc(test_instanceId,test_numDataP,test_dataArrayP,test_object);
	TEST_ASSERT_MSG((ret == COAP_404_NOT_FOUND),"test_prv_create() failed");
*/	

	uint8_t ret = 0;
	uint16_t test_instanceId = 1;
	int test_numDataP = 0;
	lwm2m_data_t * test_dataArrayP;
	lwm2m_data_cfg_t * tet_dataCfg;
	lwm2m_object_t *test_object = NULL;
	test_object = acc_ctrl_create_object();
	test_dataArrayP = (lwm2m_data_t*)lwm2m_malloc(sizeof(lwm2m_data_t));

	test_dataArrayP->id = 0;
	{
		test_dataArrayP->type = LWM2M_TYPE_UNDEFINED;
		ret = test_object->createFunc(test_instanceId,test_numDataP,test_dataArrayP,test_object);
		TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST),"test_prv_create() failed");

		test_dataArrayP->type = LWM2M_TYPE_INTEGER;
		test_dataArrayP->value.asInteger = -1;
		ret = test_object->createFunc(test_instanceId,test_numDataP,test_dataArrayP,test_object);
		TEST_ASSERT_MSG((ret == COAP_406_NOT_ACCEPTABLE),"test_prv_create() failed");

		test_dataArrayP->value.asInteger = 1;
		ret = test_object->createFunc(test_instanceId,test_numDataP,test_dataArrayP,test_object);
		TEST_ASSERT_MSG((ret == COAP_201_CREATED),"test_prv_create() failed");
	}
	
	test_dataArrayP->id = 1;
	{
		test_dataArrayP->type = LWM2M_TYPE_UNDEFINED;
		ret = test_object->createFunc(test_instanceId,test_numDataP,test_dataArrayP,test_object);
		TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST),"test_prv_create() failed");	

		test_dataArrayP->type = LWM2M_TYPE_INTEGER;
		test_dataArrayP->value.asInteger = -1;
		ret = test_object->createFunc(test_instanceId,test_numDataP,test_dataArrayP,test_object);
		TEST_ASSERT_MSG((ret == COAP_406_NOT_ACCEPTABLE),"test_prv_create() failed");

		test_dataArrayP->value.asInteger = 1;
		ret = test_object->createFunc(test_instanceId,test_numDataP,test_dataArrayP,test_object);
		TEST_ASSERT_MSG((ret == COAP_201_CREATED),"test_prv_create() failed");
	}
	lwm2m_free(test_dataArrayP);
	acl_ctrl_free_object(test_object);

/*	lwm2m_data_t * test_tlvArray;
	test_tlvArray = (lwm2m_data_t*)lwm2m_malloc(sizeof(lwm2m_data_t));
	lwm2m_free(test_tlvArray);
/*	
	test_dataArrayP->id = 0;
	test_dataArrayP->type = LWM2M_TYPE_UNDEFINED;
	test_dataArrayP->value.asInteger = -1;
	ret = test_object->createFunc(test_instanceId,test_numDataP,test_dataArrayP,test_object);
*/	
}

void TestObjectAccessControl::test_prv_write()    		//prv_write_resources
{
	uint8_t ret = 0;
	lwm2m_object_t *test_object = NULL;
	test_object = NULL;
	test_object = acc_ctrl_create_object();
	lwm2m_data_t* test_tlvArray = NULL;
	lwm2m_data_t* test_subTlvArray = NULL;
	ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
	TEST_ASSERT_MSG((ret == COAP_404_NOT_FOUND),"test_prv_create() failed");

	acc_ctrl_obj_add_inst(test_object,0,0,0,0);
	test_tlvArray = (lwm2m_data_t*)lwm2m_malloc(sizeof(lwm2m_data_t));
	if(NULL != test_tlvArray)
		{
			memset(test_tlvArray,0,sizeof(test_tlvArray));
		}
	
	test_tlvArray->id = 0;
	ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
	TEST_ASSERT_MSG((ret == COAP_405_METHOD_NOT_ALLOWED),"test_prv_create() failed");

	test_tlvArray->id = 1;
	ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
	TEST_ASSERT_MSG((ret == COAP_405_METHOD_NOT_ALLOWED),"test_prv_create() failed");

	test_tlvArray->id = 2;
	
		test_tlvArray->type = LWM2M_TYPE_UNDEFINED;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST),"test_prv_create() failed");

		test_tlvArray->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
		acc_ctrl_oi_add_ac_val(test_object,0,1,1);
		test_tlvArray->value.asChildren.count = 0;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_204_CHANGED),"test_prv_create() failed");	

		test_tlvArray->value.asChildren.count = 1;
		test_tlvArray->value.asChildren.array = test_subTlvArray;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST),"test_prv_create() failed");

	/*	test_tlvArray->value.asChildren.array = (lwm2m_data_t*)lwm2m_malloc(sizeof(lwm2m_data_t));
		test_tlvArray->value.asChildren.array->type = LWM2M_TYPE_UNDEFINED;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST),"test_prv_create() failed");
	*/		
				test_subTlvArray = (lwm2m_data_t*)lwm2m_malloc(sizeof(lwm2m_data_t));
				if(NULL != test_subTlvArray)
					{
						memset(test_subTlvArray,0,sizeof(test_subTlvArray));
					}
				test_subTlvArray->type = LWM2M_TYPE_UNDEFINED;
				test_tlvArray->value.asChildren.array = test_subTlvArray;
				TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST),"test_prv_create() failed");
				lwm2m_free(test_subTlvArray);
			

	/*	test_tlvArray->value.asChildren.array->value.asInteger = -1;
		test_tlvArray->value.asChildren.array->type = LWM2M_TYPE_INTEGER;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_406_NOT_ACCEPTABLE),"test_prv_create() failed");
	*/		
		test_subTlvArray = (lwm2m_data_t*)lwm2m_malloc(sizeof(lwm2m_data_t));
		if(NULL != test_subTlvArray)
			{
				memset(test_subTlvArray,0,sizeof(test_subTlvArray));
			}
		test_subTlvArray->type = LWM2M_TYPE_INTEGER;
		test_subTlvArray->value.asInteger = -1;
		test_tlvArray->value.asChildren.array = test_subTlvArray;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_406_NOT_ACCEPTABLE),"test_prv_create() failed");
		lwm2m_free(test_subTlvArray);
		

	/*	test_tlvArray->value.asChildren.array->type = LWM2M_TYPE_INTEGER;
		test_tlvArray->value.asChildren.array->value.asInteger = 1;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_204_CHANGED),"test_prv_create() failed");
	*/	// ...
		
		test_subTlvArray = (lwm2m_data_t*)lwm2m_malloc(sizeof(lwm2m_data_t));
		if(NULL != test_subTlvArray)
				{
					memset(test_subTlvArray,0,sizeof(test_subTlvArray));
				}
		test_subTlvArray->type = LWM2M_TYPE_INTEGER;
		test_subTlvArray->value.asInteger = 1;
		test_tlvArray->value.asChildren.array = test_subTlvArray;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_204_CHANGED),"test_prv_create() failed");
		lwm2m_free(test_subTlvArray);
		
	
	test_tlvArray->id = 3;
	
		test_tlvArray->type = LWM2M_TYPE_UNDEFINED;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST),"test_prv_create() failed");
	
		test_tlvArray->type = LWM2M_TYPE_INTEGER;
		test_tlvArray->value.asInteger = -1;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_406_NOT_ACCEPTABLE),"test_prv_create() failed");

		test_tlvArray->value.asInteger = 1;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_204_CHANGED),"test_prv_create() failed");
	

//	test_subTlvArray = test_tlvArray->value.asChildren.array;
//	lwm2m_free(test_subTlvArray);
	lwm2m_free(test_tlvArray);
	acl_ctrl_free_object(test_object);

/*	{
		test_tlvArray->type = LWM2M_TYPE_UNDEFINED;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST), "prv_write() failed"); //prv_write_resources:287

		test_tlvArray->type = LWM2M_TYPE_MULTIPLE_RESOURCE;
		test_tlvArray->value.asChildren.count = 0;
		ret = test_object->writeFunc(0,0,test_tlvArray,test_object);
		TEST_ASSERT_MSG((ret == COAP_204_CHANGED), "prv_write() failed"); //prv_write_resources:302
	}
*/	
}

void TestObjectAccessControl::test_acc_auth_operate()
{
 	OBJ_ACC_OPERATE test_op;
	lwm2m_uri_t* test_uri = NULL;
	uint8_t ret = 0;
	uint16_t test_serverId = 1;
	lwm2m_context_t* test_contextP = NULL;
	test_contextP = (lwm2m_context_t*)lwm2m_malloc(sizeof(lwm2m_context_t));
	if(test_contextP != NULL)
	{
		memset(test_contextP,0,sizeof(lwm2m_context_t));
	}
	
	test_contextP->serverList = (lwm2m_server_t*)lwm2m_malloc(sizeof(lwm2m_server_t));
	if(test_contextP->serverList != NULL)
	{
		memset(test_contextP->serverList,0,sizeof(lwm2m_server_t));
	}
	test_contextP->serverList->dirty = 1;
	ret = acc_auth_operate(test_contextP,NULL,OBJ_ACC_READ,0);
	TEST_ASSERT_MSG((ret == COAP_NO_ERROR), "prv_write() failed");

	test_contextP->serverList->next = NULL;
	test_contextP->serverList->dirty = 0;
	ret = acc_auth_operate(test_contextP,NULL,OBJ_ACC_READ,0);
	TEST_ASSERT_MSG((ret == COAP_NO_ERROR), "prv_write() failed");

	lwm2m_server_t * test_serverList = NULL;
	test_serverList = (lwm2m_server_t*)lwm2m_malloc(sizeof(lwm2m_server_t));
	if(test_serverList != NULL)
	{
		memset(test_serverList,0,sizeof(lwm2m_server_t));
	}
	test_serverList->next = NULL;
	test_serverList->dirty = 0;
	test_contextP->serverList->next = test_serverList;
	ret = acc_auth_operate(test_contextP,NULL,OBJ_ACC_READ,0);
	TEST_ASSERT_MSG((ret == COAP_NO_ERROR), "prv_write() failed");

	ret = acc_auth_operate(test_contextP,NULL,OBJ_ACC_READ,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_401_UNAUTHORIZED), "prv_write() failed");

	test_contextP->objectList = (lwm2m_object_t*)lwm2m_malloc(sizeof(lwm2m_object_t));
	memset(test_contextP->objectList,0,sizeof(lwm2m_object_t));
	test_contextP->objectList->objID = LWM2M_ACL_OBJECT_ID;
	ret = acc_auth_operate(test_contextP,NULL,OBJ_ACC_READ,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_401_UNAUTHORIZED), "prv_write() failed");

	test_op = OBJ_ACC_CREATE;
	
		test_uri = (lwm2m_uri_t*)lwm2m_malloc(sizeof(lwm2m_uri_t));
		memset(test_uri, 0, sizeof(lwm2m_uri_t));
		test_uri->objectId = 0;
		acc_ctrl_oi_t* test_accCtrlOiP;
		test_accCtrlOiP = (acc_ctrl_oi_t *)lwm2m_malloc(sizeof(acc_ctrl_oi_t));
		memset(test_accCtrlOiP, 0, sizeof(acc_ctrl_oi_t));
		test_accCtrlOiP->objectId = 0;
		test_accCtrlOiP->objectInstId = 0; // 65535
	    test_contextP->objectList->instanceList = (lwm2m_list_t*)test_accCtrlOiP;
		ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
		TEST_ASSERT_MSG((ret == COAP_401_UNAUTHORIZED), "prv_write() failed");
		// free(test_accCtrlOiP);

		acc_ctrl_oi_t* test_accCtrlOiP_1;
		test_accCtrlOiP_1 = (acc_ctrl_oi_t *)lwm2m_malloc(sizeof(acc_ctrl_oi_t));
		memset(test_accCtrlOiP_1, 0, sizeof(acc_ctrl_oi_t));
		test_accCtrlOiP_1->objectId = 0;
		test_accCtrlOiP_1->objectInstId = 65535;
		test_accCtrlOiP_1->accCtrlOwner = 0;
		test_contextP->objectList->instanceList = (lwm2m_list_t*)test_accCtrlOiP_1;
		ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
		TEST_ASSERT_MSG((ret == COAP_401_UNAUTHORIZED), "prv_write() failed");
		// free(test_accCtrlOiP_1);

		acc_ctrl_oi_t* test_accCtrlOiP_2;
		test_accCtrlOiP_2 = (acc_ctrl_oi_t *)lwm2m_malloc(sizeof(acc_ctrl_oi_t));
		memset(test_accCtrlOiP_2, 0, sizeof(acc_ctrl_oi_t));
		test_accCtrlOiP_2->objectId = 0;
		test_accCtrlOiP_2->objectInstId = 65535;
		test_accCtrlOiP_2->accCtrlOwner = test_serverId;
		test_contextP->objectList->instanceList = (lwm2m_list_t*)test_accCtrlOiP_2;
		ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
		TEST_ASSERT_MSG((ret == COAP_NO_ERROR), "prv_write() failed");
		// free(test_accCtrlOiP_2);
	

	test_op = OBJ_ACC_DISCOVER;
	ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_405_METHOD_NOT_ALLOWED), "prv_write() failed");

	acc_ctrl_oi_t* test_accCtrlOiP_3;
	test_accCtrlOiP_3 = (acc_ctrl_oi_t *)lwm2m_malloc(sizeof(acc_ctrl_oi_t));
	memset(test_accCtrlOiP_3, 0, sizeof(acc_ctrl_oi_t));
	test_accCtrlOiP_3->objInstId = 0;
	test_contextP->objectList->instanceList = (lwm2m_list_t*)test_accCtrlOiP_3;

	test_uri->flag = 0x01;
	ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_NO_ERROR), "prv_write() failed");

	test_op = OBJ_ACC_READ;
	test_uri->objectId = LWM2M_ACL_OBJECT_ID;
	test_uri->flag = 0x02;
	test_uri->instanceId = 1;
	ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_401_UNAUTHORIZED), "prv_write() failed");
	// free(test_accCtrlOiP_3);
	
	acc_ctrl_oi_t* test_accCtrlOiP_4;
	test_accCtrlOiP_4 = (acc_ctrl_oi_t *)lwm2m_malloc(sizeof(acc_ctrl_oi_t));
	memset(test_accCtrlOiP_4, 0, sizeof(acc_ctrl_oi_t));
	test_accCtrlOiP_4->objInstId = 1;
	test_accCtrlOiP_4->accCtrlOwner = test_serverId;
	test_contextP->objectList->instanceList = (lwm2m_list_t*)test_accCtrlOiP_4;
	ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_NO_ERROR), "prv_write() failed");
	// free(test_accCtrlOiP_4);

	test_op = OBJ_ACC_EXCUTE;
	ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_405_METHOD_NOT_ALLOWED), "prv_write() failed");

	test_uri->flag = 0x0;
	test_op = OBJ_ACC_READ;
	ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_NO_ERROR), "prv_write() failed");

	test_uri->objectId = LWM2M_CONN_STATS_OBJECT_ID;
	test_uri->flag = 0x02;
	ret = acc_auth_operate(test_contextP,test_uri,test_op,test_serverId);
	TEST_ASSERT_MSG((ret == COAP_401_UNAUTHORIZED), "prv_write() failed");

	

	lwm2m_free(test_accCtrlOiP);
	lwm2m_free(test_accCtrlOiP_1);
	lwm2m_free(test_accCtrlOiP_2);
	lwm2m_free(test_accCtrlOiP_3);
	lwm2m_free(test_accCtrlOiP_4);
	lwm2m_free(test_uri);
	lwm2m_free(test_contextP->objectList);
	lwm2m_free(test_serverList);
	lwm2m_free(test_contextP->serverList);
	lwm2m_free(test_contextP);

}


TestObjectAccessControl::TestObjectAccessControl(){
	TEST_ADD(TestObjectAccessControl::test_prv_write);
	TEST_ADD(TestObjectAccessControl::test_prv_create);
	TEST_ADD(TestObjectAccessControl::test_prv_read);
	TEST_ADD(TestObjectAccessControl::test_prv_delete);
	TEST_ADD(TestObjectAccessControl::test_acc_ctrl_create_object);
	TEST_ADD(TestObjectAccessControl::test_acl_ctrl_free_object);
	TEST_ADD(TestObjectAccessControl::test_acc_ctrl_obj_add_inst);
	TEST_ADD(TestObjectAccessControl::test_acc_ctrl_oi_add_ac_val);
	TEST_ADD(TestObjectAccessControl::test_acc_auth_operate);

}


void TestObjectAccessControl::setup(){
    std::cout<<"in steup\n";
}

void TestObjectAccessControl::tear_down(){
    std::cout<<"in teardown\n";
}


