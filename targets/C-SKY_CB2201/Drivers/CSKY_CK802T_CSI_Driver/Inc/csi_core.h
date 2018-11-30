/*
 * Copyright (C) 2017 C-SKY Microsystems Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************
 * @file     csi_core.h
 * @brief    CSI Core Layer Header File
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/

#ifndef _CORE_H_
#define _CORE_H_

#include <stdint.h>

#if defined(__CK801__)
#include <core_ck801.h>
#elif defined(__CK802__)
#include <core_ck802.h>
#elif defined(__CK803__) || defined(__CK803S__)
#include <core_ck803.h>
#elif defined(__CK610__)
#include <core_ck610.h>
#elif defined(__CK810__)
#include <core_ck810.h>
#elif defined(__CK807__)
#include <core_ck807.h>
#endif

#include <csi_gcc.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _CORE_H_ */
