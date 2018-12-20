

#include "test_flash_manager.h"


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


extern void flash_manager_init(int (*cmd_ioctl)(mqtt_cmd_e cmd, void *arg, int32_t len));
extern int flash_manager_read(flash_info_s *flash_info);
extern int flash_manager_write(const flash_info_s *flash_info);

void *stub_atiny_malloc(size_t size)
{
	return NULL;
}

char *stub_atiny_strdup(const char *ch)
{
    return NULL;
}



static int example_cmd_ioctl(mqtt_cmd_e cmd, void *arg, int32_t len)
{
   uint8_t *length_buf=(uint8_t *)arg;
   uint8_t  context[8]={0x32,0x12,0x38,0x57,0x20,0x00,0x00,0x00};
   int i;
   if(len<=8)
  {
   memcpy(length_buf,context,sizeof(context));

  }
   else
  {
   memcpy(length_buf,context,sizeof(context));
   
   char example0[]="abc";
   char *length_bufv=(char *)(length_buf+8);
   
   for(i=0;i<4;i++)
   {
   
   memcpy(length_bufv,example0,sizeof(example0));
   length_bufv+=sizeof(example0);
   
   }
  }
   return 0;
}
static int example_cmd_ioctlb(mqtt_cmd_e cmd, void *arg, int32_t len)
{
   uint8_t *length_buf=(uint8_t *)arg;
   uint8_t  context[8]={0x32,0x12,0x38,0x57,0x20,0x00,0x00,0x00};
   memcpy(length_buf,context,sizeof(context));
   if (len>10)
   	return -1;
   
   return 0;
}


static int example_cmd_ioctl1(mqtt_cmd_e cmd, void *arg, int32_t len)
{
	return -1;
}


static int example_cmd_ioctl2(mqtt_cmd_e cmd, void *arg, int32_t len)
{
   uint8_t *length_buf=(uint8_t *)arg;
   uint8_t  context[8]={0x3f,0x12,0x38,0x57,0x20,0x00,0x00,0x00};
   memcpy(length_buf,context,sizeof(context));
   return 0;
}

static int example_cmd_ioctl3(mqtt_cmd_e cmd, void *arg, int32_t len)
{
	uint8_t *length_buf=(uint8_t *)arg;
	uint8_t  context[8]={0x32,0x12,0x38,0x57,0x01,0x00,0x00,0x00};
	memcpy(length_buf,context,sizeof(context));
    return 0;

}


 
}

TestFlashmanager::TestFlashmanager()
{
   
    TEST_ADD(TestFlashmanager::test_flash_manager_init);
	TEST_ADD(TestFlashmanager::test_flash_manager_read);
	TEST_ADD(TestFlashmanager::test_flash_manager_write);
}


TestFlashmanager::~TestFlashmanager()
{
    
}

void TestFlashmanager::test_flash_manager_init(void)
{
  flash_manager_init(example_cmd_ioctl);
}

void TestFlashmanager::test_flash_manager_read(void)
{
  int ret;
  flash_info_s flash_info;
  stubInfo stub_info;
  stubInfo stub_info1;
  
  flash_manager_init(example_cmd_ioctl1);
  ret= flash_manager_read(&flash_info);//-1
  TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "flash_manager_read(...) failed");

  flash_manager_init(example_cmd_ioctl2);
  ret=flash_manager_read(&flash_info);//0
  TEST_ASSERT_MSG((ATINY_OK == ret), "flash_manager_read(...) ok");

  flash_manager_init(example_cmd_ioctl3);
  ret=flash_manager_read(&flash_info);//0
  TEST_ASSERT_MSG((ATINY_OK == ret), "flash_manager_read(...) ok");

  flash_manager_init(example_cmd_ioctl);
  setStub((void *)atiny_malloc,(void *)stub_atiny_malloc,&stub_info);
  ret = flash_manager_read(&flash_info);//-4
  TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "flash_manager_read(...) failed");
  cleanStub(&stub_info);

  flash_manager_init(example_cmd_ioctlb);
  ret=flash_manager_read(&flash_info);//-1
  TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "flash_manager_read(...) failed");

  flash_manager_init(example_cmd_ioctl);
  setStub((void *)atiny_strdup,(void *)stub_atiny_strdup,&stub_info1);
  ret = flash_manager_read(&flash_info);//-4
  TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "flash_manager_read(...) failed");
  	
  cleanStub(&stub_info1);

  
  flash_manager_init(example_cmd_ioctl);
  ret=flash_manager_read(&flash_info);//-9
  TEST_ASSERT_MSG((ATINY_ERR == ret), "flash_manager_read(...) failed");
}


void TestFlashmanager::test_flash_manager_write(void)
{   
    int ret; 
	flash_info_s flash_info;
	char strtest[]=
	"-----BEGIN CERTIFICATE-----\r\n"
	"MIIEwTCCAqmgAwIBAgIRdi8oqJITu4uSWV2C/dUS1swwDQYJKoZIhvcNAQELBQAw\r\n"
	"PDELMAkGA1UEBhMCQ04xDzANBgNVBAoTBkh1YXdlaTEcMBoGA1UEAxMTSHVhd2Vp\r\n"
	"IEVxdWlwbWVudCBDQTAeFw0xNzAyMTYwNjM0MzVaFw00MTEwMDgwNjM0MzVaME0x\r\n"
	"CzAJBgNVBAYTAkNOMQ8wDQYDVQQKEwZIdWF3ZWkxLTArBgNVBAMTJEh1YXdlaSBD\r\n"
	"bG91ZCBDb3JlIE5ldHdvcmsgUHJvZHVjdCBDQTCCASIwDQYJKoZIhvcNAQEBBQAD\r\n"
	"ggEPADCCAQoCggEBAKQQz5uvp3lmtK9uzeje7cZUF8RlRKavEQj9EQk45ly9a/Kr\r\n"
	"07TlGIhaZv7j+N9ZV1jTiwA0+XWlni1anjy01qsBQ22eIzX3HQ3fTMjPfk67SFhS\r\n"
	"aHdzkJwO66/Nojnaum84HfUTBuXfgiBNH4C2Bc9YBongLktwunqMGvMWEKj4YqjN\r\n"
	"bjjZQ1G1Qnhk15qpEWI9YUv0I5X94oT5idEMIH+V+2hcW/6GmztoOgCekW3GPHGl\r\n"
	"UJLt3cSaDkp1b5IchnGpwocZLJMd+V3emcLhbjXewIY3meUIkXMrqJ12L3ltkRVp\r\n"
	"nHElgmpvp3dBjUrBiITLakrGq8P/Ta7OO5kf9JUCAwEAAaOBrDCBqTAfBgNVHSME\r\n"
	"GDAWgBQq+BBZJ4A1H6d8ujufKuRKqpuS6jBGBgNVHSAEPzA9MDsGBFUdIAAwMzAx\r\n"
	"BggrBgEFBQcCARYlaHR0cDovL3N1cHBvcnQuaHVhd2VpLmNvbS9zdXBwb3J0L3Br\r\n"
	"aTAPBgNVHRMECDAGAQH/AgEAMA4GA1UdDwEB/wQEAwIBBjAdBgNVHQ4EFgQU9CcR\r\n"
	"7wi0DatgF91OjC2HvAn0bG4wDQYJKoZIhvcNAQELBQADggIBADli3WJezyDe4qVl\r\n"
	"jSpF3kxRvBPf6hPhf81WT/A3lo5f7rTOEkRqTB515i5p8Xv8QgP8WTcMu22K5oZt\r\n"
	"6iV4PpWCaEVaHgAYeI8sjjWqI498nKWrJ1kaJkdOxB3omVa2Fet7xDCL6MR2jDZq\r\n"
	"dtZGF2XCIiNuZhvTYOTvKTfuzN5/gi/z8GD8xP95V4vX3sB2jqgleMTirFdeA+RB\r\n"
	"HDbS55MSIfA2jeXJt0IHoB5FKshcCPNLIW/s0O7lbSH62o4d+5PeVV8tbMESQ6Ve\r\n"
	"NSRt22+n6Z2Z6n/ZMfM2jSziEZNIyPXQtywltkcrhRIbxWoB0IEr0Ci+7FVr9CRu\r\n"
	"ZlcpliSKemrxiLo5EkoznWwxfUP11i473lUVljloJRglYWh6uo9Ci5Cu001occ4L\r\n"
	"9qs13MTtpC96LouOyrqBUOlUmJvitqCrHSfqOowyi8B0pxH/+m+Q8+fP9cBztw22\r\n"
	"JId8bth5j0OUbNDoY7DnjQLCI0bEZt4RSpQN1c6xf90gHutM62qoGI6NKlb2IH+r\r\n"
	"Yfun6P4jYTN9vMnaDfxBH7ofz4q9nj27UBkX9ebqM8kS+RijnUUy8L2N6KsUpp2R\r\n"
	"01cjcmp699kFIJ7ShCOmI95ZC9cociTnhTK6WScCweBH7eBxZwoQLi5ER3VkDs1r\r\n"
	"rqnNVUgf2j9TOshCI6zuaxsA35wr\r\n"
	"-----END CERTIFICATE-----\r\n";

	flash_info.items[0]=strtest;
	flash_info.items[1]=strtest;
	flash_info.items[2]=strtest;
	flash_info.items[3]=strtest;
	

	
	ret=flash_manager_write(&flash_info);//-9
	TEST_ASSERT_MSG((ATINY_ERR == ret), "flash_manager_write(...) failed");

	
	
	flash_info.items[0]="123\0";
	flash_info.items[1]="123\0";
	flash_info.items[2]="123\0";
	flash_info.items[3]="123\0";

	

	stubInfo stub_info;

	setStub((void *)atiny_malloc,(void *)stub_atiny_malloc,&stub_info);
    ret=flash_manager_write(&flash_info);//-4
	TEST_ASSERT_MSG((ATINY_MALLOC_FAILED == ret), "flash_manager_write(...) failed");
    cleanStub(&stub_info);

    ret=flash_manager_write(&flash_info);//0
	TEST_ASSERT_MSG((ATINY_OK == ret), "flash_manager_write(...) ok");

	flash_manager_init(example_cmd_ioctl1);
	ret=flash_manager_write(&flash_info);//-1
	TEST_ASSERT_MSG((ATINY_ARG_INVALID == ret), "flash_manager_write(...) ok");
	

}


void TestFlashmanager::setup()
{
   // printf("come into %d func, in file %s\n", ++funcno, __FILE__);
}


void TestFlashmanager::tear_down()
{
   // printf("exit %d func, in file %s\n", funcno, __FILE__);
}





