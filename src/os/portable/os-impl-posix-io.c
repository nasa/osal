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
 * \file   os-impl-posix-io.c
 * \author joseph.p.hickey@nasa.gov
 *
 * This file contains generic calls for manipulating filehandles
 * in a file system / C library that implements the UNIX-style file API
 *
 * These generic ops may apply to regular files, sockets, pipes, or
 * special devices, depending on the OS in use.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must include whatever is required to get the prototypes of these functions:
 *
 *   read()
 *   write()
 *   close()
 */
#include <string.h>
#include <errno.h>

#include "os-impl-io.h"
#include "os-shared-file.h"
#include "os-shared-select.h"
#include "os-shared-idmap.h"

/* some OS libraries (e.g. VxWorks) do not declare the API to be const-correct
 * It can still use this generic implementation but the call to write() must be
 * cast to a void* to avoid a warning.  The includer can define this if needed.
 * If not defined, assume no cast is needed (fine for anything POSIX-compliant).
 */
#ifndef GENERIC_IO_CONST_DATA_CAST
#define GENERIC_IO_CONST_DATA_CAST
#endif

/*----------------------------------------------------------------
 *
 * Function: OS_GenericClose_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericClose_Impl(const OS_object_token_t *token)
{
    int                             result;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    result = close(impl->fd);
    if (result < 0)
    {
        /*
         * close() can technically fail for various reasons, but
         * there isn't much recourse if this call fails.  Just log
         * the failure for debugging.
         *
         * POSIX also does not specify the state of the filehandle
         * after a close() with an error.
         *
         * At least in  Linux/glibc the filehandle is always closed
         * in the kernel and should not be used again or re-closed.
         */
        OS_DEBUG("close: %s\n", strerror(errno));
    }
    impl->fd = -1;
    return OS_SUCCESS;
} /* end OS_GenericClose_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_GenericSeek_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericSeek_Impl(const OS_object_token_t *token, int32 offset, uint32 whence)
{
    int                             where;
    off_t                           os_result;
    int32                           retval;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    switch (whence)
    {
        case OS_SEEK_SET:
            where = SEEK_SET;
            break;
        case OS_SEEK_CUR:
            where = SEEK_CUR;
            break;
        case OS_SEEK_END:
            where = SEEK_END;
            break;
        default:
            return OS_ERROR;
    }

    os_result = lseek(impl->fd, (off_t)offset, where);
    if (os_result == (off_t)-1)
    {
        if (errno == ESPIPE)
        {
            /*
             * this means the user tried to seek on a pipe, socket,
             * or other fifo-like handle that doesn't support seeking.
             *
             * Use a different error code to differentiate from an
             * error involving a bad whence/offset
             */
            retval = OS_ERR_NOT_IMPLEMENTED;
        }
        else
        {
            /*
             * Most likely the "whence" and/or "offset" combo was not valid.
             */
            OS_DEBUG("lseek: %s\n", strerror(errno));
            retval = OS_ERROR;
        }
    }
    else
    {
        /*
         * convert value to int32 type for returning to caller.
         * Note that this could potentially overflow an int32
         * for a large file seek.
         */
        retval = (int32)os_result;
    }

    return retval;
} /* end OS_GenericSeek_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_GenericRead_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericRead_Impl(const OS_object_token_t *token, void *buffer, size_t nbytes, int32 timeout)
{
    int32                           return_code;
    ssize_t                         os_result;
    uint32                          operation;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    return_code = OS_SUCCESS;

    if (nbytes > 0)
    {
        operation = OS_STREAM_STATE_READABLE;

        /*
         * If filehandle is set with O_NONBLOCK, then must call select() here.
         *
         * The "selectable" field should be set false for those file handles
         * which the underlying OS does not support select() on.
         *
         * Note that a timeout will not work unless selectable is true.
         */
        if (impl->selectable)
        {
            return_code = OS_SelectSingle_Impl(token, &operation, timeout);
        }

        if (return_code == OS_SUCCESS && (operation & OS_STREAM_STATE_READABLE) != 0)
        {
            os_result = read(impl->fd, buffer, nbytes);
            if (os_result < 0)
            {
                OS_DEBUG("read: %s\n", strerror(errno));
                return_code = OS_ERROR;
            }
            else
            {
                /* type conversion from ssize_t to int32 for return */
                return_code = (int32)os_result;
            }
        }
    }

    return (return_code);
} /* end OS_GenericRead_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_GenericWrite_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GenericWrite_Impl(const OS_object_token_t *token, const void *buffer, size_t nbytes, int32 timeout)
{
    int32                           return_code;
    ssize_t                         os_result;
    uint32                          operation;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    return_code = OS_SUCCESS;

    if (nbytes > 0)
    {
        operation = OS_STREAM_STATE_WRITABLE;

        /*
         * If filehandle is set with O_NONBLOCK, then must call select() here.
         *
         * The "selectable" field should be set false for those file handles
         * which the underlying OS does not support select() on.
         *
         * Note that a timeout will not work unless selectable is true.
         */
        if (impl->selectable)
        {
            return_code = OS_SelectSingle_Impl(token, &operation, timeout);
        }

        if (return_code == OS_SUCCESS && (operation & OS_STREAM_STATE_WRITABLE) != 0)
        {
            /* on some system libraries for which the write() argument is not
             * qualified correctly, it needs to be case to a void* here */
            os_result = write(impl->fd, GENERIC_IO_CONST_DATA_CAST buffer, nbytes);
            if (os_result < 0)
            {
                OS_DEBUG("write: %s\n", strerror(errno));
                return_code = OS_ERROR;
            }
            else
            {
                /* type conversion from ssize_t to int32 for return */
                return_code = (int32)os_result;
            }
        }
    }

    return (return_code);
} /* end OS_GenericWrite_Impl */
