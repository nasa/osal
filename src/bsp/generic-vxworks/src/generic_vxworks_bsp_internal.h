/**
 * \file
 *
 * Purpose:
 *   Header file for internal data to the VxWorks BSP
 */

#ifndef GENERIC_VXWORKS_BSP_INTERNAL_H
#define GENERIC_VXWORKS_BSP_INTERNAL_H

/*
** OSAL includes
*/
#include "bsp-impl.h"

#include <semLib.h>

/*
** BSP types
*/
typedef struct
{
    SEM_ID AccessMutex;
    VX_MUTEX_SEMAPHORE(AccessMutexMem);

} OS_BSP_GenericVxWorksGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_GenericVxWorksGlobalData_t OS_BSP_GenericVxWorksGlobal;

#endif /* GENERIC_VXWORKS_BSP_INTERNAL_H */
