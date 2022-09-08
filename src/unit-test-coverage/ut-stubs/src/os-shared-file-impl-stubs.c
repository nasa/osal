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
 * Auto-Generated stub implementations for functions defined in os-shared-file header
 */

#include "os-shared-file.h"
#include "utgenstub.h"

void UT_DefaultHandler_OS_GenericRead_Impl(void *, UT_EntryKey_t, const UT_StubContext_t *);
void UT_DefaultHandler_OS_GenericWrite_Impl(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileChmod_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileChmod_Impl(const char *local_path, uint32 access_mode)
{
    UT_GenStub_SetupReturnBuffer(OS_FileChmod_Impl, int32);

    UT_GenStub_AddParam(OS_FileChmod_Impl, const char *, local_path);
    UT_GenStub_AddParam(OS_FileChmod_Impl, uint32, access_mode);

    UT_GenStub_Execute(OS_FileChmod_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileChmod_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileOpen_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileOpen_Impl(const OS_object_token_t *token, const char *local_path, int32 flags, int32 access_mode)
{
    UT_GenStub_SetupReturnBuffer(OS_FileOpen_Impl, int32);

    UT_GenStub_AddParam(OS_FileOpen_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_FileOpen_Impl, const char *, local_path);
    UT_GenStub_AddParam(OS_FileOpen_Impl, int32, flags);
    UT_GenStub_AddParam(OS_FileOpen_Impl, int32, access_mode);

    UT_GenStub_Execute(OS_FileOpen_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileOpen_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileRemove_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileRemove_Impl(const char *local_path)
{
    UT_GenStub_SetupReturnBuffer(OS_FileRemove_Impl, int32);

    UT_GenStub_AddParam(OS_FileRemove_Impl, const char *, local_path);

    UT_GenStub_Execute(OS_FileRemove_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileRemove_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileRename_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileRename_Impl(const char *old_path, const char *new_path)
{
    UT_GenStub_SetupReturnBuffer(OS_FileRename_Impl, int32);

    UT_GenStub_AddParam(OS_FileRename_Impl, const char *, old_path);
    UT_GenStub_AddParam(OS_FileRename_Impl, const char *, new_path);

    UT_GenStub_Execute(OS_FileRename_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileRename_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileStat_Impl()
 * ----------------------------------------------------
 */
int32 OS_FileStat_Impl(const char *local_path, os_fstat_t *filestat)
{
    UT_GenStub_SetupReturnBuffer(OS_FileStat_Impl, int32);

    UT_GenStub_AddParam(OS_FileStat_Impl, const char *, local_path);
    UT_GenStub_AddParam(OS_FileStat_Impl, os_fstat_t *, filestat);

    UT_GenStub_Execute(OS_FileStat_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileStat_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GenericClose_Impl()
 * ----------------------------------------------------
 */
int32 OS_GenericClose_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_GenericClose_Impl, int32);

    UT_GenStub_AddParam(OS_GenericClose_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_GenericClose_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_GenericClose_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GenericRead_Impl()
 * ----------------------------------------------------
 */
int32 OS_GenericRead_Impl(const OS_object_token_t *token, void *buffer, size_t nbytes, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_GenericRead_Impl, int32);

    UT_GenStub_AddParam(OS_GenericRead_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_GenericRead_Impl, void *, buffer);
    UT_GenStub_AddParam(OS_GenericRead_Impl, size_t, nbytes);
    UT_GenStub_AddParam(OS_GenericRead_Impl, int32, timeout);

    UT_GenStub_Execute(OS_GenericRead_Impl, Basic, UT_DefaultHandler_OS_GenericRead_Impl);

    return UT_GenStub_GetReturnValue(OS_GenericRead_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GenericSeek_Impl()
 * ----------------------------------------------------
 */
int32 OS_GenericSeek_Impl(const OS_object_token_t *token, int32 offset, uint32 whence)
{
    UT_GenStub_SetupReturnBuffer(OS_GenericSeek_Impl, int32);

    UT_GenStub_AddParam(OS_GenericSeek_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_GenericSeek_Impl, int32, offset);
    UT_GenStub_AddParam(OS_GenericSeek_Impl, uint32, whence);

    UT_GenStub_Execute(OS_GenericSeek_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_GenericSeek_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_GenericWrite_Impl()
 * ----------------------------------------------------
 */
int32 OS_GenericWrite_Impl(const OS_object_token_t *token, const void *buffer, size_t nbytes, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_GenericWrite_Impl, int32);

    UT_GenStub_AddParam(OS_GenericWrite_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_GenericWrite_Impl, const void *, buffer);
    UT_GenStub_AddParam(OS_GenericWrite_Impl, size_t, nbytes);
    UT_GenStub_AddParam(OS_GenericWrite_Impl, int32, timeout);

    UT_GenStub_Execute(OS_GenericWrite_Impl, Basic, UT_DefaultHandler_OS_GenericWrite_Impl);

    return UT_GenStub_GetReturnValue(OS_GenericWrite_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ShellOutputToFile_Impl()
 * ----------------------------------------------------
 */
int32 OS_ShellOutputToFile_Impl(const OS_object_token_t *token, const char *Cmd)
{
    UT_GenStub_SetupReturnBuffer(OS_ShellOutputToFile_Impl, int32);

    UT_GenStub_AddParam(OS_ShellOutputToFile_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_ShellOutputToFile_Impl, const char *, Cmd);

    UT_GenStub_Execute(OS_ShellOutputToFile_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_ShellOutputToFile_Impl, int32);
}
