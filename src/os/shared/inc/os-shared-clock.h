/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_CLOCK_H
#define OS_SHARED_CLOCK_H

#include "osapi-clock.h"
#include "os-shared-globaldefs.h"

/*
 * Clock API low-level handlers
 * These simply get/set the kernel RTC (if it has one)
 */

/*----------------------------------------------------------------
   Function: OS_GetLocalTime_Impl

    Purpose: Get the time from the RTC

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_GetLocalTime_Impl(OS_time_t *time_struct);

/*----------------------------------------------------------------
   Function: OS_SetLocalTime_Impl

    Purpose: Set the time in the RTC

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_SetLocalTime_Impl(const OS_time_t *time_struct);

#endif /* OS_SHARED_CLOCK_H */
