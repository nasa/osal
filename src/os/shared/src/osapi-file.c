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
 * \file     osapi-file.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "os-shared-file.h"
#include "os-shared-idmap.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_NUM_OPEN_FILES) || (OS_MAX_NUM_OPEN_FILES <= 0)
#error "osconfig.h must define OS_MAX_NUM_OPEN_FILES to a valid value"
#endif

/*
 * Global data for the API
 */
enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_NUM_OPEN_FILES,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_STREAM
};

OS_stream_internal_record_t OS_stream_table[OS_MAX_NUM_OPEN_FILES];

/****************************************************************************************
                                  FILE API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_FileAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_FileAPI_Init(void)
{
    memset(OS_stream_table, 0, sizeof(OS_stream_table));
    return OS_SUCCESS;
} /* end OS_FileAPI_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_OpenCreate
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Implements both "open" and "creat" file operations
 *           (The difference is a matter of what flags are passed in)
 *
 *-----------------------------------------------------------------*/
int32 OS_OpenCreate(osal_id_t *filedes, const char *path, int32 flags, int32 access)
{
    int32                        return_code;
    char                         local_path[OS_MAX_LOCAL_PATH_LEN];
    OS_object_token_t            token;
    OS_stream_internal_record_t *stream;

    if (filedes == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
    ** Check for a valid access mode
    */
    if (access != OS_WRITE_ONLY && access != OS_READ_ONLY && access != OS_READ_WRITE)
    {
        return OS_ERROR;
    }

    /*
     * Translate the path
     */
    return_code = OS_TranslatePath(path, (char *)local_path);

    if (return_code == OS_SUCCESS)
    {
        /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
        return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, NULL, &token);
        if (return_code == OS_SUCCESS)
        {
            stream = OS_OBJECT_TABLE_GET(OS_stream_table, token);

            /* Reset the table entry and save the name */
            OS_OBJECT_INIT(token, stream, stream_name, path);

            /* Now call the OS-specific implementation.  */
            return_code = OS_FileOpen_Impl(&token, local_path, flags, access);

            /* Check result, finalize record, and unlock global table. */
            return_code = OS_ObjectIdFinalizeNew(return_code, &token, filedes);
        }
    }

    return return_code;
} /* end OS_OpenCreate */

/*
 * The OS_open and OS_creat functions are deprecated, replaced by
 * the generic OS_OpenCreate above
 */
#ifndef OSAL_OMIT_DEPRECATED

/*----------------------------------------------------------------
 *
 * Function: OS_creat
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_creat(const char *path, int32 access)
{
    osal_id_t filedes;
    int32     return_code;

    /*
    ** Check for a valid access mode
    */
    switch (access)
    {
        case OS_WRITE_ONLY:
        case OS_READ_WRITE:
            break;
        case OS_READ_ONLY:
        default:
            /* Read only does not make sense for creat() */
            return OS_ERROR;
    }

    return_code = OS_OpenCreate(&filedes, path, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, access);
    if (return_code == OS_SUCCESS)
    {
        /* for backward compatibility, on success return the ID as an int32.
         * This will always within the positive range */
        return_code = (int32)OS_ObjectIdToInteger(filedes);
    }

    return return_code;
} /* end OS_creat */

/*----------------------------------------------------------------
 *
 * Function: OS_open
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_open(const char *path, int32 access, uint32 mode)
{
    osal_id_t filedes;
    int32     return_code;

    /*
    ** Check for a valid access mode
    */
    switch (access)
    {
        case OS_WRITE_ONLY:
        case OS_READ_WRITE:
        case OS_READ_ONLY:
            break;
        default:
            return OS_ERROR;
    }

    return_code = OS_OpenCreate(&filedes, path, OS_FILE_FLAG_NONE, access);
    if (return_code == OS_SUCCESS)
    {
        /* for backward compatibility, on success return the ID as an int32.
         * This will always within the positive range */
        return_code = (int32)OS_ObjectIdToInteger(filedes);
    }

    return return_code;
} /* end OS_open */

#endif

/*----------------------------------------------------------------
 *
 * Function: OS_close
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_close(osal_id_t filedes)
{
    OS_object_token_t token;
    int32             return_code;

    /* Make sure the file descriptor is legit before using it */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, filedes, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_GenericClose_Impl(&token);

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }

    return return_code;

} /* end OS_close */

/*----------------------------------------------------------------
 *
 * Function: OS_TimedRead
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimedRead(osal_id_t filedes, void *buffer, size_t nbytes, int32 timeout)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    if (buffer == NULL || nbytes == 0)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, filedes, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_GenericRead_Impl(&token, buffer, nbytes, timeout);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_TimedRead */

/*----------------------------------------------------------------
 *
 * Function: OS_TimedWrite
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TimedWrite(osal_id_t filedes, const void *buffer, size_t nbytes, int32 timeout)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    if (buffer == NULL || nbytes == 0)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, filedes, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_GenericWrite_Impl(&token, buffer, nbytes, timeout);
        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_TimedWrite */

/*----------------------------------------------------------------
 *
 * Function: OS_read
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_read(osal_id_t filedes, void *buffer, size_t nbytes)
{
    return OS_TimedRead(filedes, buffer, nbytes, OS_PEND);
} /* end OS_read */

/*----------------------------------------------------------------
 *
 * Function: OS_write
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_write(osal_id_t filedes, const void *buffer, size_t nbytes)
{
    return OS_TimedWrite(filedes, buffer, nbytes, OS_PEND);
} /* end OS_write */

/*----------------------------------------------------------------
 *
 * Function: OS_chmod
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_chmod(const char *path, uint32 access)
{
    char  local_path[OS_MAX_LOCAL_PATH_LEN];
    int32 return_code;

    return_code = OS_TranslatePath(path, local_path);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_FileChmod_Impl(local_path, access);
    }

    return return_code;

} /* end OS_chmod */

/*----------------------------------------------------------------
 *
 * Function: OS_stat
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_stat(const char *path, os_fstat_t *filestats)
{
    int32 return_code;
    char  local_path[OS_MAX_LOCAL_PATH_LEN];

    if (filestats == NULL)
    {
        return OS_INVALID_POINTER;
    }

    memset(filestats, 0, sizeof(*filestats));

    return_code = OS_TranslatePath(path, local_path);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_FileStat_Impl(local_path, filestats);
    }

    return return_code;
} /* end OS_stat */

/*----------------------------------------------------------------
 *
 * Function: OS_lseek
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_lseek(osal_id_t filedes, int32 offset, uint32 whence)
{
    OS_object_token_t token;
    int32             return_code;

    /* Make sure the file descriptor is legit before using it */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, filedes, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_GenericSeek_Impl(&token, offset, whence);
        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_lseek */

/*----------------------------------------------------------------
 *
 * Function: OS_remove
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_remove(const char *path)
{
    int32 return_code;
    char  local_path[OS_MAX_LOCAL_PATH_LEN];

    return_code = OS_TranslatePath(path, local_path);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_FileRemove_Impl(local_path);
    }

    return return_code;

} /* end OS_remove */

/*----------------------------------------------------------------
 *
 * Function: OS_rename
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_rename(const char *old, const char *new)
{
    OS_object_iter_t             iter;
    OS_stream_internal_record_t *stream;
    int32                        return_code;
    char                         old_path[OS_MAX_LOCAL_PATH_LEN];
    char                         new_path[OS_MAX_LOCAL_PATH_LEN];

    return_code = OS_TranslatePath(old, old_path);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_TranslatePath(new, new_path);
    }

    if (return_code == OS_SUCCESS)
    {
        return_code = OS_FileRename_Impl(old_path, new_path);
    }

    if (return_code == OS_SUCCESS)
    {
        OS_ObjectIdIterateActive(LOCAL_OBJID_TYPE, &iter);

        while (OS_ObjectIdIteratorGetNext(&iter))
        {
            stream = OS_OBJECT_TABLE_GET(OS_stream_table, iter.token);

            if (stream->socket_domain == OS_SocketDomain_INVALID && strcmp(stream->stream_name, old) == 0)
            {
                strcpy(stream->stream_name, new);
            }
        }

        OS_ObjectIdIteratorDestroy(&iter);
    }

    return return_code;

} /* end OS_rename */

/*----------------------------------------------------------------
 *
 * Function: OS_cp
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_cp(const char *src, const char *dest)
{
    int32     return_code;
    int32     rd_size;
    int32     wr_size;
    int32     wr_total;
    osal_id_t file1;
    osal_id_t file2;
    uint8     copyblock[512];

    if (src == NULL || dest == NULL)
    {
        return OS_INVALID_POINTER;
    }

    file1       = OS_OBJECT_ID_UNDEFINED;
    file2       = OS_OBJECT_ID_UNDEFINED;
    return_code = OS_OpenCreate(&file1, src, OS_FILE_FLAG_NONE, OS_READ_ONLY);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_OpenCreate(&file2, dest, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY);
    }

    while (return_code == OS_SUCCESS)
    {
        rd_size = OS_read(file1, copyblock, sizeof(copyblock));
        if (rd_size < 0)
        {
            return_code = rd_size;
            break;
        }
        if (rd_size == 0)
        {
            break;
        }
        wr_total = 0;
        while (wr_total < rd_size)
        {
            wr_size = OS_write(file2, &copyblock[wr_total], rd_size - wr_total);
            if (wr_size < 0)
            {
                return_code = wr_size;
                break;
            }
            wr_total += wr_size;
        }
    }

    if (OS_ObjectIdDefined(file1))
    {
        OS_close(file1);
    }
    if (OS_ObjectIdDefined(file2))
    {
        OS_close(file2);
    }

    return return_code;

} /* end OS_cp */

/*----------------------------------------------------------------
 *
 * Function: OS_mv
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_mv(const char *src, const char *dest)
{
    int32 return_code;

    /* First try rename - this only works if it is on the same filesystem */
    return_code = OS_rename(src, dest);
    if (return_code != OS_SUCCESS)
    {
        return_code = OS_cp(src, dest);
        if (return_code == OS_SUCCESS)
        {
            OS_remove(src);
        }
    }

    return (return_code);

} /* end OS_mv */

/*----------------------------------------------------------------
 *
 * Function: OS_FDGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FDGetInfo(osal_id_t filedes, OS_file_prop_t *fd_prop)
{
    OS_common_record_t *record;
    OS_object_token_t   token;
    int32               return_code;

    /* Check parameters */
    if (fd_prop == NULL)
    {
        return (OS_INVALID_POINTER);
    }

    memset(fd_prop, 0, sizeof(OS_file_prop_t));

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, filedes, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_stream_table, token);

        strncpy(fd_prop->Path, record->name_entry, OS_MAX_PATH_LEN - 1);
        fd_prop->User    = record->creator;
        fd_prop->IsValid = true;

        OS_ObjectIdRelease(&token);
    }

    return return_code;

} /* end OS_FDGetInfo */

/*----------------------------------------------------------------
 *
 * Function: OS_FileOpenCheck
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileOpenCheck(const char *Filename)
{
    int32                        return_code;
    OS_object_iter_t             iter;
    OS_stream_internal_record_t *stream;

    if (Filename == NULL)
    {
        return (OS_INVALID_POINTER);
    }

    return_code = OS_ERROR;

    OS_ObjectIdIterateActive(LOCAL_OBJID_TYPE, &iter);

    while (OS_ObjectIdIteratorGetNext(&iter))
    {
        stream = OS_OBJECT_TABLE_GET(OS_stream_table, iter.token);
        if (stream->socket_domain == OS_SocketDomain_INVALID && (strcmp(stream->stream_name, Filename) == 0))
        {
            return_code = OS_SUCCESS;
            break;
        }
    }

    OS_ObjectIdIteratorDestroy(&iter);

    return return_code;
} /* end OS_FileOpenCheck */

/*----------------------------------------------------------------
 *
 * Function: OS_CloseFileByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CloseFileByName(const char *Filename)
{
    int32                        return_code;
    int32                        close_code;
    OS_object_iter_t             iter;
    OS_stream_internal_record_t *stream;

    if (Filename == NULL)
    {
        return (OS_INVALID_POINTER);
    }

    return_code = OS_FS_ERR_PATH_INVALID;

    OS_ObjectIdIterateActive(LOCAL_OBJID_TYPE, &iter);

    while (OS_ObjectIdIteratorGetNext(&iter))
    {
        stream = OS_OBJECT_TABLE_GET(OS_stream_table, iter.token);

        if (stream->socket_domain == OS_SocketDomain_INVALID && (strcmp(stream->stream_name, Filename) == 0))
        {
            /* call OS_close() on the entry referred to by the iterator */
            close_code = OS_ObjectIdIteratorProcessEntry(&iter, OS_close);

            if (return_code == OS_FS_ERR_PATH_INVALID || close_code != OS_SUCCESS)
            {
                return_code = close_code;
            }
        }
    }

    OS_ObjectIdIteratorDestroy(&iter);

    return (return_code);

} /* end OS_CloseFileByName */

/*----------------------------------------------------------------
 *
 * Function: OS_CloseAllFiles
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_CloseAllFiles(void)
{
    int32            return_code;
    int32            close_code;
    OS_object_iter_t iter;

    return_code = OS_SUCCESS;

    OS_ObjectIdIterateActive(LOCAL_OBJID_TYPE, &iter);

    while (OS_ObjectIdIteratorGetNext(&iter))
    {
        /* call OS_close() on the entry referred to by the iterator */
        close_code = OS_ObjectIdIteratorProcessEntry(&iter, OS_close);
        if (close_code != OS_SUCCESS)
        {
            return_code = close_code;
        }
    }

    OS_ObjectIdIteratorDestroy(&iter);

    return (return_code);

} /* end OS_CloseAllFiles */
