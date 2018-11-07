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
 #ifndef __SOTA_TEST_H_
 #define __SOTA_TEST_H_
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "../cmockery/src/google/cmockery.h"
#include "at_device/bc95.h"
#include "ota_port.h"
#include "sota/sota.h"
#include "../../components/ota/sota/sota_hal.h"
#include "ota/package.h"
#include "../../include/at_frame/at_main.h"

#define VER_LEN  16
#define DEVICE_VER "V0.0"


static sota_opt_t * g_flash_op_t;



#define htons_ota(x) ((((x) & 0x00ff) << 8) | (((x) & 0xff00) >> 8))
#define PCP_HEAD 0xFFFE
#define BLOCK_HEAD 3

extern int nb_send_str(const char* buf, int len);

void * arg = NULL;

extern int read_ver(char* buf, uint32_t len);
extern int set_ver(const char* buf, uint32_t len);

int sota_test_main(void);

int sota_init_normal()
{
	sota_opt_t flash_op =
	{
	    .get_ver = read_ver,
	    .set_ver = set_ver,
	    .sota_send = nb_send_str,
	};
	    hal_get_ota_opt(&flash_op.ota_info);
	    flash_op.ota_info.key.rsa_N = "C94BECB7BCBFF459B9A71F12C3CC0603B11F0D3A366A226FD3E73D453F96EFBBCD4DFED6D9F77FD78C3AB1805E1BD3858131ACB5303F61AF524F43971B4D429CB847905E68935C1748D0096C1A09DD539CE74857F9FDF0B0EA61574C5D76BD9A67681AC6A9DB1BB22F17120B1DBF3E32633DCE34F5446F52DD7335671AC3A1F21DC557FA4CE9A4E0E3E99FED33A0BAA1C6F6EE53EDD742284D6582B51E4BF019787B8C33C2F2A095BEED11D6FE68611BD00825AF97DB985C62C3AE0DC69BD7D0118E6D620B52AFD514AD5BFA8BAB998332213D7DBF5C98DC86CB8D4F98A416802B892B8D6BEE5D55B7E688334B281E4BEDDB11BD7B374355C5919BA5A9A1C91F";
	    flash_op.ota_info.key.rsa_E = "10001";
	    flash_op.frame_buf_len = 1224;
	    hal_init_ota();
	    int ret;
	    ret = sota_init(&flash_op);
	    return ret;
}

#endif
