/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-bsp header
 */

#include "osapi-bsp.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BSP_GetArgC()
 * ----------------------------------------------------
 */
uint32 OS_BSP_GetArgC(void)
{
    UT_GenStub_SetupReturnBuffer(OS_BSP_GetArgC, uint32);

    UT_GenStub_Execute(OS_BSP_GetArgC, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_BSP_GetArgC, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BSP_GetArgV()
 * ----------------------------------------------------
 */
char *const *OS_BSP_GetArgV(void)
{
    UT_GenStub_SetupReturnBuffer(OS_BSP_GetArgV, char *const *);

    UT_GenStub_Execute(OS_BSP_GetArgV, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_BSP_GetArgV, char *const *);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BSP_GetResourceTypeConfig()
 * ----------------------------------------------------
 */
uint32 OS_BSP_GetResourceTypeConfig(uint32 ResourceType)
{
    UT_GenStub_SetupReturnBuffer(OS_BSP_GetResourceTypeConfig, uint32);

    UT_GenStub_AddParam(OS_BSP_GetResourceTypeConfig, uint32, ResourceType);

    UT_GenStub_Execute(OS_BSP_GetResourceTypeConfig, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_BSP_GetResourceTypeConfig, uint32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BSP_SetExitCode()
 * ----------------------------------------------------
 */
void OS_BSP_SetExitCode(int32 code)
{
    UT_GenStub_AddParam(OS_BSP_SetExitCode, int32, code);

    UT_GenStub_Execute(OS_BSP_SetExitCode, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_BSP_SetResourceTypeConfig()
 * ----------------------------------------------------
 */
void OS_BSP_SetResourceTypeConfig(uint32 ResourceType, uint32 ConfigOptionValue)
{
    UT_GenStub_AddParam(OS_BSP_SetResourceTypeConfig, uint32, ResourceType);
    UT_GenStub_AddParam(OS_BSP_SetResourceTypeConfig, uint32, ConfigOptionValue);

    UT_GenStub_Execute(OS_BSP_SetResourceTypeConfig, Basic, NULL);
}
