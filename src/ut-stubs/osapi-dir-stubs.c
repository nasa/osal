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
 * Auto-Generated stub implementations for functions defined in osapi-dir header
 */

#include "osapi-dir.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_DirectoryClose(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_DirectoryOpen(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_DirectoryRead(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirectoryClose()
 * ----------------------------------------------------
 */
int32 OS_DirectoryClose(osal_id_t dir_id)
{
    UT_GenStub_SetupReturnBuffer(OS_DirectoryClose, int32);

    UT_GenStub_AddParam(OS_DirectoryClose, osal_id_t, dir_id);

    UT_GenStub_Execute(OS_DirectoryClose, Basic, UT_DefaultHandler_OS_DirectoryClose);

    return UT_GenStub_GetReturnValue(OS_DirectoryClose, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirectoryOpen()
 * ----------------------------------------------------
 */
int32 OS_DirectoryOpen(osal_id_t *dir_id, const char *path)
{
    UT_GenStub_SetupReturnBuffer(OS_DirectoryOpen, int32);

    UT_GenStub_AddParam(OS_DirectoryOpen, osal_id_t *, dir_id);
    UT_GenStub_AddParam(OS_DirectoryOpen, const char *, path);

    UT_GenStub_Execute(OS_DirectoryOpen, Basic, UT_DefaultHandler_OS_DirectoryOpen);

    return UT_GenStub_GetReturnValue(OS_DirectoryOpen, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirectoryRead()
 * ----------------------------------------------------
 */
int32 OS_DirectoryRead(osal_id_t dir_id, os_dirent_t *dirent)
{
    UT_GenStub_SetupReturnBuffer(OS_DirectoryRead, int32);

    UT_GenStub_AddParam(OS_DirectoryRead, osal_id_t, dir_id);
    UT_GenStub_AddParam(OS_DirectoryRead, os_dirent_t *, dirent);

    UT_GenStub_Execute(OS_DirectoryRead, Basic, UT_DefaultHandler_OS_DirectoryRead);

    return UT_GenStub_GetReturnValue(OS_DirectoryRead, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DirectoryRewind()
 * ----------------------------------------------------
 */
int32 OS_DirectoryRewind(osal_id_t dir_id)
{
    UT_GenStub_SetupReturnBuffer(OS_DirectoryRewind, int32);

    UT_GenStub_AddParam(OS_DirectoryRewind, osal_id_t, dir_id);

    UT_GenStub_Execute(OS_DirectoryRewind, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_DirectoryRewind, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_mkdir()
 * ----------------------------------------------------
 */
int32 OS_mkdir(const char *path, uint32 access)
{
    UT_GenStub_SetupReturnBuffer(OS_mkdir, int32);

    UT_GenStub_AddParam(OS_mkdir, const char *, path);
    UT_GenStub_AddParam(OS_mkdir, uint32, access);

    UT_GenStub_Execute(OS_mkdir, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_mkdir, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_rmdir()
 * ----------------------------------------------------
 */
int32 OS_rmdir(const char *path)
{
    UT_GenStub_SetupReturnBuffer(OS_rmdir, int32);

    UT_GenStub_AddParam(OS_rmdir, const char *, path);

    UT_GenStub_Execute(OS_rmdir, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_rmdir, int32);
}
