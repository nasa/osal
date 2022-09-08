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
 * Auto-Generated stub implementations for functions defined in os-shared-filesys header
 */

#include "os-shared-filesys.h"
#include "utgenstub.h"

void UT_DefaultHandler_OS_FileSysStatVolume_Impl(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysCheckVolume_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileSysCheckVolume_Impl(const OS_object_token_t *token, bool repair)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysCheckVolume_Impl, int32);

    UT_GenStub_AddParam(OS_FileSysCheckVolume_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_FileSysCheckVolume_Impl, bool, repair);

    UT_GenStub_Execute(OS_FileSysCheckVolume_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysCheckVolume_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysFormatVolume_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileSysFormatVolume_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysFormatVolume_Impl, int32);

    UT_GenStub_AddParam(OS_FileSysFormatVolume_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_FileSysFormatVolume_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysFormatVolume_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysMountVolume_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileSysMountVolume_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysMountVolume_Impl, int32);

    UT_GenStub_AddParam(OS_FileSysMountVolume_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_FileSysMountVolume_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysMountVolume_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysStartVolume_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileSysStartVolume_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysStartVolume_Impl, int32);

    UT_GenStub_AddParam(OS_FileSysStartVolume_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_FileSysStartVolume_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysStartVolume_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysStatVolume_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileSysStatVolume_Impl(const OS_object_token_t *token, OS_statvfs_t *result)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysStatVolume_Impl, int32);

    UT_GenStub_AddParam(OS_FileSysStatVolume_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_FileSysStatVolume_Impl, OS_statvfs_t *, result);

    UT_GenStub_Execute(OS_FileSysStatVolume_Impl, Basic, UT_DefaultHandler_OS_FileSysStatVolume_Impl);

    return UT_GenStub_GetReturnValue(OS_FileSysStatVolume_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysStopVolume_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileSysStopVolume_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysStopVolume_Impl, int32);

    UT_GenStub_AddParam(OS_FileSysStopVolume_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_FileSysStopVolume_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysStopVolume_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysUnmountVolume_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileSysUnmountVolume_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysUnmountVolume_Impl, int32);

    UT_GenStub_AddParam(OS_FileSysUnmountVolume_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_FileSysUnmountVolume_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysUnmountVolume_Impl, int32);
}
