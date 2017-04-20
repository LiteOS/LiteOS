#ifndef _CMSIS_LOS_H
#define _CMSIS_LOS_H

#include "cmsis_os.h"


#if (LOSCFG_COMPAT_CMSIS_FW == YES)

typedef struct fw_MailQ_def{
    uint32_t    event_begin_time;   /* Last time when the queue was successfully read. */
    uint32_t    timeout_cnt;        /* Number of times the processing of messages in the queue times out. */
    fw_event_t  last_event;         /* null indicates that no event is being processed or the previous event has been processed. */
    struct fw_MailQ_def *next;
    osMailQDef_t *queue_id;
}fwMailQDef_t;

typedef struct fwMailQDef_t *fwMailQId;
extern fwMailQId g_fwMailQList;

extern uint32_t g_maxEventTime; /* Default maximum time for processing a single event. */

#define fwMailQDef(name, queue_sz, type)  \
    osMailQDef(name, queue_sz, type);    \
    fwMailQDef_t fw_mailQ_def_##name = \
    {0, 0, {0, 0, 0}, NULL, osMailQ(name)}

#define fwMailQ(name)  \
    &fw_mailQ_def_##name


#define SET_EVENT_PHASE(mailQ, phase) \
    (((fwMailQDef_t *)(mailQ))->last_event.option = (0xFFFFFFC0 & (((fwMailQDef_t *)(mailQ))->last_event.option)) | (0x3F & (phase)))

#define GET_EVENT_PHASE(mailQ)        \
    ((((fwMailQDef_t *)(mailQ))->last_event.option) & 0x3F)

#define SET_EVENT_MAXTIME(mailQ, time)   \
    ((((fwMailQDef_t *)(mailQ))->last_event.option) |= ((time) << 0x6))

#define GET_EVENT_MAXTIME(mailQ)    \
    ((((fwMailQDef_t *)(mailQ))->last_event.option) >> 0x6)

#else

#define fwMailQDef(name, queue_sz, type)  \
    osMailQDef(name, queue_sz, type)

#define fwMailQ(name)  \
    osMailQ(name)

#define fwMailQId osMailQId
#define fwMailQDef_t osMailQDef_t

#define SET_EVENT_PHASE(event, phase)
#define GET_EVENT_PHASE(event)
#define SET_EVENT_MAXTIME(event, time)
#define GET_EVENT_MAXTIME(event)
#endif

typedef struct fw_event_def
{
    uint8_t tag;
    uint8_t cmd;
    uint16_t option; /* Bits 0-5 indicate the message processing progress; bits 6-15 indicate the timeout period for processing this command. */
}fw_event_t;


fwMailQId fwMailCreate (fwMailQDef_t *queue_def, osThreadId thread_id);
void *fwMailAlloc (fwMailQId queue_id, uint32_t millisec, uint8_t tag, uint8_t cmd);
void *fwMailCAlloc (fwMailQId queue_id, uint32_t millisec, uint8_t tag, uint8_t cmd);
osStatus fwMailPut (fwMailQId queue_id, void *mail);
osEvent fwMailGet (fwMailQId queue_id, uint32_t millisec);
osStatus fwMailFree (fwMailQId queue_id, void *mail);
uint32_t fwMailQGetStatus(void);


#endif