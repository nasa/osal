/*
 * File:  bsp_app_run.c
 *
 * Purpose:
 *   Application run default implementation.
 *
 * NOTE: This is isolated in a separate compilation unit, so that a user
 *   application may directly provide an OS_Application_Run() implementation
 *   which will override this default.
 */

#include "osapi-common.h"
#include "bsp-impl.h"

/*
 * The default implementation of OS_Application_Run()
 * just calls the OS_IdleLoop() provided by OSAL.
 */
void OS_Application_Run(void)
{
    OS_IdleLoop();
}
