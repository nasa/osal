/*
 * File:  bsp_app_startup.c
 *
 * Purpose:
 *   Application startup default implementation.
 *
 * NOTE: This is isolated in a separate compilation unit, so that a user
 *   application may directly provide an OS_Application_Startup() implementation
 *   which will override this default.
 */

#include <stdlib.h>

#include "bsp-impl.h"

/*
 * The default implementation of OS_Application_Startup()
 * just calls the OS_API_Init() provided by OSAL.
 */
void OS_Application_Startup(void)
{
    int32 Status;

    Status = OS_API_Init();
    if (Status != OS_SUCCESS)
    {
        BSP_DEBUG("Error: OS_API_Init() failed with status=%d\n", (int)Status);
        OS_ApplicationExit(Status);
    }
}
