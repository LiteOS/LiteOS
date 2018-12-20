

#include "test_hmac.h"


#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>


extern "C"
{
#include "los_base.h"
#include "los_task.ph"
#include "los_typedef.h"
#include "los_sys.h"


//#define MBEDTLS_MD_C

//#define MBEDTLS_MD5_C


 extern int mbedtls_hmac_calc(mbedtls_hmac_t *hmac_info);
 extern int mbedtls_md_setup( mbedtls_md_context_t *ctx, const mbedtls_md_info_t *md_info, int hmac );

 static int stub_mbedtls_md_setup(size_t size)
{
	return -1;
}

 
}

TestHmac::TestHmac()
{
   
    TEST_ADD(TestHmac::test_mbedtls_hmac_calc);
}


TestHmac::~TestHmac()
{
    
}

void TestHmac::test_mbedtls_hmac_calc(void)
{
    int ret;

    mbedtls_hmac_t hmac_info;
	unsigned char tsecret[20]="abc";
	unsigned char tinput[20]="123";
	unsigned char tdigst[20]="123";
    
	ret=mbedtls_hmac_calc(NULL);
	TEST_ASSERT_MSG((ret == MBEDTLS_ERR_MD_BAD_INPUT_DATA), "test_mbedtls_hmac_calc unnormal failed");

	hmac_info.digest=tdigst;
	hmac_info.input=tinput;
	hmac_info.secret=tsecret;
	hmac_info.digest_len=10;
	hmac_info.input_len=5;
	hmac_info.secret_len=5;
	hmac_info.hmac_type=MBEDTLS_MD_MD5;
	
	ret=mbedtls_hmac_calc(&hmac_info);
	TEST_ASSERT_MSG((ret == MBEDTLS_ERR_MD_BAD_INPUT_DATA), "test_mbedtls_hmac_calc unnormal failed");

	
	hmac_info.digest=tdigst;
	hmac_info.input=tinput;
	hmac_info.secret=tsecret;
	hmac_info.digest_len=18;
	hmac_info.input_len=5;
	hmac_info.secret_len=5;
	hmac_info.hmac_type=MBEDTLS_MD_MD5;
	

	ret=mbedtls_hmac_calc(&hmac_info);
	TEST_ASSERT_MSG((ret == ATINY_OK), "test_mbedtls_hmac_calc unnormal ok");

    stubInfo stub_info;
	setStub((void *)mbedtls_md_setup,(void *)stub_mbedtls_md_setup,&stub_info);
	ret = mbedtls_hmac_calc(&hmac_info);
	TEST_ASSERT_MSG((ret == ATINY_ARG_INVALID), "test_mbedtls_hmac_calc unnormal failed");
	cleanStub(&stub_info);
     

	


}

void TestHmac::setup()
{
   // printf("come into %d func, in file %s\n", ++funcno, __FILE__);
}


void TestHmac::tear_down()
{
   // printf("exit %d func, in file %s\n", funcno, __FILE__);
}




