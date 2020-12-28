/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.
 * Description: Sensor Manager
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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
 * --------------------------------------------------------------------------- */

#include "sensor_manager.h"
#include "sensor_commu.h"
#include "los_printf.h"
#include "los_base.h"
#include "los_task.h"
#include "los_queue.h"
#include "los_sem.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define STASK_PRIORITY_SM  3
#define STASK_STKDEPTH_SM  0x800

#define SM_MAILS_SIZE      10
#define SM_MAILS_TYPE_SIZE sizeof(SmMail)

#define CONFIG_BUFFER_SIZE 25
#define CNT_INVLID         0xffffffff

STATIC UINT32 g_sensorManagerTskId;
STATIC UINT32 g_sensorManagerQueue;
STATIC UINT32 g_sensorManagerSem;

STATIC UINT32 g_configBuffer[CONFIG_BUFFER_SIZE];
STATIC SensorScbTable g_sensorScbTable[TAG_END - 1];

SensorScbTable *SensorTableGet(VOID)
{
    return (SensorScbTable *)&g_sensorScbTable;
}

STATIC UINT8 ScbTableIndex(UINT8 tag)
{
    return (tag - TAG_BEGIN);
}

INT32 SensorRegister(SensorType *sensor)
{
    if ((sensor == NULL) || (sensor->tag >= TAG_END) || (sensor->tag < TAG_BEGIN)) {
        return LOS_NOK;
    }

    UINT8 index = ScbTableIndex(sensor->tag);

    PRINT_DEBUG("tag:%d, SensorType:0x%x\n", sensor->tag, sensor);
    g_sensorScbTable[index].sensorInterface = sensor;

    LOS_ListInit(&sensor->slist);

    return LOS_OK;
}

STATIC VOID ScbDataUpdate(SensorType *sensor)
{
    UINT32 cnt;
    SensorItem *item = NULL;
    struct LOS_DL_LIST *pl = NULL;

    LOS_DL_LIST_FOR_EACH(pl, &sensor->slist) {
        item = LOS_DL_LIST_ENTRY(pl, SensorItem, list);
        if (item->state != WORKING) {
            continue;
        }
        if (item->updateFunc != NULL) {
            item->updateTime += sensor->interval;
            cnt = item->updateTime / item->openParam.period;
            if (cnt != item->updateCnt) {
                item->updateCnt = cnt;
                item->updateFunc(item->updateArg, sensor->sensorData, sensor->sensorDataLen);
            }
        }
    }
}

INT32 SensorSample(SensorType *sensor)
{
    SmMail smMail;
    UINT32 ret;

    if ((sensor == NULL) || (sensor->tag >= TAG_END) || (sensor->tag < TAG_BEGIN)) {
        return LOS_NOK;
    }

    smMail.event = SM_EVENT_SENSOR_SAMPLE;
    smMail.sample.tag = sensor->tag;
    ret = LOS_QueueWriteCopy(g_sensorManagerQueue, (VOID *)&smMail, SM_MAILS_TYPE_SIZE, 0);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
    return LOS_OK;
}

STATIC VOID SampleFromMail(const MailSample *sample)
{
    INT32 ret;
    SensorType *sensorInterface = NULL;

    sensorInterface = g_sensorScbTable[ScbTableIndex(sample->tag)].sensorInterface;
    if ((sensorInterface == NULL) || (sensorInterface->sensorOp == NULL) ||
        (sensorInterface->sensorOp->ReadData == NULL)) {
        PRINT_ERR("sample_from_mail, TAG:%d read data fail\n", sample->tag);
        return;
    }
    ret = sensorInterface->sensorOp->ReadData(sensorInterface);
    if (ret != LOS_NOK) {
        ScbDataUpdate(sensorInterface);
    }
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID ScbSwitchComplete(const SensorType *sensor, INT32 result)
{
    SensorItem *item = NULL;
    struct LOS_DL_LIST *pl = NULL;

    LOS_DL_LIST_FOR_EACH(pl, &sensor->slist) {
        item = LOS_DL_LIST_ENTRY(pl, SensorItem, list);
        if ((item->state == OPEN) || (item->state == CLOSE)) {
            if ((item->resp != 0) && (item->respFunc != NULL)) {
                item->respFunc(item->tag, item->id, item->state, result);
            }
            item->state -= 1; // OPEN->WORKING, CLOSE->IDLE
        }
    }
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 UpdateScbPeriod(SensorType *sensor, const SensorItem *currentItem)
{
    SensorItem *item       = NULL;
    struct LOS_DL_LIST *pl = NULL;
    UINT8 cnt              = 0;
    UINT8 mode             = 0;
    OpenParam openParam    = currentItem->openParam;

    LOS_DL_LIST_FOR_EACH(pl, &sensor->slist) {
        item = LOS_DL_LIST_ENTRY(pl, SensorItem, list);
        if ((item->state == WORKING) || (item->state == OPEN)) {
            if (item->openParam.mode >= MODE_END) {
                item->openParam.mode = 0;
            }
            mode |= 1 << item->openParam.mode;

            if (item->openParam.period < openParam.period) {
                openParam.period = item->openParam.period;
            }
            if (item->openParam.batchCount < openParam.batchCount) {
                openParam.batchCount = item->openParam.batchCount;
            }
            cnt++;
        }
    }

    if (cnt == 0) {
        sensor->openParam.period = 0;
        if (item != NULL) {
            sensor->closeParam = item->closeParam;
        }
        return SENSOR_ATTACH_NONE;
    }

    if (mode & (1 << REALTIME_MODE)) {
        openParam.mode = REALTIME_MODE;
    } else if (mode & (1 << INTEGRATE_MODE)) {
        openParam.mode = INTEGRATE_MODE;
    } else if (mode & (1 << AUTO_MODE)) {
        openParam.mode = AUTO_MODE;
    } else {
        openParam.mode = FIFO_MODE;
    }

    if (memcmp(&(sensor->openParam), &openParam, sizeof(OpenParam))) {
        sensor->openParam = openParam;
        return SENSOR_ATTACH_CHANGE;
    }
    return SENSOR_ATTACH_UNCHANGE;
}

LITE_OS_SEC_TEXT_MINOR STATIC INT32 SensorSwitch(const SensorItem *item)
{
    INT32 ret = LOS_OK;
    SensorType *sensorInterface = g_sensorScbTable[ScbTableIndex(item->tag)].sensorInterface;
    if (sensorInterface == NULL) {
        PRINT_ERR("sensor_switch, TAG:%d SensorType is null\n", item->tag);
        return LOS_NOK;
    }

    switch (UpdateScbPeriod(sensorInterface, item)) {
        case SENSOR_ATTACH_NONE:  // no item is working,sensor would be turn off
            if ((sensorInterface->sensorOp == NULL) || (sensorInterface->sensorOp->Close == NULL)) {
                break;
            }
            ret = sensorInterface->sensorOp->Close(sensorInterface);
            if (!ret) {
                sensorInterface->sensorStat &= ~SENSOR_STAT_OPEN;
            }
            break;
        case SENSOR_ATTACH_CHANGE:  // sensor would be turn on,or change period
            sensorInterface->interval = sensorInterface->openParam.period;
            if ((sensorInterface->sensorOp == NULL) || (sensorInterface->sensorOp->Open == NULL)) {
                break;
            }
            ret = sensorInterface->sensorOp->Open(sensorInterface, &sensorInterface->openParam);
            if (!ret) {
                sensorInterface->sensorStat |= SENSOR_STAT_OPEN;
            }
            break;
        default:  // sensor is on ,and period no change
            break;
    }
    return ret;
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID ItemSwitch(const SensorItem *item)
{
    INT32 res;
    SensorType *sensorInterface = g_sensorScbTable[ScbTableIndex(item->tag)].sensorInterface;

    if (sensorInterface == NULL) {
        PRINT_ERR("item_switch, TAG:%d SensorType is null\n", item->tag);
        return;
    }
    PRINT_DEBUG("===start sensor_switch \n");
    res = SensorSwitch(item);
    PRINT_DEBUG("===end sensor_switch \n");
    ScbSwitchComplete(sensorInterface, res);
    PRINT_DEBUG("===end scb_switch_complete \n");
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID ItemEnable(const MailItemEnable *itemEnable)
{
    PRINT_DEBUG("TAG %d item enable\n", itemEnable->item->tag);
    itemEnable->item->state      = OPEN;
    itemEnable->item->resp       = itemEnable->resp;
    itemEnable->item->updateCnt  = CNT_INVLID;  // Insure the first data be updated to app
    itemEnable->item->updateTime = 0;
    itemEnable->item->openParam  = itemEnable->openParam;
    itemEnable->item->id         = itemEnable->id;
    ItemSwitch(itemEnable->item);
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID ItemDisable(const MailItemDisable *itemDisable)
{
    PRINT_DEBUG("TAG %d item disable\n", itemDisable->item->tag);
    itemDisable->item->state      = CLOSE;
    itemDisable->item->resp       = itemDisable->resp;
    itemDisable->item->closeParam = itemDisable->closeParam;
    itemDisable->item->id         = itemDisable->id;
    ItemSwitch(itemDisable->item);
}

LITE_OS_SEC_TEXT_MINOR STATIC VOID ItemInit(const MailItemInit *itemInit)
{
    SensorType *sensorInterface = NULL;
    SensorItem *item = NULL;

    PRINT_DEBUG("item init tag %d index %d \n", itemInit->item->tag, itemInit->tag - TAG_BEGIN);
    itemInit->item->state      = IDLE;
    itemInit->item->respFunc   = itemInit->respFunc;
    itemInit->item->tag        = itemInit->tag;
    itemInit->item->updateFunc = itemInit->updateFunc;
    itemInit->item->updateArg  = itemInit->updateArg;

    sensorInterface = g_sensorScbTable[ScbTableIndex(itemInit->tag)].sensorInterface;
    if (sensorInterface != NULL) {
        LOS_DL_LIST_FOR_EACH_ENTRY(item, &(sensorInterface->slist), SensorItem, list) {
            if ((item->tag == itemInit->item->tag) && (item->id == itemInit->item->id)) {
                PRINT_ERR("duplicate item init err tag %d id %u\n", item->tag, item->id);
                return;
            }
        }
        LOS_ListAdd(&(sensorInterface->slist), &(itemInit->item->list));
    }
}

LITE_OS_SEC_TEXT_MINOR INT32 SensorItemEnable(SensorItem *item, OpenParam *param, UINT16 id, UINT8 resp)
{
    SmMail smMail;
    UINT32 ret;

    if ((item == NULL) || (item->tag >= TAG_END) || (item->tag < TAG_BEGIN) || (param == NULL)) {
        return LOS_NOK;
    }

    if (param->period < SAMPLE_PERIOD_MIN) {
        PRINT_ERR("sensor item enable open period err %u min %u\n", param->period, SAMPLE_PERIOD_MIN);
    }

    smMail.event = SM_EVENT_ITEM_ENABLE;
    smMail.itemEnable.item = item;

    param->period = (param->period < SAMPLE_PERIOD_MIN) ? SAMPLE_PERIOD_MIN : param->period;
    param->mode = (param->mode >= MODE_END) ? AUTO_MODE : param->mode;
    param->batchCount = (param->batchCount > BATCH_COUNT_MAX) ? 1 : param->batchCount;
    smMail.itemEnable.openParam = *param;
    smMail.itemEnable.id = id;
    smMail.itemEnable.resp = resp;

    ret = LOS_QueueWriteCopy(g_sensorManagerQueue, (VOID *)&smMail, SM_MAILS_TYPE_SIZE, 0);
    if (ret != LOS_OK) {
        PRINT_ERR("sensor item enable write queue fail, ret %u\n", ret);
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 SensorItemDisable(SensorItem *item, CloseParam *param, UINT16 id, UINT8 resp)
{
    SmMail smMail;
    UINT32 ret;

    if ((item == NULL) || (item->tag >= TAG_END) || (item->tag < TAG_BEGIN) || (param == NULL)) {
        return LOS_NOK;
    }

    smMail.event                  = SM_EVENT_ITEM_DISABLE;
    smMail.itemDisable.item       = item;
    smMail.itemDisable.closeParam = *param;
    smMail.itemDisable.id         = id;
    smMail.itemDisable.resp       = resp;

    ret = LOS_QueueWriteCopy(g_sensorManagerQueue, (VOID *)&smMail, SM_MAILS_TYPE_SIZE, 0);
    if (ret != LOS_OK) {
        PRINT_ERR("sensor item disable write queue fail, ret %u\n", ret);
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 SensorItemInit(
    SensorItem *item, RespFunc respFunc, ObjTag tag, UpdateFunc updateFunc, UINT32 upArg)
{
    SmMail smMail;
    UINT32 ret;

    if ((item == NULL) || (tag >= TAG_END) || (tag < TAG_BEGIN)) {
        return LOS_NOK;
    }

    smMail.event               = SM_EVENT_ITEM_INIT;
    smMail.itemInit.item       = item;
    smMail.itemInit.respFunc   = respFunc;
    smMail.itemInit.tag        = tag;
    smMail.itemInit.updateFunc = updateFunc;
    smMail.itemInit.updateArg  = upArg;

    ret = LOS_QueueWriteCopy(g_sensorManagerQueue, (VOID *)&smMail, SM_MAILS_TYPE_SIZE, 0);
    if (ret != LOS_OK) {
        PRINT_ERR("sensor item init write queue fail, ret %u\n", ret);
        return LOS_NOK;
    }
    return LOS_OK;
}

LITE_OS_SEC_TEXT_MINOR INT32 SensorConfigNotify(const PktParameterReq *pkt, UINT8 resp)
{
    SmMail  smMail;
    errno_t rc;
    UINT32  ret;

    if ((pkt == NULL) || (pkt->hd.tag >= TAG_END) || (pkt->hd.tag < TAG_BEGIN)) {
        return LOS_NOK;
    }

    ret = LOS_SemPend(g_sensorManagerSem, LOS_WAIT_FOREVER);
    if (ret != LOS_OK) {
        PRINT_ERR("Sensor Manager Sem pend fail, ret %u\n", ret);
        return LOS_NOK;
    }

    rc = memcpy_s(g_configBuffer, sizeof(g_configBuffer), pkt->para, pkt->hd.length);
    if (rc != EOK) {
        PRINT_ERR("g_configBuffer copy error\n");
        LOS_SemPost(g_sensorManagerSem);
        return LOS_NOK;
    }

    smMail.event       = SM_EVENT_CONFIG;
    smMail.config.cmd  = pkt->hd.cmd;
    smMail.config.id   = pkt->hd.tranId;
    smMail.config.resp = resp;
    smMail.config.tag  = pkt->hd.tag;

    ret = LOS_QueueWriteCopy(g_sensorManagerQueue, (VOID *)&smMail, SM_MAILS_TYPE_SIZE, 0);
    if (ret != LOS_OK) {
        PRINT_ERR("sensor config notify write queue fail, ret %u\n", ret);
        LOS_SemPost(g_sensorManagerSem);
        return LOS_NOK;
    }
    return LOS_OK;
}

STATIC VOID SensorConfigRespond(UINT8 tag, UINT8 cmd, UINT16 id, UINT8 resp, INT32 result)
{
    PktHeaderResp rpkt;

    if (resp) {
        rpkt.tag    = tag;
        rpkt.resp   = NO_RESP;
        rpkt.tranId = id;
        rpkt.length = CMN_RESP_LENGTH;
        rpkt.errNo  = ((result < 0) ? EN_FAIL : EN_OK);
        rpkt.cmd    = cmd + 1;
        SensorCommuSend((UINT8 *)&rpkt, sizeof(rpkt));
    }
}

LITE_OS_SEC_TEXT_MINOR VOID STATIC SensorConfig(const MailConfig *config)
{
    SensorType *sensor = NULL;
    INT32 result = LOS_NOK;

    sensor = g_sensorScbTable[ScbTableIndex(config->tag)].sensorInterface;
    if (sensor != NULL) {
        if ((sensor->sensorOp != NULL) && (sensor->sensorOp->IoCtl != NULL)) {
            result = sensor->sensorOp->IoCtl(sensor, config->cmd, (UINT8 *)g_configBuffer);
        }
    }

    SensorConfigRespond(config->tag, config->cmd, config->id, config->resp, result);
    PRINT_DEBUG("sensor_config_respond %d %d\n", config->cmd, result);
    LOS_SemPost(g_sensorManagerSem);
}

LITE_OS_SEC_TEXT STATIC VOID SmTask(VOID const *arg)
{
    SmMail smMail;
    UINT32 ret;
    UINT32 bufferSize = SM_MAILS_TYPE_SIZE;

    while (1) {
        ret = LOS_QueueReadCopy(g_sensorManagerQueue, &smMail, &bufferSize, LOS_WAIT_FOREVER);
        if (ret != LOS_OK) {
            PRINT_ERR("sm task read queue fail, ret %u\n", ret);
            continue;
        }

        switch (smMail.event) {
            case SM_EVENT_SENSOR_SAMPLE:
                SampleFromMail(&(smMail.sample));
                break;
            case SM_EVENT_ITEM_ENABLE:
                ItemEnable(&(smMail.itemEnable));
                break;
            case SM_EVENT_ITEM_DISABLE:
                ItemDisable(&(smMail.itemDisable));
                break;
            case SM_EVENT_ITEM_INIT:
                ItemInit(&(smMail.itemInit));
                break;
            case SM_EVENT_CONFIG:
                SensorConfig(&(smMail.config));
                break;
            default:
                break;
        }
    }
}

INT32 SensorManagerInit(VOID)
{
    TSK_INIT_PARAM_S taskInitParam = {0};
    const UINT16 semCount = 1;
    UINT32 ret;

    ret = LOS_QueueCreate("SmQueue", SM_MAILS_SIZE, &g_sensorManagerQueue, 0, SM_MAILS_TYPE_SIZE);
    if (ret != LOS_OK) {
        PRINT_ERR("Sensor Manager Queue create fail, ret %u\n", ret);
        return LOS_NOK;
    }

    ret = LOS_SemCreate(semCount, &g_sensorManagerSem);
    if (ret != LOS_OK) {
        PRINT_ERR("Sensor Manager Sem create fail, ret %u\n", ret);
        return LOS_NOK;
    }

    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)SmTask;
    taskInitParam.uwStackSize  = STASK_STKDEPTH_SM;
    taskInitParam.pcName       = "Sensor Manager task";
    taskInitParam.usTaskPrio   = STASK_PRIORITY_SM; /* 1~7 */
    taskInitParam.uwResved     = LOS_TASK_STATUS_DETACHED; /* task is detached, the task can deleteself */

    ret = LOS_TaskCreate(&g_sensorManagerTskId, &taskInitParam);
    if (ret != LOS_OK) {
        PRINT_ERR("Sensor Manager Task create fail, ret %u\n", ret);
        return LOS_NOK;
    }
    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
