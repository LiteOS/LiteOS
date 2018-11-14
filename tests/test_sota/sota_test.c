/*
 * Copyright 2008 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 #include "sota_test.h"

 
static void It_Los_sota_init_001(void **state)       //flash_op = NULL
{
    int ret;
    g_flash_op_t = NULL;
    hal_init_ota();
    ret = sota_init(g_flash_op_t);

    assert_int_equal(ret, SOTA_FAILED);
    
}

static void It_Los_sota_init_002(void **state)     //data_len = 0
{
    int ret;
	sota_opt_t flash_op =
    {
    	.get_ver = read_ver,
    	.set_ver = set_ver,
    	.sota_send = nb_send_str,
    	.sota_malloc = (t_sota_malloc)at_malloc,
    	.sota_free = at_free,
    };
    hal_get_ota_opt(&flash_op.ota_info);
    flash_op.ota_info.key.rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
    flash_op.ota_info.key.rsa_E = "10002";
    flash_op.frame_buf_len = 0;
    flash_op.run_mode = APP_MODE;
    hal_init_ota();


    ret = sota_init(&flash_op);

    assert_int_equal(ret, SOTA_FAILED);
    
}

static void It_Los_sota_init_003(void **state)       //normal
{
	int ret;
	sota_opt_t flash_op =
    {
    	.get_ver = read_ver,
    	.set_ver = set_ver,
    	.sota_send = nb_send_str,
		.sota_malloc = (t_sota_malloc)at_malloc,
    	.sota_free = at_free,
    };
	hal_get_ota_opt(&flash_op.ota_info);
	flash_op.ota_info.key.rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
	flash_op.ota_info.key.rsa_E = "10001";
	flash_op.frame_buf_len = 1224;
	flash_op.run_mode = APP_MODE;
	hal_init_ota();
	ret = sota_init(&flash_op);

	assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_001(void **state)  // buf=NULL
{
    int8_t* buf = NULL;
	int32_t buflen = 0;

    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);


    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_002(void **state)  //prefix is wrong
{
    int8_t buf[] = "NNMM";
	int32_t buflen = strlen((const char *)buf);

    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_003(void **state)  //buflen = 0
{
    int8_t buf[] = "+NNMI:8,FFFE01134c9a0000";
	int32_t buflen = 0;

    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_004(void **state)   //  msg_code < 19
{
    int8_t buf[] = "+NNMI:8,FFFE0112b5010000";
	int32_t buflen = strlen((const char *)buf);

    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_005(void **state)   //  msg_code > 24
{
    int8_t buf[] = "+NNMI:8,FFFE0119d34e0000";
	int32_t buflen = strlen((const char *)buf);

    int32_t ret;
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_006(void **state)  //ver_num != 1
{
    int8_t buf[] = "+NNMI:8,FFFE02134c9a0000";
	int32_t buflen = strlen((const char *)buf);

    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_007(void **state)  //ver_num =null
{
    int8_t buf[] = "+NNMI:8,FFFE134c9a0000";
	int32_t buflen = strlen((const char *)buf);

    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_008(void **state)  //crc_code = null
{
    int8_t buf[] = "+NNMI:8,FFFE0113";
	int32_t buflen = strlen((const char *)buf);

    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_009(void **state)  //data_length = null
{
    int8_t buf[] = "+NNMI:8,FFFE02134c9a";
	int32_t buflen = strlen((const char *)buf);

    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_FAILED);
}

static void It_Los_ota_process_main_010(void **state)  //MSG_GET_VER
{
    int8_t buf[] = "+NNMI:8,FFFE01134c9a0000";
	int32_t buflen = strlen((const char *)buf);
    int32_t ret;
    
    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}


static void It_Los_ota_process_main_011(void **state)  //MSG_GET_VER && data
{
    int8_t buf[] = "+NNMI:9,FFFE011316ec000100";
	int32_t buflen = strlen((const char *)buf);
    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_012(void **state)   //IDLE
{
    int8_t buf[] = "+NNMI:26,FFFE0115765A001256312E300000000000000000000000000002";
    int32_t ret;
    int32_t buflen = strlen((const char *)buf);

	ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_013(void **state)   //IDLE
{
    int8_t buf[] = "+NNMI:9,FFFE0116850e000100";
    int32_t ret;
    int32_t buflen = strlen((const char *)buf);

	ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_014(void **state)   //IDLE
{
    int8_t buf[] = "+NNMI:9,FFFE0117b725000100";
    int32_t ret;
    int32_t buflen = strlen((const char *)buf);

	ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_015(void **state)   //IDLE
{
    int8_t buf[] = "+NNMI:9,FFFE0118afa1000100";
    int32_t ret;
    int32_t buflen = strlen((const char *)buf);

	ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_016(void **state)   //MSG_NOTIFY_NEW_VER
{
	int8_t buf1[] = "+NNMI:30,FFFE0114913F001656312E3000000000000000000000000001F400043132";
	int8_t buf2[] = "+NNMI:511,FFFE01156A5D01F7000000000000000000013B000007D00004000400000001006400154C6974654F5356315232433531305350433030FFEE00650006649C123456780003010045581107531F1F7C7FDD6EADE2E1D925F3748D7D9E8D463ABD4E2476EB376E949F8E3ACEA3A44EA224A3CFE00079830CD1644196EB7442CD30F27363F1F70D8AF1F116BFEB3CE234D10222CB292376BE272C972DFAD7CD434DDF81E516F1B688C189BB25484FC62AA443EFE5686F240F1F47F925402ACBA7293CA7C12FD49FC400D15DF3FA2FF9F1DFAFF4D9B7DBDEDC5375B1272D05BA7958BE5F27B885E5BB78891E6FD7CD448627685F68CC0FC0355596E85BFC96213217AF36D3A5AA67730FBE8EEEDE1A4D16D5AA21A5FB1C75037593A07D84F5B67A1CB5BAE3B97EA3F3E82368F0869D6660F91E993CED1BAC2FB34105E05A1CEDE8BCE97E79C1F073A2001000000010000024B900004CB900000B100000250100000C000B00DC5087DDF9C086030A00FDE2C8C8F5435B80090086B662BCC420EF1B0800B1B0AC647715BADA0700A9EACECB210F8A200600744360CCC61D63140500E0A04998F375F8E10400359A8EAA9F4FF39D0300E5A42356CF347C6802000E227EE99A59AE1F01002E0C3AF91C21D5630000A224AE100325DB080000080000000200550939005A0000000400000AD56D7795A1907C3AFA69B8E06854264844D8C5";
	int8_t buf3[] = "+NNMI:511,FFFE0115121301F70000019855E6A6581E6E0BDC5FC5542AB6EE4210F56EC49863D052123F71844520020009102F005A0000000400000AD56D7704BE59208E3C76A755389A76965026F651964094A158EF8BEF52D89B7E93D5A7C61C18860200091029005A0000000400000AD56D7670925920793A1CCA40387F47F1063A43B469705AA3CBCEAE34C8C73EC3B20200091054005A0000000400000AD56D75DC259BA01240BAA739328C826514E2DD59E83FA7BD53D84C53010B5DF5CBB540999D8881D420BC17CDC4BD050FFB445AE2B09577E9AEC497C8E07B88B03D36225C66B3BAEF3AB474000200091088005A0000000400000AD56D754011BF72CB8AAFCABE3B2292D623502FB7239A42A367B631BCE323D308317BCD12170064E4C9BECA6B5033394B5F2BD5DA00D8D0132693BF84C80C9C7E2B8B79DB03EF28979C09E2C650843C53244EDEE9A2745671B7BC21E69889E9B0DAB6632D4D8B5EB2FE0EA75D62EA109D77A41D2D34829AF79EB25B34EC98870002000910E1005A0000000400000AD56D74A4019B9FE346291E1DBDDFF548C5521E8B25B12D8B86F6C3552D0A5219231A7A7026B33E35D5B6E5AC4FD55B453AAB52A27BA7CBFDC280AD99BCAEDD151B71B96D6815C8F0847C6B1AEE16BE9A8E674DECC577667859B19BD8FE6A3C3D6507AE23306FA05AEA1C6E68EFE911FE868D2E81E43032CEDA1330AD";
	int8_t buf4[] = "+NNMI:511,FFFE0115A7DB01F70000029A413195CEB68D17380ABDEEC6D5573F31BD3B3135D699F4355ADC2BF839B31033575A474F7B2D4B33D00E2D81CD63FAE8B776940E16EE81E15C5C6B8B3942AF476C17BC583657FCF4056CA5D67E4B813F63D73C030B2EA591D498537602000910AD005A0000000400000AD56D7407EDBF729C91E7CD14210505D79887DCD144768F94D0298B2AF82036D7493EFD417EF2279252D669F939860F4289315A54A16F22AEA80E75D12B0305D67248B4A6DC6393C18C84FE64102C193B3B0B61E8C230648C3C4655798719EA903FC6C7363B403C5F7D369F9335DB35FFC827BC754E22FF1B26C379F2C60C11549B974ABABC120F71476BD6B057DD695CBBC221AFB16E1FDDB27D220CDBFE7A70E63F709A4103000B1025015A00000004001035D01EF34514B375020AD3999B3E976C43ED0995E4ED55A2835721E6443A3E6AF9BE8FAD523DF3EE64C812F20F0D8393FF49D587EBA732DD8A3E16623E53E8049060DFC97807DB61C09EDE0A05A02A14239C39BB975D6413B02F8403190DBF6C7B38BC3C2CBD9396089A40B22EE14FFCF8A5120DE7BDD5BE295A2B74DA843466B97DCA5204FCF55A35D3434533D9F0245606F3EA785848072A3FC46BC100182B50285947C7C98D62BA84B34A590A4DB89973869D00D7001A18DDC7CAA1564D640A40A27A32CE77FC6A57340515FEAC08FC398F5417574D";
	int8_t buf5[] = "+NNMI:511,FFFE011599FE01F700000324FDCA80C0A1D4EBAA802976FADB6DCB268A5ABAFC94E3DB6F1DFAA862F664B7B890F8D85750E55561E165EB814265B7D9C83093D08CA037B43D86280B02CC4A36F655A90638000000010000000100051071005A000000040000603E7F4697178ECB53DB5398D41DAFE222A5B2D006C5DD4519926D6462A3D90ADF810BAB82A950DF22BF8DE5DDC001FC82C7444E8D6DC7945CC0981ECC35B879ED2E18C6A1752DF92E39CBA02D9A444B7ECDDF3B311422110460D405089F5BDAFF6BC0F76EB72F5490540000010000000100051049005A000000040000603E7F30F3E1C2A46A7C0035AC2381D407E02A8FD12B7696FFE62A8AD09E0BA048EE8FFA81BD5A58EDB1B021C256EC007AC4B7F67665089228BCF5154B507CEDC8FB0000010000000100051064005A00000004000060408F73BDE8C59FF3E27E9163B018DAA6971BF24071E828C40C0E885F39670480378525DEB96BE4A5EA528377E28D08098695D01A5D39756CB69107FA99EAE2BDF0118C17785CAB1746829728C42A46125F7FAA3C42867B2C6A66DB93000001000000010005105F005A000000040000603E811BEAE246E095730E9A37C4E8A0F016964B0DFD644CF5C6F18B0CBC9D655F6E68DFBA5BC47DC5B417E84A4DBB0DAB145BDAD83707764A127655085B66830C8849BC549F1CB97247D68548229A19B3DEDED8474D6E00";
	int8_t buf6[] = "+NNMI:9,FFFE0116850E000100";

    int32_t ret;
    int32_t buflen1 = strlen((const char *)buf1);
    int32_t buflen2 = strlen((const char *)buf2);
    int32_t buflen3 = strlen((const char *)buf3);
    int32_t buflen4 = strlen((const char *)buf4);
    int32_t buflen5 = strlen((const char *)buf5);
    int32_t buflen6 = strlen((const char *)buf6);
    sota_process_main(arg,buf1,buflen1);
    sota_process_main(arg,buf2,buflen2);
    sota_process_main(arg,buf3,buflen3);
    sota_process_main(arg,buf4,buflen4);
    sota_process_main(arg,buf5,buflen5);
	ret = sota_process_main(arg,buf6,buflen6);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_017(void **state)   //downloading
{

    int8_t buf[] = "+NNMI:8,FFFE01134C9A0000";

    int32_t ret;

    int32_t buflen = strlen((const char *)buf);

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_018(void **state)   //downloading
{
	int8_t buf[] = "+NNMI:8,FFFE0114e51f0000";

    int32_t ret;
    int32_t buflen = strlen((const char *)buf);;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_019(void **state)   //downloading
{
	int8_t buf[] = "+NNMI:8,FFFE01151c840000";

    int32_t ret;
    int32_t buflen = strlen((const char *)buf);

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_020(void **state)   //downloading
{
	int8_t buf[] = "+NNMI:9,FFFE0116850e000100";

    int32_t ret;
    int32_t buflen = strlen((const char *)buf);

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_OK);
}

static void It_Los_ota_process_main_021(void **state)   //downloaded
{
	int8_t buf[] = "+NNMI:8,FFFE0117CF900000";
    int32_t buflen = strlen((const char *)buf);

    int32_t ret;

    ret = sota_process_main(arg,buf,buflen);

    assert_int_equal(ret, SOTA_NEEDREBOOT);
}



int sota_test_main(void)
{
    const UnitTest tests[] =
    {
        unit_test(It_Los_sota_init_001),
        unit_test(It_Los_sota_init_002),
        unit_test(It_Los_sota_init_003),
        unit_test(It_Los_ota_process_main_001),
        unit_test(It_Los_ota_process_main_002),
        unit_test(It_Los_ota_process_main_003),
        unit_test(It_Los_ota_process_main_004),
        unit_test(It_Los_ota_process_main_005),
		unit_test(It_Los_ota_process_main_006),
        unit_test(It_Los_ota_process_main_007),
        unit_test(It_Los_ota_process_main_008),
        unit_test(It_Los_ota_process_main_009),
        unit_test(It_Los_ota_process_main_010),
        unit_test(It_Los_ota_process_main_011),
		unit_test(It_Los_ota_process_main_012),
		unit_test(It_Los_ota_process_main_013),
		unit_test(It_Los_ota_process_main_014),
		unit_test(It_Los_ota_process_main_015),
		unit_test(It_Los_ota_process_main_016),
		unit_test(It_Los_ota_process_main_017),
		unit_test(It_Los_ota_process_main_018),
		unit_test(It_Los_ota_process_main_019),
		unit_test(It_Los_ota_process_main_020),
		unit_test(It_Los_ota_process_main_021),
    };

    printf("Huawei LiteOS Sota update Test\n");

    run_tests(tests);

    return 0;
}
