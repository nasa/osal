/**
 * \file
 *
 * \ingroup  rtems
 *
 */

#ifndef OS_IMPL_GETTIME_H
#define OS_IMPL_GETTIME_H

#include "osconfig.h"
#include <time.h>

/**
 * \brief Identifies the clock ID for OSAL clock operations on RTEMS
 *
 * This is the POSIX clock ID that will be used to implement
 * OS_GetLocalTime() and OS_SetLocalTime().
 */
#define OSAL_GETTIME_SOURCE_CLOCK CLOCK_REALTIME

#endif /* OS_IMPL_GETTIME_H */
