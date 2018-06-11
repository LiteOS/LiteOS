/*******************************************************************************
 *
 * Copyright (c) 2017 China Mobile and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bai Jie & Long Rong, China Mobile - initial API and implementation
 *
 *******************************************************************************/

/*
 Copyright (c) 2017 Chinamobile

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.


 Bai Jie <baijie@chinamobile.com>
 Long Rong <longrong@chinamobile.com>

*/
/************************************************************************/
/* nb-iot middle software of china mobile api                           */
/************************************************************************/
#ifndef _NB_DEBUG_H_
#define _NB_DEBUG_H_


#	include <stdio.h>
#	include <stdarg.h>

#include "cis_if_sys.h"
#include "cis_api.h"
#include "cis_def.h"


#define LOG_EXOUTPUT_TIME_MSK    (0x01)
#define LOG_EXOUTPUT_FILE_MSK    (0x02)
#define LOG_EXOUTPUT_LINE_MSK    (0x04)




#define LOG_LEVEL_ERROR		(0x00)
#define LOG_LEVEL_WARNING	(0x01)
#define LOG_LEVEL_INFO		(0x02)
#define	LOG_LEVEL_DEBUG		(0x03)


#define	LOG_TEXT_SIZE_MIN					(64)
#define	LOG_TEXT_SIZE_MAX					(2048)
#define LOG_PREFIX_SIZE_MAX				    (32)
//////////////////////////////////////////////////////////////////////////
#define LOG_DUMP_ENABLED                    1
#define LOG_OUTPUT_EXTINFO					0

//////////////////////////////////////////////////////////////////////////
#ifndef LOG_TAG
#define	LOG_TAG				"cis"
#endif

//////////////////////////////////////////////////////////////////////////
#define STRINGIFY(x)  #x
#define STRCONCAT(x, y)  #x#y

#define AJOIN(X, Y) A_DO_JOIN(X, Y)
#define A_DO_JOIN(X, Y) A_DO_JOIN2(X, Y)
#define A_DO_JOIN2(X, Y) X##Y

extern int utils_strlen(const char *str);
extern int utils_vsnprintf(char *buf, size_t buf_size, const char *fmt, va_list args);
extern int utils_snprintf(char* buffer, size_t size, const char* format, ...);

extern bool     gLogEnabled;
extern uint8_t  gLogLevel;
extern uint8_t  gLogExoutput;
extern uint16_t gLogBufferLength;

static inline void logOutput(uint32_t level, const char *tag, const char* format, ...)
{
	va_list va;
	int len, len2,space;
	char* pBuffer;

    if(!gLogEnabled ||
        level > gLogLevel)
        return;

    if(tag == NULL)
		tag = "TAG";

    pBuffer = (char*)cissys_malloc(gLogBufferLength);
    /*space "\n\0"*/
    space = gLogBufferLength - 2;
	len = utils_snprintf(pBuffer, LOG_PREFIX_SIZE_MAX, "#%s,%d:", tag,level);
	utils_snprintf( pBuffer, LOG_PREFIX_SIZE_MAX, "#%s,%d:", tag,level);
    space -= len;
	va_start(va, format);
    len2 = utils_vsnprintf( pBuffer+len, space ,format, va);
	va_end(va);
	len += (len2>0?len2:space);

	if(len < gLogBufferLength - 2){
		pBuffer[len] = '\n';
		pBuffer[len+1] = '\0';
	}else{
		pBuffer[gLogBufferLength - 5] = '.';
		pBuffer[gLogBufferLength - 4] = '.';
		pBuffer[gLogBufferLength - 3] = '?';
		pBuffer[gLogBufferLength - 2] = '\n';
		pBuffer[gLogBufferLength - 1] = '\0';
	}

	cissys_logwrite((uint8_t*)pBuffer,utils_strlen(pBuffer));
    cissys_free(pBuffer);
};



static inline void logData(const char* format, ...)
{
	va_list va;
	int len;
	char* pBuffer;

    if(!gLogEnabled)
        return;
    
    pBuffer = (char*)cissys_malloc(gLogBufferLength);

	va_start(va, format);
	len = utils_vsnprintf( pBuffer,gLogBufferLength - 1,format, va);
	va_end(va);

    if(len < gLogBufferLength - 1){
        pBuffer[len+1] = '\0';
    }

	cissys_logwrite((uint8_t*)pBuffer,utils_strlen(pBuffer));
    cissys_free(pBuffer);
};


#if CIS_ENABLE_LOG
#	if LOG_OUTPUT_EXTINFO

static inline const char* cutFileName(const char* s){
    uint32_t len = utils_strlen(s);
    while((*(s + len) != 0x5C && *(s + len) != 0x2F) && len > 0) len--;
    if(len <= 0)return "";
    return (s + len + 1);
};

#		define LOGE(fmt, ...) logOutput( LOG_LEVEL_ERROR,	LOG_TAG, STRCONCAT( "%s, line:%d, ", fmt), cutFileName(__FILE__), __LINE__, ##__VA_ARGS__)
#		define LOGW(fmt, ...) logOutput( LOG_LEVEL_WARNING, LOG_TAG, STRCONCAT( "%s, line:%d, ", fmt), cutFileName(__FILE__), __LINE__, ##__VA_ARGS__)
#		define LOGI(fmt, ...) logOutput( LOG_LEVEL_INFO,	LOG_TAG, STRCONCAT( "%s, line:%d, ", fmt), cutFileName(__FILE__), __LINE__, ##__VA_ARGS__)
#		define LOGD(fmt, ...) logOutput( LOG_LEVEL_DEBUG,	LOG_TAG, STRCONCAT( "%s, line:%d, ", fmt), cutFileName(__FILE__), __LINE__, ##__VA_ARGS__)
#		define LOG_PRINT(fmt, ...) logData( fmt, ##__VA_ARGS__)
#	else
#		define LOGE(fmt, ...) logOutput( LOG_LEVEL_ERROR,	LOG_TAG, fmt, ##__VA_ARGS__)
#		define LOGW(fmt, ...) logOutput( LOG_LEVEL_WARNING, LOG_TAG, fmt, ##__VA_ARGS__)
#		define LOGI(fmt, ...) logOutput( LOG_LEVEL_INFO,	LOG_TAG, fmt, ##__VA_ARGS__)
#		define LOGD(fmt, ...) logOutput( LOG_LEVEL_DEBUG,	LOG_TAG, fmt, ##__VA_ARGS__)
#		define LOG_PRINT(fmt, ...) logData( fmt, ##__VA_ARGS__)
#	endif
#else
#	define LOGE(fmt, ...) do {} while(0)
#	define LOGW(fmt, ...) do {} while(0)
#	define LOGI(fmt, ...) do {} while(0)
#	define LOGD(fmt, ...) do {} while(0)
#	define LOG_PRINT(fmt, ...) do {} while(0)
#endif

#if LOG_DUMP_ENABLED
#   define DUMP_BUF(b,l)	log_dump(NULL,b,l,0)
#   define LOG_BUF(t,b,l)	log_dump(t,b,l,0)
#else
#	define DUMP_BUF(b,l)    do {} while(0)
#	define LOG_BUF(t,b,l)   do {} while(0)
#endif//LOG_DUMP_ENABLED


#ifdef __cplusplus
extern "C" {
#endif

void log_config(bool enable,uint8_t exoutput,uint8_t level,uint16_t bufsize);

#if LOG_DUMP_ENABLED
void log_dump(const char* title,const uint8_t * buffer,int length,int indent);
#endif//LOG_DUMP_ENABLED


#ifdef __cplusplus
};
#endif

#endif

