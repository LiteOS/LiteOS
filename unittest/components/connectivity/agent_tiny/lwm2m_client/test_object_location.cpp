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
#include "test_object_location.h"
/* Defines ------------------------------------------------------------------*/

/* Typedefs -----------------------------------------------------------------*/
/* Macros -------------------------------------------------------------------*/
/* Local variables ----------------------------------------------------------*/
/* Extern variables ---------------------------------------------------------*/
extern "C"
{
#include "object_comm.h"
    extern void display_location_object(lwm2m_object_t *object);
    extern lwm2m_object_t *get_object_location(void);
    extern void free_object_location(lwm2m_object_t *object);

    extern int atiny_cmd_ioctl(atiny_cmd_e cmd, char *arg, int len);
    extern lwm2m_data_t *lwm2m_data_new(int size);
    extern void *lwm2m_malloc(size_t s);

    static int stub_atiny_cmd_ioctl(atiny_cmd_e cmd, char *arg, int len)
    {
        return ATINY_ARG_INVALID;
    }

    static lwm2m_data_t *stub_lwm2m_data_new(int size)
    {
        return NULL;
    }

    static void *stub_lwm2m_malloc(size_t s)
    {
        return NULL;
    }

    static void *stub_lwm2m_malloc_ex(size_t s)
    {
        static int i = 0;
        if( i++ == 0)
        {
            void *mem = NULL;
            mem = atiny_malloc(s);
            return mem;
        }
        return NULL;
    }
}

/* Global variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/
/* Public functions ---------------------------------------------------------*/
TestObjectLocation::TestObjectLocation()
{
    TEST_ADD(TestObjectLocation::test_get_object_location);
    TEST_ADD(TestObjectLocation::test_display_location_object);
    TEST_ADD(TestObjectLocation::test_free_object_location);

    TEST_ADD(TestObjectLocation::test_prv_location_read);
}
TestObjectLocation::~TestObjectLocation()
{

}

void TestObjectLocation::test_get_object_location(void)
{
    lwm2m_object_t *pLocalObject = NULL;

    stubInfo stub_info;
    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc, &stub_info);
    pLocalObject = get_object_location();
    TEST_ASSERT_MSG((pLocalObject == NULL), "get_object_location() failed");
    cleanStub(&stub_info);

    setStub((void *)lwm2m_malloc, (void *)stub_lwm2m_malloc_ex, &stub_info);
    pLocalObject = get_object_location();
    TEST_ASSERT_MSG((pLocalObject == NULL), "get_object_location() failed");
    cleanStub(&stub_info);

    pLocalObject = get_object_location();
    TEST_ASSERT_MSG((pLocalObject != NULL), "get_object_location() failed");

    free_object_location(pLocalObject);
}

void TestObjectLocation::test_display_location_object(void)
{
    lwm2m_object_t *pLocalObject = NULL;

    /* pLocalObject == NULL is not checked in function display_location_object(lwm2m_object_t *) */
    //    stubInfo stub_info;
    //    setStub((void *)lwm2m_malloc,(void *)stub_lwm2m_malloc,&stub_info);
    //    pLocalObject = get_object_location();
    //    TEST_ASSERT_MSG((pLocalObject == NULL), "get_object_location() failed");
    //    cleanStub(&stub_info);
    //    display_location_object(pLocalObject);

    pLocalObject = get_object_location();
    TEST_ASSERT_MSG((pLocalObject != NULL), "get_object_location() failed");
    uint8_t ret = 0;
    display_location_object(pLocalObject);
    TEST_ASSERT_MSG((ret == 0), "display_location_object(...) failed");
    free_object_location(pLocalObject);
}

void TestObjectLocation::test_free_object_location(void)
{
    lwm2m_object_t *pLocalObject = NULL;

    /* pLocalObject == NULL is not checked in function free_object_location(lwm2m_object_t *) */
    //    stubInfo stub_info;
    //    setStub((void *)lwm2m_malloc,(void *)stub_lwm2m_malloc,&stub_info);
    //    pLocalObject = get_object_location();
    //    TEST_ASSERT_MSG((pLocalObject == NULL), "get_object_location() failed");
    //    cleanStub(&stub_info);
    //    free_object_location(pLocalObject);

    pLocalObject = get_object_location();
    TEST_ASSERT_MSG((pLocalObject != NULL), "get_object_location() failed");
    uint8_t ret = 0;
    free_object_location(pLocalObject);
    TEST_ASSERT_MSG((ret == 0), "free_object_location(...) failed");
}

void TestObjectLocation::test_prv_location_read(void)
{
    int num = 0;
    int ret = COAP_500_INTERNAL_SERVER_ERROR;
    lwm2m_data_t *pData = NULL;
    lwm2m_object_t *pLocalObject = NULL;

    pLocalObject = get_object_location();
    TEST_ASSERT_MSG((pLocalObject != NULL && pLocalObject->readFunc != NULL),
                    "get_object_location() failed");

    ret = pLocalObject->readFunc(0, &num, &pData, NULL, NULL);
    TEST_ASSERT_MSG((ret != COAP_500_INTERNAL_SERVER_ERROR && pData != NULL),
                    "prv_location_read() failed in object_location.c");

    stubInfo stub_info;
    // other cases like first case of switch in function prv_res2tlv()
    setStub((void *)atiny_cmd_ioctl, (void *)stub_atiny_cmd_ioctl, &stub_info);
    ret = pLocalObject->readFunc(0, &num, &pData, NULL, NULL);
    TEST_ASSERT_MSG((ret == COAP_400_BAD_REQUEST && pData != NULL),
                    "prv_location_read() failed in object_location.c");
    cleanStub(&stub_info);

    if(pData != NULL)
    {
        lwm2m_data_free(num, pData);
        pData = NULL;
    }

    setStub((void *)lwm2m_data_new, (void *)stub_lwm2m_data_new, &stub_info);
    num = 0;
    ret = pLocalObject->readFunc(0, &num, &pData, NULL, NULL);
    TEST_ASSERT_MSG((ret == COAP_500_INTERNAL_SERVER_ERROR && pData == NULL),
                    "prv_location_read() failed in object_location.c");
    cleanStub(&stub_info);

    free_object_location(pLocalObject);
}


/* Private functions --------------------------------------------------------*/

