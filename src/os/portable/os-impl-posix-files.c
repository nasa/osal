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
 * \file
 * \author joseph.p.hickey@nasa.gov
 *
 * This file Contains all of the api calls for manipulating files
 * in a file system / C library that implements the POSIX-style file API
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must include whatever is required to get the prototypes of these functions:
 *
 *   open()
 *   stat()
 *   chmod()
 *   remove()
 *   rename()
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "os-impl-files.h"
#include "os-shared-file.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                 Named File API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileOpen_Impl(const OS_object_token_t *token, const char *local_path, int32 flags, int32 access_mode)
{
    int                             os_perm;
    int                             os_mode;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    /*
    ** Check for a valid access mode
    ** For creating a file, OS_READ_ONLY does not make sense
    */
    switch (access_mode)
    {
        case OS_WRITE_ONLY:
            os_perm = O_WRONLY;
            break;
        case OS_READ_ONLY:
            os_perm = O_RDONLY;
            break;
        case OS_READ_WRITE:
            os_perm = O_RDWR;
            break;
        default:
            return OS_ERROR;
    }

    if (flags & OS_FILE_FLAG_CREATE)
    {
        os_perm |= O_CREAT;
    }
    if (flags & OS_FILE_FLAG_TRUNCATE)
    {
        os_perm |= O_TRUNC;
    }

    os_perm |= OS_IMPL_REGULAR_FILE_FLAGS;

    os_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    impl->fd = open(local_path, os_perm, os_mode);

    if (impl->fd < 0)
    {
        OS_DEBUG("open(%s): %s\n", local_path, strerror(errno));
        return OS_ERROR;
    }

    /*
     * If the flags included O_NONBLOCK, then
     * enable the "select" call on this handle.
     */
    impl->selectable = ((os_perm & O_NONBLOCK) != 0);

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileStat_Impl(const char *local_path, os_fstat_t *FileStats)
{
    struct stat     st;
    mode_t          readbits;
    mode_t          writebits;
    mode_t          execbits;
    struct timespec filetime;

    if (stat(local_path, &st) < 0)
    {
        return OS_ERROR;
    }

    FileStats->FileSize = st.st_size;

    /*
     * NOTE: Traditional timestamps are only a whole number of seconds (time_t)
     * POSIX.1-2008 expands this to have a full "struct timespec" with nanosecond
     * resolution.
     *
     * GLIBC (and likely other C libraries that use similar feature selection)
     * will expose this value based on _POSIX_C_SOURCE or _XOPEN_SOURCE minimum
     * values.  Otherwise this just falls back to standard 1-second resolution
     * available via the "st_mtime" member.
     */
#if (_POSIX_C_SOURCE >= 200809L) || (_XOPEN_SOURCE >= 700)
    /*
     * Better - use the full resolution (seconds + nanoseconds) as specified in POSIX.1-2008
     */
    filetime = st.st_mtim;
#else
    /*
     * Fallback - every POSIX-compliant implementation must expose "st_mtime" field.
     */
    filetime.tv_sec  = st.st_mtime;
    filetime.tv_nsec = 0;
#endif

    FileStats->FileTime = OS_TimeAssembleFromNanoseconds(filetime.tv_sec, filetime.tv_nsec);

    /* note that the "fst_mode" member is already zeroed by the caller */
    if (S_ISDIR(st.st_mode))
    {
        FileStats->FileModeBits |= OS_FILESTAT_MODE_DIR;
    }

    /* always check world bits */
    readbits  = S_IROTH;
    writebits = S_IWOTH;
    execbits  = S_IXOTH;

    if (OS_IMPL_SELF_EUID == st.st_uid)
    {
        /* we own the file so use user bits for simplified perms */
        readbits |= S_IRUSR;
        writebits |= S_IWUSR;
        execbits |= S_IXUSR;
    }

    if (OS_IMPL_SELF_EGID == st.st_gid)
    {
        /* our group owns the file so use group bits for simplified perms */
        readbits |= S_IRGRP;
        writebits |= S_IWGRP;
        execbits |= S_IXGRP;
    }

    if (st.st_mode & readbits)
    {
        FileStats->FileModeBits |= OS_FILESTAT_MODE_READ;
    }
    if (st.st_mode & writebits)
    {
        FileStats->FileModeBits |= OS_FILESTAT_MODE_WRITE;
    }
    if (st.st_mode & execbits)
    {
        FileStats->FileModeBits |= OS_FILESTAT_MODE_EXEC;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileChmod_Impl(const char *local_path, uint32 access_mode)
{
    mode_t      readbits;
    mode_t      writebits;
    struct stat st;
    int         fd;
    int32       status;

    /* Open file to avoid filename race potential */
    fd = open(local_path, O_RDONLY, 0);
    if (fd < 0)
    {
        fd = open(local_path, O_WRONLY, 0);
        if (fd < 0)
        {
            OS_DEBUG("open(%s): %s (%d)\n", local_path, strerror(errno), errno);
            return OS_ERROR;
        }
    }

    /*
     * NOTE: After this point, execution must proceed to the end of this routine
     * so that the "fd" opened above can be properly closed.
     */

    /*
     * In order to preserve any OTHER mode bits,
     * first stat() the file and then modify the st_mode
     * to match the desired access level.
     *
     * In particular, this should preserve the execute bit,
     * which is generally not part of the OSAL API, but
     * is important for the underlying OS.
     */
    if (fstat(fd, &st) < 0)
    {
        OS_DEBUG("fstat(%s): %s (%d)\n", local_path, strerror(errno), errno);
        status = OS_ERROR;
    }
    else
    {
        /* always check world bits */
        readbits  = S_IROTH;
        writebits = S_IWOTH;

        if (OS_IMPL_SELF_EUID == st.st_uid)
        {
            /* we own the file so use user bits */
            readbits |= S_IRUSR;
            writebits |= S_IWUSR;
        }

        if (OS_IMPL_SELF_EGID == st.st_gid)
        {
            /* our group owns the file so use group bits */
            readbits |= S_IRGRP;
            writebits |= S_IWGRP;
        }

        if (access_mode == OS_WRITE_ONLY || access_mode == OS_READ_WRITE)
        {
            /* set all "write" mode bits */
            st.st_mode |= writebits;
        }
        else
        {
            /* clear all "write" mode bits */
            st.st_mode &= ~writebits;
        }

        if (access_mode == OS_READ_ONLY || access_mode == OS_READ_WRITE)
        {
            /* set all "read" mode bits */
            st.st_mode |= readbits;
        }
        else
        {
            /* clear all "read" mode bits */
            st.st_mode &= ~readbits;
        }

        /* finally, write the modified mode back to the file */
        if (fchmod(fd, st.st_mode) < 0)
        {
            /*
             * These particular errnos generally indicate that the
             * underlying filesystem does not support chmod()
             *
             * This is often the case for FAT / DOSFS filesystems
             * which do not have UNIX-style permissions, or (in the
             * case of EROFS) if the filesystem is mounted read-only.
             */
            if (errno == ENOTSUP || errno == ENOSYS || errno == EROFS)
            {
                status = OS_ERR_NOT_IMPLEMENTED;
            }
            else
            {
                OS_DEBUG("fchmod(%s): %s (%d)\n", local_path, strerror(errno), errno);
                status = OS_ERROR;
            }
        }
        else
        {
            status = OS_SUCCESS;
        }
    }

    close(fd);

    return status;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileRemove_Impl(const char *local_path)
{
    if (remove(local_path) < 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileRename_Impl(const char *old_path, const char *new_path)
{
    if (rename(old_path, new_path) < 0)
    {
        return OS_ERROR;
    }

    return OS_SUCCESS;
}
