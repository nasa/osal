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
 * Auto-Generated stub implementations for functions defined in osapi-common header
 */

#include "osapi-common.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_API_Init()
 * ----------------------------------------------------
 */
int32 OS_API_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_API_Init, int32);

    UT_GenStub_Execute(OS_API_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_API_Init, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_API_Teardown()
 * ----------------------------------------------------
 */
void OS_API_Teardown(void)
{
    UT_GenStub_Execute(OS_API_Teardown, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ApplicationExit()
 * ----------------------------------------------------
 */
void OS_ApplicationExit(int32 Status)
{
    UT_GenStub_AddParam(OS_ApplicationExit, int32, Status);

    UT_GenStub_Execute(OS_ApplicationExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ApplicationShutdown()
 * ----------------------------------------------------
 */
void OS_ApplicationShutdown(uint8 flag)
{
    UT_GenStub_AddParam(OS_ApplicationShutdown, uint8, flag);

    UT_GenStub_Execute(OS_ApplicationShutdown, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DeleteAllObjects()
 * ----------------------------------------------------
 */
void OS_DeleteAllObjects(void)
{
    UT_GenStub_Execute(OS_DeleteAllObjects, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_IdleLoop()
 * ----------------------------------------------------
 */
void OS_IdleLoop(void)
{
    UT_GenStub_Execute(OS_IdleLoop, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RegisterEventHandler()
 * ----------------------------------------------------
 */
int32 OS_RegisterEventHandler(OS_EventHandler_t handler)
{
    UT_GenStub_SetupReturnBuffer(OS_RegisterEventHandler, int32);

    UT_GenStub_AddParam(OS_RegisterEventHandler, OS_EventHandler_t, handler);

    UT_GenStub_Execute(OS_RegisterEventHandler, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RegisterEventHandler, int32);
}
