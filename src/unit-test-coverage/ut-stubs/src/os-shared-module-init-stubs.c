/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-module header
 */

#include "os-shared-module.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ModuleAPI_Init()
 * ----------------------------------------------------
 */
int32 OS_ModuleAPI_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_ModuleAPI_Init, int32);

    UT_GenStub_Execute(OS_ModuleAPI_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ModuleAPI_Init, int32);
}
