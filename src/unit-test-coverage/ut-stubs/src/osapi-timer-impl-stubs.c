/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     osapi-timer-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"
#include "os-shared-timebase.h"
#include "os-shared-clock.h"

/*
** OS Time/Tick related API
*/
UT_DEFAULT_STUB(OS_TimeBaseCreate_Impl,(uint32 timer_id))
UT_DEFAULT_STUB(OS_TimeBaseSet_Impl,(uint32 timer_id, int32 start_time, int32 interval_time))
UT_DEFAULT_STUB(OS_TimeBaseDelete_Impl,(uint32 timer_id))
void  OS_TimeBaseLock_Impl          (uint32 timebase_id)
{
    UT_DEFAULT_IMPL(OS_TimeBaseLock_Impl);
}

void  OS_TimeBaseUnlock_Impl        (uint32 timebase_id)
{
    UT_DEFAULT_IMPL(OS_TimeBaseUnlock_Impl);
}

UT_DEFAULT_STUB(OS_TimeBaseGetInfo_Impl,(uint32 timer_id, OS_timebase_prop_t *timer_prop))

UT_DEFAULT_STUB(OS_TimeBaseRegister_Impl,(uint32 timebase_id))
/*
 * Clock API low-level handlers
 */
UT_DEFAULT_STUB(OS_GetLocalTime_Impl,(OS_time_t *time_struct))
UT_DEFAULT_STUB(OS_SetLocalTime_Impl,(const OS_time_t *time_struct))


