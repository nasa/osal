/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/*
 * \file
 *
 *   RTEMS main entry point
 *   Configures RTEMS and wraps OS_BSPMain for use in a stand alone executable
 */

/*
**  Include Files
*/
#include "bsp_start.h"
#include "bsp-impl.h"
#include <rtems.h>

/*
 ** A simple entry point to start from the loader
 */
rtems_task Init(rtems_task_argument ignored)
{
    OS_BSPMain();
}

/* configuration information */

/*
** RTEMS OS Configuration definitions
*/
#define TASK_INTLEVEL 0
#define CONFIGURE_INIT
#define CONFIGURE_INIT_TASK_ATTRIBUTES \
    (RTEMS_FLOATING_POINT | RTEMS_PREEMPT | RTEMS_NO_TIMESLICE | RTEMS_ASR | RTEMS_INTERRUPT_LEVEL(TASK_INTLEVEL))
#define CONFIGURE_INIT_TASK_STACK_SIZE (20 * 1024)
#define CONFIGURE_INIT_TASK_PRIORITY   10

/*
 * Note that these resources are shared with RTEMS itself (e.g. the init task, the shell)
 * so they should be allocated slightly higher than the user limits in osconfig.h
 *
 * Many RTEMS services use tasks internally, including the idle task, BSWP, ATA driver,
 * low level console I/O, the shell, TCP/IP network stack, and DHCP (if enabled).
 * Many of these also use semaphores for synchronization.
 *
 * Budgeting for additional:
 *   8 internal tasks
 *   2 internal timers
 *   4 internal queues
 *   16 internal semaphores
 *
 */
#define CONFIGURE_MAXIMUM_TASKS          (OS_MAX_TASKS + 8)
#define CONFIGURE_MAXIMUM_TIMERS         (OS_MAX_TIMERS + 2)
#define CONFIGURE_MAXIMUM_SEMAPHORES     (OS_MAX_BIN_SEMAPHORES + OS_MAX_COUNT_SEMAPHORES + OS_MAX_MUTEXES + 16)
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES (OS_MAX_QUEUES + 4)
#define CONFIGURE_MAXIMUM_DRIVERS        10
#define CONFIGURE_MAXIMUM_POSIX_KEYS     4
#ifdef OS_RTEMS_4_DEPRECATED
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS (OS_MAX_NUM_OPEN_FILES + 8)
#else
#define CONFIGURE_MAXIMUM_FILE_DESCRIPTORS (OS_MAX_NUM_OPEN_FILES + 8)
#endif

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_FILESYSTEM_RFS
#define CONFIGURE_FILESYSTEM_IMFS
#define CONFIGURE_FILESYSTEM_DOSFS
#define CONFIGURE_FILESYSTEM_DEVFS
#define CONFIGURE_APPLICATION_NEEDS_LIBBLOCK

/* TODO figure out how to switch these if needed
#define CONFIGURE_APPLICATION_NEEDS_IDE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_ATA_DRIVER */

#define CONFIGURE_EXECUTIVE_RAM_SIZE       (8 * 1024 * 1024)
#define CONFIGURE_MICROSECONDS_PER_TICK    10000
#define CONFIGURE_ATA_DRIVER_TASK_PRIORITY 9

#include <rtems/confdefs.h>
