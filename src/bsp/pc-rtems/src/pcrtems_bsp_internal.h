/**
 * \file
 *
 * Purpose:
 *   Header file for internal data to the PC-RTEMS BSP
 */

#ifndef PCRTEMS_BSP_INTERNAL_H
#define PCRTEMS_BSP_INTERNAL_H

/*
** OSAL includes
*/
#include "bsp-impl.h"

#include <rtems.h>

/*
 * BSP compile-time tuning
 */
#define RTEMS_MAX_USER_OPTIONS 4
#define RTEMS_MAX_CMDLINE      256

/*
 * Handle the differences between RTEMS 5 and 4.11 copyright notice
 */
#ifdef _RTEMS_5_
#define OSAL_BSP_COPYRIGHT_NOTICE rtems_get_copyright_notice()
#else
#define OSAL_BSP_COPYRIGHT_NOTICE _Copyright_Notice
#endif

/*
 * The location which the general purpose file system will be mounted
 */
#define RTEMS_USER_FS_MOUNTPOINT "/mnt"

/*
 * By default put the shell at the same priority
 * as the utility task which handles OS_printf()
 */
#define RTEMS_SHELL_PRIORITY OS_UTILITYTASK_PRIORITY

/*
** BSP types
*/
typedef struct
{
    char     UserArgBuffer[RTEMS_MAX_CMDLINE];
    bool     BatchMode;
    rtems_id AccessMutex;
} OS_BSP_PcRtemsGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_PcRtemsGlobalData_t OS_BSP_PcRtemsGlobal;

#endif /* PCRTEMS_BSP_INTERNAL_H */
