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
 * Auto-Generated stub implementations for functions defined in osapi-file header
 */

#include "osapi-file.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_FDGetInfo(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_OpenCreate(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimedRead(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_TimedWrite(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_close(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_lseek(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_read(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_stat(void *, UT_EntryKey_t, const UT_StubContext_t *);
extern void UT_DefaultHandler_OS_write(void *, UT_EntryKey_t, const UT_StubContext_t *);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CloseAllFiles()
 * ----------------------------------------------------
 */
int32 OS_CloseAllFiles(void)
{
    UT_GenStub_SetupReturnBuffer(OS_CloseAllFiles, int32);

    UT_GenStub_Execute(OS_CloseAllFiles, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CloseAllFiles, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CloseFileByName()
 * ----------------------------------------------------
 */
int32 OS_CloseFileByName(const char *Filename)
{
    UT_GenStub_SetupReturnBuffer(OS_CloseFileByName, int32);

    UT_GenStub_AddParam(OS_CloseFileByName, const char *, Filename);

    UT_GenStub_Execute(OS_CloseFileByName, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_CloseFileByName, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FDGetInfo()
 * ----------------------------------------------------
 */
int32 OS_FDGetInfo(osal_id_t filedes, OS_file_prop_t *fd_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_FDGetInfo, int32);

    UT_GenStub_AddParam(OS_FDGetInfo, osal_id_t, filedes);
    UT_GenStub_AddParam(OS_FDGetInfo, OS_file_prop_t *, fd_prop);

    UT_GenStub_Execute(OS_FDGetInfo, Basic, UT_DefaultHandler_OS_FDGetInfo);

    return UT_GenStub_GetReturnValue(OS_FDGetInfo, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_FileOpenCheck()
 * ----------------------------------------------------
 */
int32 OS_FileOpenCheck(const char *Filename)
{
    UT_GenStub_SetupReturnBuffer(OS_FileOpenCheck, int32);

    UT_GenStub_AddParam(OS_FileOpenCheck, const char *, Filename);

    UT_GenStub_Execute(OS_FileOpenCheck, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_FileOpenCheck, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_OpenCreate()
 * ----------------------------------------------------
 */
int32 OS_OpenCreate(osal_id_t *filedes, const char *path, int32 flags, int32 access_mode)
{
    UT_GenStub_SetupReturnBuffer(OS_OpenCreate, int32);

    UT_GenStub_AddParam(OS_OpenCreate, osal_id_t *, filedes);
    UT_GenStub_AddParam(OS_OpenCreate, const char *, path);
    UT_GenStub_AddParam(OS_OpenCreate, int32, flags);
    UT_GenStub_AddParam(OS_OpenCreate, int32, access_mode);

    UT_GenStub_Execute(OS_OpenCreate, Basic, UT_DefaultHandler_OS_OpenCreate);

    return UT_GenStub_GetReturnValue(OS_OpenCreate, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimedRead()
 * ----------------------------------------------------
 */
int32 OS_TimedRead(osal_id_t filedes, void *buffer, size_t nbytes, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_TimedRead, int32);

    UT_GenStub_AddParam(OS_TimedRead, osal_id_t, filedes);
    UT_GenStub_AddParam(OS_TimedRead, void *, buffer);
    UT_GenStub_AddParam(OS_TimedRead, size_t, nbytes);
    UT_GenStub_AddParam(OS_TimedRead, int32, timeout);

    UT_GenStub_Execute(OS_TimedRead, Basic, UT_DefaultHandler_OS_TimedRead);

    return UT_GenStub_GetReturnValue(OS_TimedRead, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimedWrite()
 * ----------------------------------------------------
 */
int32 OS_TimedWrite(osal_id_t filedes, const void *buffer, size_t nbytes, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_TimedWrite, int32);

    UT_GenStub_AddParam(OS_TimedWrite, osal_id_t, filedes);
    UT_GenStub_AddParam(OS_TimedWrite, const void *, buffer);
    UT_GenStub_AddParam(OS_TimedWrite, size_t, nbytes);
    UT_GenStub_AddParam(OS_TimedWrite, int32, timeout);

    UT_GenStub_Execute(OS_TimedWrite, Basic, UT_DefaultHandler_OS_TimedWrite);

    return UT_GenStub_GetReturnValue(OS_TimedWrite, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_chmod()
 * ----------------------------------------------------
 */
int32 OS_chmod(const char *path, uint32 access_mode)
{
    UT_GenStub_SetupReturnBuffer(OS_chmod, int32);

    UT_GenStub_AddParam(OS_chmod, const char *, path);
    UT_GenStub_AddParam(OS_chmod, uint32, access_mode);

    UT_GenStub_Execute(OS_chmod, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_chmod, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_close()
 * ----------------------------------------------------
 */
int32 OS_close(osal_id_t filedes)
{
    UT_GenStub_SetupReturnBuffer(OS_close, int32);

    UT_GenStub_AddParam(OS_close, osal_id_t, filedes);

    UT_GenStub_Execute(OS_close, Basic, UT_DefaultHandler_OS_close);

    return UT_GenStub_GetReturnValue(OS_close, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_cp()
 * ----------------------------------------------------
 */
int32 OS_cp(const char *src, const char *dest)
{
    UT_GenStub_SetupReturnBuffer(OS_cp, int32);

    UT_GenStub_AddParam(OS_cp, const char *, src);
    UT_GenStub_AddParam(OS_cp, const char *, dest);

    UT_GenStub_Execute(OS_cp, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_cp, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_lseek()
 * ----------------------------------------------------
 */
int32 OS_lseek(osal_id_t filedes, int32 offset, uint32 whence)
{
    UT_GenStub_SetupReturnBuffer(OS_lseek, int32);

    UT_GenStub_AddParam(OS_lseek, osal_id_t, filedes);
    UT_GenStub_AddParam(OS_lseek, int32, offset);
    UT_GenStub_AddParam(OS_lseek, uint32, whence);

    UT_GenStub_Execute(OS_lseek, Basic, UT_DefaultHandler_OS_lseek);

    return UT_GenStub_GetReturnValue(OS_lseek, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_mv()
 * ----------------------------------------------------
 */
int32 OS_mv(const char *src, const char *dest)
{
    UT_GenStub_SetupReturnBuffer(OS_mv, int32);

    UT_GenStub_AddParam(OS_mv, const char *, src);
    UT_GenStub_AddParam(OS_mv, const char *, dest);

    UT_GenStub_Execute(OS_mv, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_mv, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_read()
 * ----------------------------------------------------
 */
int32 OS_read(osal_id_t filedes, void *buffer, size_t nbytes)
{
    UT_GenStub_SetupReturnBuffer(OS_read, int32);

    UT_GenStub_AddParam(OS_read, osal_id_t, filedes);
    UT_GenStub_AddParam(OS_read, void *, buffer);
    UT_GenStub_AddParam(OS_read, size_t, nbytes);

    UT_GenStub_Execute(OS_read, Basic, UT_DefaultHandler_OS_read);

    return UT_GenStub_GetReturnValue(OS_read, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_remove()
 * ----------------------------------------------------
 */
int32 OS_remove(const char *path)
{
    UT_GenStub_SetupReturnBuffer(OS_remove, int32);

    UT_GenStub_AddParam(OS_remove, const char *, path);

    UT_GenStub_Execute(OS_remove, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_remove, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_rename()
 * ----------------------------------------------------
 */
int32 OS_rename(const char *old_filename, const char *new_filename)
{
    UT_GenStub_SetupReturnBuffer(OS_rename, int32);

    UT_GenStub_AddParam(OS_rename, const char *, old_filename);
    UT_GenStub_AddParam(OS_rename, const char *, new_filename);

    UT_GenStub_Execute(OS_rename, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_rename, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_stat()
 * ----------------------------------------------------
 */
int32 OS_stat(const char *path, os_fstat_t *filestats)
{
    UT_GenStub_SetupReturnBuffer(OS_stat, int32);

    UT_GenStub_AddParam(OS_stat, const char *, path);
    UT_GenStub_AddParam(OS_stat, os_fstat_t *, filestats);

    UT_GenStub_Execute(OS_stat, Basic, UT_DefaultHandler_OS_stat);

    return UT_GenStub_GetReturnValue(OS_stat, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_write()
 * ----------------------------------------------------
 */
int32 OS_write(osal_id_t filedes, const void *buffer, size_t nbytes)
{
    UT_GenStub_SetupReturnBuffer(OS_write, int32);

    UT_GenStub_AddParam(OS_write, osal_id_t, filedes);
    UT_GenStub_AddParam(OS_write, const void *, buffer);
    UT_GenStub_AddParam(OS_write, size_t, nbytes);

    UT_GenStub_Execute(OS_write, Basic, UT_DefaultHandler_OS_write);

    return UT_GenStub_GetReturnValue(OS_write, int32);
}
