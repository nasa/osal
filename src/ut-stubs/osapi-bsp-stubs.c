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
