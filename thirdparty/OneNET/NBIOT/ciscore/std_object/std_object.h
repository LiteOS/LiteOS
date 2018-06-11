#ifndef STD_OBJECT_H_
#define STD_OBJECT_H_

#include "../cis_api.h"
#include "../cis_internals.h"
extern int g_reboot;


typedef enum et_std_objectid
{
    std_object_security	= 0,
    std_object_count = 1,
}cis_std_objectid;


/*
 * Standard Object IDs
 */
#define CIS_SECURITY_OBJECT_ID          (0)



/*
 * Resource IDs for the LWM2M Security Object
 */
#define CIS_SECURITY_URI_ID                 0
#define CIS_SECURITY_BOOTSTRAP_ID           1
#define CIS_SECURITY_SECURITY_ID            2
#define CIS_SECURITY_PUBLIC_KEY_ID          3
#define CIS_SECURITY_SERVER_PUBLIC_KEY_ID   4
#define CIS_SECURITY_SECRET_KEY_ID          5
#define CIS_SECURITY_SMS_SECURITY_ID        6
#define CIS_SECURITY_SMS_KEY_PARAM_ID       7
#define CIS_SECURITY_SMS_SECRET_KEY_ID      8
#define CIS_SECURITY_SMS_SERVER_NUMBER_ID   9
#define CIS_SECURITY_SHORT_SERVER_ID        10
#define CIS_SECURITY_HOLD_OFF_ID            11

#define CIS_SECURITY_MODE_PRE_SHARED_KEY  0
#define CIS_SECURITY_MODE_RAW_PUBLIC_KEY  1
#define CIS_SECURITY_MODE_CERTIFICATE     2
#define CIS_SECURITY_MODE_NONE            3


typedef cis_coapret_t (*std_object_read_callback)(cis_iid_t instanceId,int * numDataP, st_data_t ** dataArrayP,st_object_t * objectP);
typedef cis_coapret_t (*std_object_exec_callback)(cis_iid_t instanceId,uint16_t resourceId,uint8_t * buffer,int length,st_object_t * objectP);
typedef cis_coapret_t (*std_object_write_callback)(cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP);


struct st_std_object_callback_mapping
{
    cis_oid_t           stdObjectId;
    std_object_read_callback onRead;
    std_object_write_callback onWrite;
    std_object_exec_callback onExec;
};


/*
 * object.c
 */
cis_coapret_t std_object_read_handler(cis_iid_t instanceId,int * numDataP, st_data_t ** dataArrayP,st_object_t * objectP);
cis_coapret_t std_object_exec_handler(cis_iid_t instanceId,uint16_t resourceId,uint8_t * buffer,int length,st_object_t * objectP);
cis_coapret_t std_object_write_handler(cis_iid_t instanceId,int numData,st_data_t * dataArray,st_object_t * objectP);

cis_coapret_t std_object_writeInstance(st_context_t * contextP,st_uri_t * uriP, st_data_t * dataP);
cis_list_t* std_object_get_securitys();
cis_list_t* std_object_put_securitys(cis_list_t* targetP);
void        std_object_remove_securitys(cis_list_t* targetP);





/*
 * object_security.c
 */
bool            std_security_create(int instanceId, const char* serverHost,bool isBootstrap,st_object_t * securityObj);
void            std_security_clean();
char *          std_security_get_host(cis_iid_t instanceId);


#endif /* STD_OBJECT_H_ */
