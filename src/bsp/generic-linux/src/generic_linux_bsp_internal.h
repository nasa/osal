/**
 * \file
 *
 * Purpose:
 *   Header file for internal data to the LINUX BSP
 */

#ifndef GENERIC_LINUX_BSP_INTERNAL_H
#define GENERIC_LINUX_BSP_INTERNAL_H

#include "osapi-common.h"
#include "osapi-bsp.h"
#include "osapi-error.h"
#include "bsp-impl.h"

#include <pthread.h>

/*
** BSP types
*/
typedef struct
{
    bool            EnableTermControl; /**< Will be set "true" when invoked from a TTY device, false otherwise */
    pthread_mutex_t AccessMutex;
    int             AccessCancelState;
} OS_BSP_GenericLinuxGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_GenericLinuxGlobalData_t OS_BSP_GenericLinuxGlobal;

#endif /* GENERIC_LINUX_BSP_INTERNAL_H */
