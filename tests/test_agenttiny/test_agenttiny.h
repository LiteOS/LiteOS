#ifndef TEST_AGENTTINY_H_
#define TEST_AGENTTINY_H_

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"
#include "atiny_lwm2m/agenttiny.h"
#include "osdepends/atiny_osdep.h"
#include <cmockery.h>
//#include "regresstest.h"
#include "atiny_lwm2m/agenttiny.h"
#include "agent_tiny_demo.h"

#define TEST_LOG    printf("Atiny_Test_Log : %s : %d \n",__FUNCTION__, __LINE__)

#define TEST_LWM2M_SERVER_IP					"192.168.1.102"     //leshan server
#define TEST_LWM2M_SERVER_DTLS_IP               "192.168.1.102"     //leshan server
#define TEST_LWM2M_SERVER_IP_INVALID			"not server ip"

#define TEST_LWM2M_SERVER_PORT                  "5683"
#define TEST_LWM2M_SERVER_DTLS_PORT				"5684"
#define TEST_LWM2M_SERVER_DTLS_PORT_INVALID 	"not a port"

#define TEST_LWM2M_SERVER_PSK_ID				"99990009"
#define TEST_LWM2M_SERVER_PSK_ID_INVALID		"not a psk id"

#define TEST_LWM2M_SERVER_PSK_INVALID			"not a psk"

extern atiny_event_e g_RegState;
extern void *test_phandle;
extern char TEST_LWM2M_SERVER_PSK[16];
extern UINT32 creat_test_task(void);
extern UINT32 creat_report_task(void);
extern void atiny_destroy(void *handle);

//const UnitTest tests;
extern int test_agenttyiny(void);

/********* atiny_init **********/
extern void test_atiny_init_000(void **state);
extern void test_atiny_init_001(void **state);
extern void test_atiny_init_002(void **state);
extern void test_atiny_init_003(void **state);
extern void test_atiny_init_004(void **state);
extern void test_atiny_init_005(void **state);
extern void test_atiny_init_006(void **state);
extern void test_atiny_init_007(void **state);
extern void test_atiny_init_008(void **state);
extern void test_atiny_init_009(void **state);
extern void test_atiny_init_010(void **state);
extern void test_atiny_init_011(void **state);
extern void test_atiny_init_012(void **state);
extern void test_atiny_init_013(void **state);
extern void test_atiny_init_014(void **state);
extern void test_atiny_init_015(void **state);
extern void test_atiny_init_016(void **state);
extern void test_atiny_init_017(void **state);
extern void test_atiny_init_018(void **state);
extern void test_atiny_init_019(void **state);
extern void test_atiny_init_020(void **state);
extern void test_atiny_init_021(void **state);
extern void test_atiny_init_022(void **state);
extern void test_atiny_init_023(void **state);
extern void test_atiny_init_024(void **state);
extern void test_atiny_init_025(void **state);
extern void test_atiny_init_026(void **state);
extern void test_atiny_init_027(void **state);
extern void test_atiny_init_028(void **state);
extern void test_atiny_init_029(void **state);
extern void test_atiny_init_030(void **state);
extern void test_atiny_init_031(void **state);
extern void test_atiny_init_032(void **state);
extern void test_atiny_init_033(void **state);
extern void test_atiny_init_034(void **state);
extern void test_atiny_init_035(void **state);
extern void test_atiny_init_036(void **state);
extern void test_atiny_init_037(void **state);

/********* atiny_bind **********/
extern void test_atiny_bind_000(void **state);
extern void test_atiny_bind_001(void **state);
extern void test_atiny_bind_002(void **state);
extern void test_atiny_bind_003(void **state);
extern void test_atiny_bind_004(void **state);
extern void test_atiny_bind_005(void **state);
extern void test_atiny_bind_006(void **state);
extern void test_atiny_bind_007(void **state);
extern void test_atiny_bind_008(void **state);
extern void test_atiny_bind_009(void **state);
extern void test_atiny_bind_010(void **state);
extern void test_atiny_bind_011(void **state);
extern void test_atiny_bind_012(void **state);
extern void test_atiny_bind_013(void **state);
extern void test_atiny_bind_014(void **state);
extern void test_atiny_bind_015(void **state);
extern void test_atiny_bind_016(void **state);
extern void test_atiny_bind_017(void **state);
extern void test_atiny_bind_018(void **state);
extern void test_atiny_bind_019(void **state);
extern void test_atiny_bind_020(void **state);
extern void test_atiny_bind_021(void **state);
extern void test_atiny_bind_022(void **state);
extern void test_atiny_bind_023(void **state);
extern void test_atiny_bind_024(void **state);
extern void test_atiny_bind_025(void **state);
extern void test_atiny_bind_026(void **state);


#endif

