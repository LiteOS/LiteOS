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

#include "test_sota.h"
#include "ota/ota_api.h"

#define DEVICE_VER "V0.0"
#define FLASH_FLAG_SIZE (512)
#define VER_LEN      16

typedef struct
{
    int (*get_ver)(char* buf, uint32_t len);
    int (*set_ver)(const char* buf, uint32_t len);
    int (*sota_send)(const char* buf, int len);
    void* (*sota_malloc)(size_t size);
    void (*sota_free)(void *ptr);
    uint32_t frame_buf_len;
    uint8_t  run_mode;
    uint8_t  rsv[3];
    ota_opt_s ota_info;
} sota_opt_t;

extern "C"
{
	#include<string.h>
	#include<stdio.h>
	#include<stdlib.h>
	#include "at_frame/at_main.h"
	#include "ota/package.h"
	#include "package_head.h"
	#include "package_writer.h"
	#include<stdint.h>
	#include <unistd.h>

	#include "flag_manager.h"
	#include "upgrade_flag.h"
    extern int sota_init(sota_opt_t* flash_opt);
    extern int32_t sota_process_main(void *arg, const int8_t *buf, int32_t buflen); 
	extern void sota_timeout_handler(void);
	extern int pack_init_device(const ota_opt_s *ota_opt);
	extern int nb_send_str(const char* buf, int len);
	char *strnstr(const char *s1, const char *s2, size_t len) 
    {}
    extern void sota_timeout_handler(void);
	extern int flag_set_info(upgrade_type_e upgrade_type, uint32_t image_size);
    void dtls_int()
    {}
}

void *arg = NULL;

typedef  uint8_t   BYTE;
typedef  uint16_t  WORD;

typedef enum
{
    IDLE = 0,
    DOWNLOADING,
    DOWNLOADED,
    UPDATING,
    UPDATED,
}at_fota_state;

typedef enum
{
    APP_MODE = 0,
    BOOTLOADER_MODE,
}run_mode_e;

typedef struct ota_pcp_head_t
{
    WORD ori_id;
    BYTE ver_num;
    BYTE msg_code;
    WORD chk_code;
    WORD data_len;
}ota_pcp_head_s;

typedef struct
{
    int (*read_flash)(ota_flash_type_e type, void *buf, int32_t len, uint32_t location);
    int (*write_flash)(ota_flash_type_e type, const void *buf, int32_t len, uint32_t location);
}sota_flag_opt_s;

typedef enum
{
SOTA_OK = 0,
SOTA_DOWNLOADING = 1,
SOTA_NEEDREBOOT = 2,
SOTA_BOOTLOADER_DOWNLOADING = 3,
SOTA_MEM_FAILED = 4,
SOTA_FAILED = -1,
SOTA_TIMEOUT = -2,
}sota_ret;

typedef struct ota_ver_notify
{
    BYTE ver[16];
    WORD block_size;
    WORD block_totalnum;
    WORD ver_chk_code;
}ota_ver_notify_t;

typedef struct sota_update_info
{
    uint16_t block_size;
    uint32_t block_num;
    uint32_t block_offset;
    uint32_t block_totalnum;
    uint32_t block_tolen;
    uint32_t ver_chk_code;
    char ver[VER_LEN];
    uint8_t state;
} sota_update_info_t;

#define SOTA_DEBUG
#ifdef SOTA_DEBUG
#define SOTA_LOG(fmt, arg...)  printf("[%s:%d][I]"fmt"\n", __func__, __LINE__, ##arg)
#else
#define SOTA_LOG(fmt, arg...)
#endif

unsigned char* temp_rabuf = NULL;
 
static int funcno;



static int stub_flag_read1(flag_type_e flag_type, void *buf, int32_t len)
{
	return -1;
}

static int stub_flag_read2(flag_type_e flag_type, void *buf, int32_t len)
{
	return 0;
}

static int stub_flag_read3(flag_type_e flag_type, void *buf, int32_t len)
{
	sota_update_info_t *temp = (sota_update_info_t *)buf;
	temp->state = DOWNLOADING;
	temp->block_size = 1024;
	memcpy(buf, temp, len);
	return 0;
}

static int stub_flag_write(flag_type_e flag_type, const void *buf, int32_t len)
{
	return 0;
}

static int stub_flag_upgrade_get_result(upgrade_state_e *state)
{
	*state = OTA_SUCCEED;
	return 0;
}

static int stub_nb_send_str(const char* buf, int len)
{
	return 0;
}

int read_ver(char* buf, uint32_t len)
{
    memcpy(buf,DEVICE_VER,strlen(DEVICE_VER));
    return 0;
}

int set_ver(const char* buf, uint32_t len)
{
    return 0;
}

static int stub_pack_head_parse(pack_head_s *head, uint32_t offset, const uint8_t *buff,uint16_t len, uint16_t *used_len)
{
	return 0;
}

static int stub_pack_wr_write(pack_writer_s *writer, uint32_t offset, const uint8_t *buff, uint16_t len)
{
	return 0;
}

static int stub_pack_head_check(const pack_head_s *head, uint32_t len) 
{
	return 0;
}

static void *stub_at_malloc(size_t size)
{
	return NULL;
}

void temp_free(void *ptr)
{
	free(ptr);
}

static void *stub_at_malloc1(size_t size)
{
	temp_rabuf = (unsigned char *)malloc(size);
	return temp_rabuf;
}

static int stub_pack_wr_write_end(pack_writer_s *writer)
{
	return -1;
}

TestSota::TestSota()
{
	printf("in TestSita consfunc\n");
    TEST_ADD(TestSota::test_sota_init);
    TEST_ADD(TestSota::test_sota_process_main);
}

TestSota::~TestSota()
{  
}


void TestSota::test_sota_init()
{
	printf("start test_sota_init,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,\n");
    int ret = 0;
	sota_opt_t flash_op =
    {
    	.get_ver = read_ver,
    	.set_ver = set_ver,
    	.sota_send = nb_send_str,
		.sota_malloc = at_malloc,
    	.sota_free = at_free,
    };
	flash_op.ota_info.key.rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
	flash_op.ota_info.key.rsa_E = "10001";
	flash_op.frame_buf_len = 1224;

	flash_op.run_mode = APP_MODE;
	ret = sota_init(NULL);
    TEST_ASSERT_MSG((SOTA_FAILED == ret), "sota_init(...) failed");

	stubInfo si_flag_write;
	setStub((void *)flag_write, (void *)stub_flag_write, &si_flag_write);
	stubInfo si_flag_read;
	setStub((void *)flag_read, (void *)stub_flag_read2, &si_flag_read);
	stubInfo si_at_malloc;
	setStub((void *)at_malloc, (void *)stub_at_malloc, &si_at_malloc);
	ret = sota_init(&flash_op);
	cleanStub(&si_flag_read);
	cleanStub(&si_at_malloc);
	TEST_ASSERT_MSG((SOTA_FAILED == ret), "sota_init(...) failed");
	
	stubInfo si_at_malloc1;
	setStub((void *)at_malloc, (void *)stub_at_malloc1, &si_at_malloc1);
	stubInfo si_flag_read1;
	setStub((void *)flag_read, (void *)stub_flag_read1, &si_flag_read1);
	ret = sota_init(&flash_op);
	cleanStub(&si_flag_read1);
	TEST_ASSERT_MSG((SOTA_FAILED == ret), "sota_init(...) failed");
	temp_free(temp_rabuf);
	
	stubInfo si_flag_read3;
	setStub((void *)flag_read, (void *)stub_flag_read3, &si_flag_read3);
	flash_op.run_mode = BOOTLOADER_MODE;
    ret = sota_init(&flash_op);
	cleanStub(&si_flag_read3);
	TEST_ASSERT_MSG((SOTA_BOOTLOADER_DOWNLOADING == ret), "sota_init(...) downloading");
	sota_timeout_handler();
	temp_free(temp_rabuf);
	
	stubInfo si_flag_read2;
	setStub((void *)flag_read, (void *)stub_flag_read2, &si_flag_read2);
	stubInfo si_flag_upgrade_get_result;
	setStub((void *)flag_upgrade_get_result, (void *)stub_flag_upgrade_get_result, &si_flag_upgrade_get_result);
	stubInfo si_sota_send;
	setStub((void *)nb_send_str, (void *)stub_nb_send_str, &si_sota_send);
    flash_op.run_mode = APP_MODE;
	ret = sota_init(&flash_op);
	
	
	cleanStub(&si_sota_send);
	cleanStub(&si_flag_upgrade_get_result);
	cleanStub(&si_flag_read2);
	cleanStub(&si_at_malloc1);
	cleanStub(&si_flag_write);

	TEST_ASSERT_MSG((SOTA_OK == ret), "sota_init(...) success");
	
	
}

void TestSota::test_sota_process_main()
{
    int ret = 0;
	int8_t* buf = NULL;
	int32_t buflen = 0;
	ret = sota_process_main(arg,buf,buflen);
    TEST_ASSERT_MSG((SOTA_FAILED == ret), "sota_process_main() is failed");
	

	int8_t buf1[] = "NNMI,8,FFFE011620380000";
	int32_t buflen1 = strlen((const char *)buf1);
	ret = sota_process_main(arg,buf1,buflen1);
	TEST_ASSERT_MSG((SOTA_FAILED == ret), "sota_process_main() is failed");
	
	
	int8_t buf2[] = "NNMI:12:11";
	int32_t buflen2 = strlen((const char *)buf2);
	ret = sota_process_main(arg,buf2,buflen2);
	TEST_ASSERT_MSG((SOTA_FAILED == ret), "sota_process_main() is failed");
	
	
	int8_t buf3[] = "+NNMI:8,FFFE0113409a0000";
	int32_t buflen3 = strlen((const char *)buf3);
	ret = sota_process_main(arg,buf3,buflen3);
	TEST_ASSERT_MSG((SOTA_FAILED == ret), "sota_process_main() crc_check is failed");
	
	
	int8_t buf4[] = "+NNMI:9,FFFE011316ec000100";
	int32_t buflen4 = strlen((const char*)buf4);
	stubInfo si_sota_send;
	setStub((void *)nb_send_str, (void *)stub_nb_send_str, &si_sota_send);
	stubInfo si_flag_write;
	setStub((void *)flag_write, (void *)stub_flag_write, &si_flag_write);
	ret = sota_process_main(arg,buf4,buflen4);
	TEST_ASSERT_MSG((SOTA_OK == ret), "MSG_GET_VER is succeed");
	
	
	int8_t buf5[] = "+NNMI:8,FFFE01134c9a0000";
	int8_t buf6[] = "+NNMI:9,FFFE0116850e000100";
	int8_t buf7[] = "+NNMI:30,FFFE0114913F001656312E3000000000000000000000000001F400043132";
	int8_t buf8[] = "+NNMI:511,FFFE01156A5D01F7000000000000000000013B000007D00004000400000001006400154C6974654F5356315232433531305350433030FFEE00650006649C123456780003010045581107531F1F7C7FDD6EADE2E1D925F3748D7D9E8D463ABD4E2476EB376E949F8E3ACEA3A44EA224A3CFE00079830CD1644196EB7442CD30F27363F1F70D8AF1F116BFEB3CE234D10222CB292376BE272C972DFAD7CD434DDF81E516F1B688C189BB25484FC62AA443EFE5686F240F1F47F925402ACBA7293CA7C12FD49FC400D15DF3FA2FF9F1DFAFF4D9B7DBDEDC5375B1272D05BA7958BE5F27B885E5BB78891E6FD7CD448627685F68CC0FC0355596E85BFC96213217AF36D3A5AA67730FBE8EEEDE1A4D16D5AA21A5FB1C75037593A07D84F5B67A1CB5BAE3B97EA3F3E82368F0869D6660F91E993CED1BAC2FB34105E05A1CEDE8BCE97E79C1F073A2001000000010000024B900004CB900000B100000250100000C000B00DC5087DDF9C086030A00FDE2C8C8F5435B80090086B662BCC420EF1B0800B1B0AC647715BADA0700A9EACECB210F8A200600744360CCC61D63140500E0A04998F375F8E10400359A8EAA9F4FF39D0300E5A42356CF347C6802000E227EE99A59AE1F01002E0C3AF91C21D5630000A224AE100325DB080000080000000200550939005A0000000400000AD56D7795A1907C3AFA69B8E06854264844D8C5";
	int8_t buf9[] = "+NNMI:511,FFFE0115121301F70000019855E6A6581E6E0BDC5FC5542AB6EE4210F56EC49863D052123F71844520020009102F005A0000000400000AD56D7704BE59208E3C76A755389A76965026F651964094A158EF8BEF52D89B7E93D5A7C61C18860200091029005A0000000400000AD56D7670925920793A1CCA40387F47F1063A43B469705AA3CBCEAE34C8C73EC3B20200091054005A0000000400000AD56D75DC259BA01240BAA739328C826514E2DD59E83FA7BD53D84C53010B5DF5CBB540999D8881D420BC17CDC4BD050FFB445AE2B09577E9AEC497C8E07B88B03D36225C66B3BAEF3AB474000200091088005A0000000400000AD56D754011BF72CB8AAFCABE3B2292D623502FB7239A42A367B631BCE323D308317BCD12170064E4C9BECA6B5033394B5F2BD5DA00D8D0132693BF84C80C9C7E2B8B79DB03EF28979C09E2C650843C53244EDEE9A2745671B7BC21E69889E9B0DAB6632D4D8B5EB2FE0EA75D62EA109D77A41D2D34829AF79EB25B34EC98870002000910E1005A0000000400000AD56D74A4019B9FE346291E1DBDDFF548C5521E8B25B12D8B86F6C3552D0A5219231A7A7026B33E35D5B6E5AC4FD55B453AAB52A27BA7CBFDC280AD99BCAEDD151B71B96D6815C8F0847C6B1AEE16BE9A8E674DECC577667859B19BD8FE6A3C3D6507AE23306FA05AEA1C6E68EFE911FE868D2E81E43032CEDA1330AD";
	int8_t buf10[] = "+NNMI:511,FFFE0115A7DB01F70000029A413195CEB68D17380ABDEEC6D5573F31BD3B3135D699F4355ADC2BF839B31033575A474F7B2D4B33D00E2D81CD63FAE8B776940E16EE81E15C5C6B8B3942AF476C17BC583657FCF4056CA5D67E4B813F63D73C030B2EA591D498537602000910AD005A0000000400000AD56D7407EDBF729C91E7CD14210505D79887DCD144768F94D0298B2AF82036D7493EFD417EF2279252D669F939860F4289315A54A16F22AEA80E75D12B0305D67248B4A6DC6393C18C84FE64102C193B3B0B61E8C230648C3C4655798719EA903FC6C7363B403C5F7D369F9335DB35FFC827BC754E22FF1B26C379F2C60C11549B974ABABC120F71476BD6B057DD695CBBC221AFB16E1FDDB27D220CDBFE7A70E63F709A4103000B1025015A00000004001035D01EF34514B375020AD3999B3E976C43ED0995E4ED55A2835721E6443A3E6AF9BE8FAD523DF3EE64C812F20F0D8393FF49D587EBA732DD8A3E16623E53E8049060DFC97807DB61C09EDE0A05A02A14239C39BB975D6413B02F8403190DBF6C7B38BC3C2CBD9396089A40B22EE14FFCF8A5120DE7BDD5BE295A2B74DA843466B97DCA5204FCF55A35D3434533D9F0245606F3EA785848072A3FC46BC100182B50285947C7C98D62BA84B34A590A4DB89973869D00D7001A18DDC7CAA1564D640A40A27A32CE77FC6A57340515FEAC08FC398F5417574D";
	int8_t buf11[] = "+NNMI:511,FFFE011599FE01F700000324FDCA80C0A1D4EBAA802976FADB6DCB268A5ABAFC94E3DB6F1DFAA862F664B7B890F8D85750E55561E165EB814265B7D9C83093D08CA037B43D86280B02CC4A36F655A90638000000010000000100051071005A000000040000603E7F4697178ECB53DB5398D41DAFE222A5B2D006C5DD4519926D6462A3D90ADF810BAB82A950DF22BF8DE5DDC001FC82C7444E8D6DC7945CC0981ECC35B879ED2E18C6A1752DF92E39CBA02D9A444B7ECDDF3B311422110460D405089F5BDAFF6BC0F76EB72F5490540000010000000100051049005A000000040000603E7F30F3E1C2A46A7C0035AC2381D407E02A8FD12B7696FFE62A8AD09E0BA048EE8FFA81BD5A58EDB1B021C256EC007AC4B7F67665089228BCF5154B507CEDC8FB0000010000000100051064005A00000004000060408F73BDE8C59FF3E27E9163B018DAA6971BF24071E828C40C0E885F39670480378525DEB96BE4A5EA528377E28D08098695D01A5D39756CB69107FA99EAE2BDF0118C17785CAB1746829728C42A46125F7FAA3C42867B2C6A66DB93000001000000010005105F005A000000040000603E811BEAE246E095730E9A37C4E8A0F016964B0DFD644CF5C6F18B0CBC9D655F6E68DFBA5BC47DC5B417E84A4DBB0DAB145BDAD83707764A127655085B66830C8849BC549F1CB97247D68548229A19B3DEDED8474D6E00";
	int32_t buflen5 = strlen((const char*)buf5);
	int32_t buflen6 = strlen((const char*)buf6);
	int32_t buflen7 = strlen((const char*)buf7);
	int32_t buflen8 = strlen((const char*)buf8);
	int32_t buflen9 = strlen((const char*)buf9);
	int32_t buflen10 = strlen((const char*)buf10);
	int32_t buflen11 = strlen((const char*)buf11);
	sota_process_main(arg,buf5,buflen5);
	ret = sota_process_main(arg,buf5,buflen5);
	
	TEST_ASSERT_MSG((SOTA_OK == ret), "sota_process_main() is succeed");
	sota_process_main(arg,buf6,buflen6);
	int8_t buf12[] = "+NNMI:9,FFFE0116850e000100";
	int8_t buf13[] = "+NNMI:8,FFFE0117CF900000";
	int8_t buf14[] = "+NNMI:30,FFFE0114A15C001656312E3000000000000000000000000001F400043131";
	int32_t buflen12 = strlen((const char*)buf12);
	int32_t buflen13 = strlen((const char*)buf13);
	int32_t buflen14 = strlen((const char*)buf14);

	
	stubInfo si_pack_head_parse;
	setStub((void *)pack_head_parse, (void *)stub_pack_head_parse, &si_pack_head_parse);
	stubInfo si_pack_wr_write;
	setStub((void *)pack_wr_write, (void *)stub_pack_wr_write, &si_pack_wr_write);
	stubInfo si_pack_head_check;
	setStub((void *)pack_head_check, (void *)stub_pack_head_check, &si_pack_head_check);
	
	sota_process_main(arg,buf7,buflen7);
	sota_process_main(arg,buf8,buflen8);

	sota_process_main(arg,buf9,buflen9);
	sota_process_main(arg,buf10,buflen10);
	ret = sota_process_main(arg,buf11,buflen11);
	TEST_ASSERT_MSG((ret == SOTA_OK), "sota_process_main() is succeed");
	
	ret = sota_process_main(arg,buf13,buflen13);
	TEST_ASSERT_MSG((ret == SOTA_NEEDREBOOT), "sota_process_main() is succeed");
	sota_process_main(arg,buf12,buflen12);
	
	sota_timeout_handler();
	sota_process_main(arg,buf5,buflen5);
	
	sota_process_main(arg,buf7,buflen7);
	ret = sota_process_main(arg,buf9,buflen9);
	TEST_ASSERT_MSG((ret == SOTA_FAILED), "sota_process_main() is failed");
	
	stubInfo si_pack_wr_write_end;
	setStub((void *)pack_wr_write_end, (void *)stub_pack_wr_write_end, &si_pack_wr_write_end);
	sota_process_main(arg,buf14,buflen14);
	sota_process_main(arg,buf8,buflen8);
	sota_process_main(arg,buf9,buflen9);
	sota_process_main(arg,buf10,buflen10);
	ret = sota_process_main(arg,buf11,buflen11);
    TEST_ASSERT_MSG((ret == SOTA_FAILED), "sota_process_main() is failed");

	cleanStub(&si_pack_wr_write_end);
	cleanStub(&si_pack_head_check);
	cleanStub(&si_pack_wr_write);
	cleanStub(&si_pack_head_parse);
	
	sota_timeout_handler();
	sota_process_main(arg,buf7,buflen7);
	ret = sota_process_main(arg,buf8,buflen8);
	TEST_ASSERT_MSG((ret == SOTA_FAILED), "sota_process_main() is failed");
	
	cleanStub(&si_flag_write);
	cleanStub(&si_sota_send);
	temp_free(temp_rabuf);
	
}



void TestSota::setup()
{
    printf("come into test funcno %d,%s\n", ++funcno,__FILE__);
}

void TestSota::tear_down()
{
    printf("exit from funcno %d,%s\n", funcno,__FILE__);
}


