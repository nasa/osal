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

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSysFilterFree()
 * ----------------------------------------------------
 */
bool OS_FileSysFilterFree(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSysFilterFree, bool);

    UT_GenStub_AddParam(OS_FileSysFilterFree, void *, ref);
    UT_GenStub_AddParam(OS_FileSysFilterFree, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_FileSysFilterFree, const OS_common_record_t *, obj);

    UT_GenStub_Execute(OS_FileSysFilterFree, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSysFilterFree, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSys_FindVirtMountPoint()
 * ----------------------------------------------------
 */
bool OS_FileSys_FindVirtMountPoint(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSys_FindVirtMountPoint, bool);

    UT_GenStub_AddParam(OS_FileSys_FindVirtMountPoint, void *, ref);
    UT_GenStub_AddParam(OS_FileSys_FindVirtMountPoint, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_FileSys_FindVirtMountPoint, const OS_common_record_t *, obj);

    UT_GenStub_Execute(OS_FileSys_FindVirtMountPoint, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSys_FindVirtMountPoint, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileSys_Initialize()
 * ----------------------------------------------------
 */
int32 OS_FileSys_Initialize(char *address, const char *fsdevname, const char *fsvolname, size_t blocksize,
                            osal_blockcount_t numblocks, bool should_format)
{
    UT_GenStub_SetupReturnBuffer(OS_FileSys_Initialize, int32);

    UT_GenStub_AddParam(OS_FileSys_Initialize, char *, address);
    UT_GenStub_AddParam(OS_FileSys_Initialize, const char *, fsdevname);
    UT_GenStub_AddParam(OS_FileSys_Initialize, const char *, fsvolname);
    UT_GenStub_AddParam(OS_FileSys_Initialize, size_t, blocksize);
    UT_GenStub_AddParam(OS_FileSys_Initialize, osal_blockcount_t, numblocks);
    UT_GenStub_AddParam(OS_FileSys_Initialize, bool, should_format);

    UT_GenStub_Execute(OS_FileSys_Initialize, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileSys_Initialize, int32);
}
