/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_TIME_H
#define OS_SHARED_TIME_H

#include "osapi-timer.h"

#include "os-shared-globaldefs.h"
#include "os-shared-idmap.h"

#define TIMECB_FLAG_DEDICATED_TIMEBASE 0x1

typedef struct
{
    char              timer_name[OS_MAX_API_NAME];
    uint32            flags;
    OS_object_token_t timebase_token;
    osal_id_t         prev_cb;
    osal_id_t         next_cb;
    uint32            backlog_resets;
    int32             wait_time;
    int32             interval_time;
    OS_ArgCallback_t  callback_ptr;
    void *            callback_arg;
} OS_timecb_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_timecb_internal_record_t OS_timecb_table[OS_MAX_TIMERS];

/*---------------------------------------------------------------------------------------
   Name: OS_TimerCbAPI_Init

   Purpose: Initialize the OS-independent layer for timer callback objects

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_TimerCbAPI_Init(void);

#endif /* OS_SHARED_TIME_H */
