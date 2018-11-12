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
	int8_t buf2[] = "+NNMI:511,FFFE01151CC801F70000000000000000000037000007240004000400000001006400154C6974654F5356315232433531305350433030FFEE00650006649C1234567800100000001000007CB90000C8B900000B100000450100000C000B005AA4C32535BE47240A00F39D4C6FDC80EA4B090061070BFAB4DDCA7A0800539510164ED2E54B0700D3A46953FBC8952F0600CC2C10D5D4B06C200500FC2967A8C573C04A0400590045AE29594C310300478186F7000E9CCA020071226A634261C78A01003F63A42249FDCE340000FCFA759720D308FC0000080000000200D1093B005A0000000400001402FCC8D0572B91E1D53A280A24D5E5228A85F0DD1558560BA74A6280BF32405547B2DDA6AF7E40A9E3553EE12B4AF0171FF7000200091031005A0000000400001402FCC83D24501100AA50BDF6DC77E47B41001851EED98A582927298DAFC86ECE37465B8DC3B0779F000200091031005A0000000400001402FCC7AD31CD263D56692B41A0E709751292CD4EF707B371F6398D6F1015552369E8457DCEF6B51D000200091051005A0000000400001402FCC71CFEC4A5FC518165A601A28DCA7C42BBBE58DFB18B26DB8860CF8DB3000D25899EB364E758009EF2387832E7DE4C03898A1C77D4170E36FE1E030AD6B622E93BFF2506ACD4000200091094005A0000000400001402FCC68512D8B2F5265969FB564769A5B6";
	int8_t buf3[] = "+NNMI:511,FFFE01155C5601F700000121FB788623E7D021306296D7F6493D5F7D66E601612E624B9F65AA5674F036210FF1CA3995AAA084B1BF70DFD6A32B996D3E3B079EA84AB6387C718128A8694FEF9F1BBFBAABC06C3031EECEA956E8D6BE1C054E7F4F6543F2636BF395EC69C5F98DCF23076DE1673A476C0263FDA123117C6F3C42317BBADEB90002000910E8005A0000000400001402FCC5ECFB6558F373F447A62A0F1F846D037B535B926D3A442DA75C196FD591132A37F5C3A5A64DECBB6622A65357D5B03B90F3D33591617AC7A46BFCEA88C89BCA252D4221AC0C6F21827513EDED536396E4D200339487187344A4333FE46530CEF16425473FEEAD46113046C6CC4280358E63D836390918520B2507D86A72698AFF20189AD0AE97262DBB7B3967836F35C395F964C50BB48D0A01109563F11F5CF075700663D49A60E910BEBD3FD4EECCA9F7C71372E1CD72ED24C820DDD55ED9D2AEAC20FA1EC88288FD39432813B00B239EF68A978F740C9CB3D41852E602000910B4005A0000000400001402FCC54FB94C8847AD52AC669C360947F3B1D7B3579C1EFFED0DF455169AFCE2C308A3B9C0A8F264F0021CEB17FCCE082C8C00AAC8A59537AAB54DF9F1C4B30BC9A5FC19FE810A3F5CD9F95632ECFDFEB026ED9946A0B30685B760E2EC311F16C184D8FDC7DF1FD314ED07CBFEA9538D3C2CAC9271FD73ED98DDB63856";
	int8_t buf4[] = "+NNMI:511,FFFE011591FA01F700000283A9DB1894BC9A942AC021469F6EC77891F64D8249AE7FCE1FA8AF7CB5987EAAA20034B8CCF0E02E8BC79E9B8A94CA03000B1045015A0000000400102D181EF345D23350BC8CA41AFA44FF00A41C6CE2787860D91A0E7156DA0DFCDFBFFE8B513223C79AF4ACECCF9ADA2556171156DF312B41DC6891909B3E74D6877BB4C418DBD836A9A8016C26E5D02BE969CFB518BA800F7B8FD9F90B52FC4D52541AE9446E239FC86A075F445266765815B543A6C34E93259494E44601630443B7CBD3AE63EA0ACC8852E97E5F535B4D6F7A26014DC2D9D2B0717F9969AB5D7F186EA62AD2AFF0A5D43363D42E834759FB7A24FB9D8C3C463FAEE8F93A9A2C76E21B8D3480F91B585FAA176622464C69BD088329FDE2F31756F5E75912FEFAE6F0C6591A52ABDDB9E3784AA376927F39E6B171EACBB4D8949504B911B9C6C842F20E14C3E039718BF7856BF4D2690E54734E00E927FF555A99C3DCA28AEFFCFD5613553793ACCFD7F9C0578B885DA1CEB882BE86129037D2C93055CD44B90000010000000100051076005A000000040000603E7F4697178F205AAADE503FD62A1F25ABDB313D7D8A1D4DD01F1721BA426A4666733D001B13EFE24B051EE2E6D4F3C6770474ED563916802F1F37D39152D459856F5BD3ACAA0AD43D9560A75A36E1AF9A7BD579965D29564EF097A1FA0FD1DE8B9F799EB709";
	int8_t buf5[] = "+NNMI:339,FFFE0115B3D4014B0000032DA6DD176E061000000001000000010005104A005A000000040000603E7F30F3E1C2D23DDE791C0B0622CCC1DBA0F11227FA2740BAF102EB3AC4D2A455FD293850B405D79EC8748AC37C5D1D2C21D71EC61298862E59FDCDFDF1AF0C21A4000001000000010005106D005A00000004000060408F73BDE8CA559CC754A6E0B43DEAF04BBDCE10B0CAF949DE99A7D1BA030B13939AC7DAAB9592B842C0812A4FBB5563ACBD379CF8A9414B1B5B3EFA8BFA1C678D78A8576F161B3A321C6E7E7796A15BB9BEA3C46AB6BDA2CB5D5EFA58F2F247DC1F40CD200000010000000100051065005A000000040000603E811C199DA16A23B4B094A59480309784828484D293EA17F2AA726EA625549E5805CFBF5C0FA6FCCD5CF283E1078ECF17DFBBF344414D285F58D5657EC98F34475DC519FE1D3E4371E2E78FADD619787171E4130734EB4862336D0100";
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
