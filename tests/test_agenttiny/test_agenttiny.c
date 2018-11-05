#include "test_agenttiny.h"

char TEST_LWM2M_SERVER_PSK[16] = {0x58,0xea,0xfd,0xab,0x2f,0x38,0x4d,0x39,0x80,0x69,0x4d,0x1c,0xda,0x69,0xb0,0x43};
UINT32 test_deinit_handle;
void *test_phandle;
extern atiny_event_e g_RegState;

int is_reg_ok()
{
    if (ATINY_REG_OK == g_RegState)
    {
        printf(">>>Bind register is OK!\n");
        return 1;
    }
    else
        return 0;
}

static void test_atiny_deinit(void)
{
    static int deinit_cnt = 0;
    const int CON_TIME_CNT = 10; // seconds
    static int del_handle_time = 0;
    
    while(1)
    {
        if(NULL != test_phandle && is_reg_ok())
        {    
            atiny_deinit(test_phandle);
            del_handle_time++;
            printf(">>>TEST:deinit success. use time %d seconds\n",deinit_cnt);
            printf(">>>TEST:deinit success. del_handle_time = %d \n",del_handle_time);
            deinit_cnt = 0;
        }
        else
        {
            deinit_cnt++;
            if(deinit_cnt >= CON_TIME_CNT)
            {
                if(NULL != test_phandle)
                {
                    atiny_deinit(test_phandle);
                    del_handle_time++;
                    printf(">>>TEST:deinit success. use time %d seconds\n",deinit_cnt);
                    printf(">>>TEST:deinit success. del_handle_time = %d \n",del_handle_time);
                    deinit_cnt = 0;
                }
            }
        }
        LOS_TaskDelay(4*250);
    }
}

UINT32 test_creat_deinit_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 1;
    task_init_param.pcName = "atiny_deinit_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)test_atiny_deinit;
    task_init_param.uwStackSize = 0x4000;

    uwRet = LOS_TaskCreate(&test_deinit_handle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

int test_agenttiny(void)
{
    UINT32 uwRet;
    uwRet = test_creat_deinit_task();
    if(LOS_OK != uwRet)
    {
        ATINY_LOG(LOG_INFO,">>>TEST:Deinit task create fail!");
        return -1;
    }
    
    const UnitTest test_atiny_bind[] =
    {
#ifdef WITH_DTLS
        
#else
//        unit_test(test_atiny_bind_000),
//        unit_test(test_atiny_bind_001),
//        unit_test(test_atiny_bind_002),
//        unit_test(test_atiny_bind_003), 
//        unit_test(test_atiny_bind_004),
//        unit_test(test_atiny_bind_005),
//        unit_test(test_atiny_bind_006),
//        unit_test(test_atiny_bind_007),
//        unit_test(test_atiny_bind_008),
//        unit_test(test_atiny_bind_009),
//        unit_test(test_atiny_bind_010),
//        unit_test(test_atiny_bind_011),
//        unit_test(test_atiny_bind_012),
//        unit_test(test_atiny_bind_013),
//        unit_test(test_atiny_bind_014),
//        unit_test(test_atiny_bind_015),
//        unit_test(test_atiny_bind_016),
//        unit_test(test_atiny_bind_017),
//        unit_test(test_atiny_bind_018),
//        unit_test(test_atiny_bind_019),
//        unit_test(test_atiny_bind_020),
//        unit_test(test_atiny_bind_021),
//        unit_test(test_atiny_bind_022),
//        unit_test(test_atiny_bind_023),
//        unit_test(test_atiny_bind_024),
//        unit_test(test_atiny_bind_025),
        unit_test(test_atiny_bind_026),
#endif
    };
    
	const UnitTest test_atiny_init[] =
	{
		unit_test(test_atiny_init_000),
	    unit_test(test_atiny_init_001),
		unit_test(test_atiny_init_002),
		unit_test(test_atiny_init_003),
		unit_test(test_atiny_init_004),
		unit_test(test_atiny_init_005),
		unit_test(test_atiny_init_006),
		unit_test(test_atiny_init_007),
		unit_test(test_atiny_init_008),
		unit_test(test_atiny_init_009),
		unit_test(test_atiny_init_010),
		unit_test(test_atiny_init_011),
		unit_test(test_atiny_init_012),
		unit_test(test_atiny_init_013),
		unit_test(test_atiny_init_014),
		unit_test(test_atiny_init_015),
		unit_test(test_atiny_init_016),
		unit_test(test_atiny_init_017),
		unit_test(test_atiny_init_018),
		unit_test(test_atiny_init_019),
		unit_test(test_atiny_init_020),
		unit_test(test_atiny_init_021),
		unit_test(test_atiny_init_022),
		unit_test(test_atiny_init_023),
		unit_test(test_atiny_init_024),
		unit_test(test_atiny_init_025),
		unit_test(test_atiny_init_026),
		unit_test(test_atiny_init_027),
		unit_test(test_atiny_init_028),
		unit_test(test_atiny_init_029),
		unit_test(test_atiny_init_030),
		unit_test(test_atiny_init_031),
		unit_test(test_atiny_init_032),
		unit_test(test_atiny_init_033),
		unit_test(test_atiny_init_034),
		unit_test(test_atiny_init_035),
		unit_test(test_atiny_init_036),
		unit_test(test_atiny_init_037),
	};
        
//	run_tests(test_atiny_init);
    run_tests(test_atiny_bind);
    
    uwRet = LOS_TaskDelete(test_deinit_handle);
    if(LOS_OK != uwRet)
    {
        ATINY_LOG(LOG_INFO,">>>TEST:Deinit task delete fail!");
        return -1;
    }
    
    return 0;
}
