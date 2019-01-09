/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
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
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/


/* Includes -----------------------------------------------------------------*/
#include "test_atiny_osdep.h"
#include <stdlib.h>
//#include <cstring>
/* Defines ------------------------------------------------------------------*/
/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/

static char g_state = 0;

extern "C"
{
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"
#include "osdepends/atiny_osdep.h"
#include "los_hwi.h"




extern unsigned int  g_vuwIntCount;
extern unsigned int  g_semCrt;


	

//#include "atiny_osdep.h"
//#include "mqtt_client.h"
#define MQTT_VERSION_3_1 (3)
#define MQTT_VERSION_3_1_1 (4)
#define MQTT_CLEAN_SESSION_TRUE (1)
#define MQTT_CLEAN_SESSION_FALSE (0)
#define MQTT_TOPIC_SUBSCRIBED_TRUE (1)
#define MQTT_TOPIC_SUBSCRIBED_FALSE (0)

#define LSOCFG_BASE_IPC_SEM		YES

void * stub_atiny_malloc_fail(size_t size)
{
    return NULL;
}

static UINT32  stub_LOS_MuxCreate (UINT32 *puwMuxHandle)
{
    if(g_state != 0)
	{
	    return -1;
	}
	return 0;
}

static UINT32 stub_LOS_MuxDelete(UINT32 uwMuxHandle)
{
	return 0;
}


extern uint64_t atiny_gettime_ms(void);
extern void *atiny_malloc(size_t size);
extern void atiny_free(void *ptr);
extern int atiny_snprintf(char *buf, unsigned int size, const char *format, ...);
extern int atiny_printf(const char *format, ...);
extern char *atiny_strdup(const char *ch);
extern void *atiny_mutex_create(void);
extern void atiny_mutex_destroy(void *mutex);
extern void atiny_mutex_lock(void *mutex);
extern void atiny_mutex_unlock(void *mutex);
extern int atiny_task_mutex_create(atiny_task_mutex_s *mutex);
extern int atiny_task_mutex_delete(atiny_task_mutex_s *mutex);
extern int atiny_task_mutex_lock(atiny_task_mutex_s *mutex);
extern int atiny_task_mutex_unlock(atiny_task_mutex_s *mutex);
}

/* Global variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestAtiny_Osdep::TestAtiny_Osdep()
{
    //TEST_ADD(TestAtiny_Osdep::test_osKernelGetTickCount);
    TEST_ADD(TestAtiny_Osdep::test_atiny_gettime_ms);
    TEST_ADD(TestAtiny_Osdep::test_atiny_malloc);
    TEST_ADD(TestAtiny_Osdep::test_atiny_free);
    TEST_ADD(TestAtiny_Osdep::test_atiny_snprintf);
    TEST_ADD(TestAtiny_Osdep::test_atiny_printf);
    TEST_ADD(TestAtiny_Osdep::test_atiny_strdup);
    TEST_ADD(TestAtiny_Osdep::test_atiny_mutex_create);
    TEST_ADD(TestAtiny_Osdep::test_atiny_mutex_destroy);
    TEST_ADD(TestAtiny_Osdep::test_atiny_mutex_lock);
    TEST_ADD(TestAtiny_Osdep::test_atiny_mutex_unlock);	
	TEST_ADD(TestAtiny_Osdep::test_atiny_task_mutex_create);
    TEST_ADD(TestAtiny_Osdep::test_atiny_task_mutex_delete);
    TEST_ADD(TestAtiny_Osdep::test_atiny_task_mutex_lock);
    TEST_ADD(TestAtiny_Osdep::test_atiny_task_mutex_unlock);	
}


TestAtiny_Osdep::~TestAtiny_Osdep(void)
{

}

/*
void TestAtiny_Osdep::test_osKernelGetTickCount(void)
{

}
*/

void TestAtiny_Osdep::test_atiny_gettime_ms(void)
{
	int result = 0;
	unsigned int oldvuwIntCount;
	oldvuwIntCount = g_vuwIntCount;
	
    g_vuwIntCount = 1;
	result = atiny_gettime_ms();
	TEST_ASSERT_MSG((result == 0), "atiny_gettime_ms(...) failed");
	extern UINT64 g_ullTickCount;
	g_vuwIntCount = 0;
	result = atiny_gettime_ms();
	TEST_ASSERT_MSG((result == g_ullTickCount), "atiny_gettime_ms(...) failed");

	g_vuwIntCount = oldvuwIntCount;
}

void TestAtiny_Osdep::test_atiny_malloc(void)
{
	char *p = NULL;
	p = (char *)atiny_malloc(100);
	TEST_ASSERT_MSG((p != 0), "atiny_malloc(...) failed");
	atiny_free((void *)p);
}

void TestAtiny_Osdep::test_atiny_free(void)
{
	int result = 0;
	//test in test_atiny_malloc
	TEST_ASSERT_MSG((result == 0), "atiny_free(...) failed");
}

void TestAtiny_Osdep::test_atiny_snprintf(void)
{
	int result;
	char test[50];
	result = atiny_snprintf(test, 50, "%s", "hello_word");
	//printf("result = %d\n", result);
	TEST_ASSERT_MSG((result == 10), "atiny_snprintf(...) failed");
}

void TestAtiny_Osdep::test_atiny_printf(void)
{
	int result;
	result = atiny_printf("hello_world");
	//printf("result = %d\n", result);
	TEST_ASSERT_MSG((result == 11), "atiny_printf(...) failed");
}

void TestAtiny_Osdep::test_atiny_strdup(void)
{
	
	char *p = NULL;
	char test[20] = "hello_world";
	p = atiny_strdup(NULL);
	TEST_ASSERT_MSG((p == NULL), "atiny_strdup(...) failed");
	stubInfo stub;
    setStub((void *)atiny_malloc, (void *)stub_atiny_malloc_fail, &stub);
	
	p = atiny_strdup(test);
	TEST_ASSERT_MSG((p == NULL), "atiny_strdup(...) failed");
	
    cleanStub(&stub); 
	
	
	p = atiny_strdup(test);
	TEST_ASSERT_MSG((p != NULL), "atiny_strdup(...) failed");
	atiny_free(p);
}

void TestAtiny_Osdep::test_atiny_mutex_create(void)
{
	void* p = NULL;
	unsigned int oldsemCrt,oldvuwIntCount;
	oldsemCrt = g_semCrt;
	oldvuwIntCount = g_vuwIntCount;
	
	g_vuwIntCount = 1;
	p = atiny_mutex_create();
	TEST_ASSERT_MSG((p == NULL), "atiny_mutex_create(...) failed");

	g_vuwIntCount = 0;
    
	g_semCrt = 1;
	p = atiny_mutex_create();
	TEST_ASSERT_MSG((p == NULL), "atiny_mutex_create(...) failed");
	g_semCrt = 0;
	p = atiny_mutex_create();
	TEST_ASSERT_MSG((p != NULL), "atiny_mutex_create(...) failed");

	g_semCrt = oldsemCrt;
	g_vuwIntCount = oldvuwIntCount;
}

void TestAtiny_Osdep::test_atiny_mutex_destroy(void)
{
	int result = 0;
	unsigned int oldvuwIntCount;

	oldvuwIntCount = g_vuwIntCount;

	g_vuwIntCount = 1;
	atiny_mutex_destroy(NULL);
	
	g_vuwIntCount = 0;
	atiny_mutex_destroy(NULL);
	TEST_ASSERT_MSG((result == 0), "atiny_mutex_destroy(...) failed");
	
	SEM_CB_S  test_mutex;
	
	test_mutex.usSemID = 1;
	
	atiny_mutex_destroy(&test_mutex);
	TEST_ASSERT_MSG((result == 0), "atiny_mutex_destroy(...) failed");

	g_vuwIntCount = oldvuwIntCount;
}

void TestAtiny_Osdep::test_atiny_mutex_lock(void)
{
	int result = 0;
    unsigned int oldvuwIntCount;

	oldvuwIntCount = g_vuwIntCount;
	
	atiny_mutex_lock(NULL);
	TEST_ASSERT_MSG((result == 0), "atiny_mutex_lock(...) failed");
	
	g_vuwIntCount = 1;
	SEM_CB_S  test_mutex;
	
	test_mutex.usSemID = 1;
    atiny_mutex_lock(&test_mutex);
	TEST_ASSERT_MSG((result == 0), "atiny_mutex_lock(...) failed");
	
	g_vuwIntCount = 0;
	test_mutex.usSemID = 1;
	atiny_mutex_lock(&test_mutex);
	TEST_ASSERT_MSG((result == 0), "atiny_mutex_lock(...) failed");
}

void TestAtiny_Osdep::test_atiny_mutex_unlock(void)
{
	int result = 0;
	atiny_mutex_unlock(NULL);
	TEST_ASSERT_MSG((result == 0), "atiny_mutex_unlock(...) failed");
	
	SEM_CB_S para_mutex, *test_mutex = 	NULL;
	memset(&para_mutex, 0, sizeof(SEM_CB_S));
	test_mutex = &para_mutex;
	
	para_mutex.usSemID = 10;
	atiny_mutex_unlock(test_mutex);
	TEST_ASSERT_MSG((result == 0), "atiny_mutex_unlock(...) failed");
}	


void TestAtiny_Osdep::test_atiny_task_mutex_create(void)
{

	int result = 0;
	result = atiny_task_mutex_create(NULL);

	TEST_ASSERT_MSG((result == -1), "atiny_task_mutex_create(...) failed");

	g_state = 1;
	atiny_task_mutex_s test_mutex;
	test_mutex.mutex = 0;
	test_mutex.valid = 0;
	stubInfo stub_info;
	
	setStub((void *)LOS_MuxCreate,(void *)stub_LOS_MuxCreate,&stub_info);
    result = atiny_task_mutex_create(&test_mutex);
    TEST_ASSERT_MSG((result == -1), "atiny_task_mutex_create() failed");
    cleanStub(&stub_info);

	g_state = 0;
	setStub((void *)LOS_MuxCreate,(void *)stub_LOS_MuxCreate,&stub_info);
	result = atiny_task_mutex_create(&test_mutex);
    TEST_ASSERT_MSG((result == LOS_OK), "atiny_task_mutex_create() failed");
    cleanStub(&stub_info);


	
		

	
	
}
void TestAtiny_Osdep::test_atiny_task_mutex_delete(void)
{
	int result = 0;
	result = atiny_task_mutex_delete(NULL);
	TEST_ASSERT_MSG((result == -1), "atiny_task_mutex_delete(...) failed");

	atiny_task_mutex_s test_mutex;
	test_mutex.mutex = 1;
	test_mutex.valid = 1;
    stubInfo stub_info;
	setStub((void *)LOS_MuxDelete,(void *)stub_LOS_MuxDelete,&stub_info);
	result = atiny_task_mutex_delete(&test_mutex);
    TEST_ASSERT_MSG((result == 0), "atiny_task_mutex_delete() failed");
    cleanStub(&stub_info);

	

}
void TestAtiny_Osdep::test_atiny_task_mutex_lock(void)
{
	int result = 0;
	result = atiny_task_mutex_lock(NULL);
	TEST_ASSERT_MSG((result == -1), "atiny_task_mutex_lock(...) failed");

	atiny_task_mutex_s test_mutex;
	test_mutex.mutex = 1;
	test_mutex.valid = 1;
	result = atiny_task_mutex_lock(&test_mutex);
    TEST_ASSERT_MSG((result == 0), "atiny_task_mutex_lock() failed");
	
}
void TestAtiny_Osdep::test_atiny_task_mutex_unlock(void)
{
	int result = 0;
	result = atiny_task_mutex_unlock(NULL);
	TEST_ASSERT_MSG((result == -1), "atiny_task_mutex_unlock(...) failed");

	atiny_task_mutex_s test_mutex;
	test_mutex.mutex = 1;
	test_mutex.valid = 1;
	result = atiny_task_mutex_unlock(&test_mutex);
    TEST_ASSERT_MSG((result == 0), "atiny_task_mutex_unlock() failed");
}


void TestAtiny_Osdep::setup()
{

}

void TestAtiny_Osdep::tear_down()
{

}

