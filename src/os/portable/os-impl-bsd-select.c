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
 * Purpose: This file contains wrappers around the select() system call
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/*
 * Inclusions Defined by OSAL layer.
 *
 * This must include whatever is required to get the prototypes of these functions:
 *
 *   FD_SET/FD_CLR/FD_ISSET macros and fd_set typedef
 *   select()
 *   clock_gettime() - for computing select timeouts
 */
#include <string.h>
#include <errno.h>
#include <time.h>

#include "os-impl-select.h"
#include "os-shared-clock.h"
#include "os-shared-select.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/***************************************************************************************
                                 FUNCTION PROTOTYPES
 **************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/****************************************************************************************
                                LOCAL FUNCTIONS
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *          Convert an OS_FdSet (OSAL) structure into an fd_set (POSIX)
 *          which can then be passed to the POSIX select function.
 *
 * returns: Highest numbered file descriptor in the output fd_set
 *-----------------------------------------------------------------*/
static int32 OS_FdSet_ConvertIn_Impl(int *os_maxfd, fd_set *os_set, const OS_FdSet *OSAL_set)
{
    size_t       offset;
    size_t       bit;
    osal_index_t id;
    uint8        objids;
    int          osfd;
    int32        status;

    status = OS_SUCCESS;
    for (offset = 0; offset < sizeof(OSAL_set->object_ids); ++offset)
    {
        objids = OSAL_set->object_ids[offset];
        bit    = 0;
        while (objids != 0)
        {
            id = OSAL_INDEX_C((offset * 8) + bit);
            if ((objids & 0x01) != 0 && id < OS_MAX_NUM_OPEN_FILES)
            {
                osfd = OS_impl_filehandle_table[id].fd;
                if (osfd >= 0)
                {
                    if (osfd >= FD_SETSIZE || !OS_impl_filehandle_table[id].selectable)
                    {
                        /* out of range of select() implementation */
                        status = OS_ERR_OPERATION_NOT_SUPPORTED;
                    }
                    else
                    {
                        FD_SET(osfd, os_set);
                        if (osfd > *os_maxfd)
                        {
                            *os_maxfd = osfd;
                        }
                    }
                }
            }
            ++bit;
            objids >>= 1;
        }
    }

    return status;
}

/*----------------------------------------------------------------*/
/**
 * \brief Convert a POSIX fd_set structure into an OSAL OS_FdSet
 *        which can then be returned back to the application.
 *
 * Local helper routine, not part of OSAL API.
 *
 * This un-sets bits in OSAL_set that are set in the OS_set
 *
 * \param[in]     OS_set   The fd_set from select
 * \param[in,out] OSAL_set The OS_FdSet updated by this helper
 */
/*-----------------------------------------------------------------*/
static void OS_FdSet_ConvertOut_Impl(fd_set *OS_set, OS_FdSet *OSAL_set)
{
    size_t       offset;
    size_t       bit;
    osal_index_t id;
    uint8        objids;
    int          osfd;

    for (offset = 0; offset < sizeof(OSAL_set->object_ids); ++offset)
    {
        objids = OSAL_set->object_ids[offset];
        bit    = 0;
        while (objids != 0)
        {
            id = OSAL_INDEX_C((offset * 8) + bit);
            if ((objids & 0x01) != 0 && id < OS_MAX_NUM_OPEN_FILES)
            {
                osfd = OS_impl_filehandle_table[id].fd;
                if (osfd < 0 || !FD_ISSET(osfd, OS_set))
                {
                    OSAL_set->object_ids[offset] &= ~(1 << bit);
                }
            }
            ++bit;
            objids >>= 1;
        }
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *          Actual implementation of select() call
 *          Used by SelectSingle and SelectMultiple implementations (below)
 *-----------------------------------------------------------------*/
static int32 OS_DoSelect(int maxfd, fd_set *rd_set, fd_set *wr_set, OS_time_t abs_timeout)
{
    int             os_status;
    int32           return_code;
    struct timeval  tv;
    struct timeval *tvptr;
    OS_time_t       curr_time;

    /* Implementations may pass OS_TIME_MAX to mean wait forever */
    if (OS_TimeEqual(abs_timeout, OS_TIME_MAX))
    {
        tvptr = NULL;
    }
    else
    {
        /* Not waiting forever, some form of timeout will be calculated */
        tvptr = &tv;
    }

    curr_time = abs_timeout;

    do
    {
        if (tvptr != NULL)
        {
            if (OS_TimeGetSign(abs_timeout) > 0)
            {
                OS_GetLocalTime_Impl(&curr_time);
                curr_time = OS_TimeSubtract(abs_timeout, curr_time);
            }
            if (OS_TimeGetSign(curr_time) <= 0)
            {
                /* timeout has already passed - this will still poll, but not block */
                tv.tv_sec  = 0;
                tv.tv_usec = 0;
            }
            else
            {
                /* timeout is in the future */
                tv.tv_sec  = OS_TimeGetTotalSeconds(curr_time);
                tv.tv_usec = OS_TimeGetMicrosecondsPart(curr_time);
            }
        }

        os_status = select(maxfd + 1, rd_set, wr_set, NULL, tvptr);

    } while (os_status < 0 && (errno == EINTR || errno == EAGAIN));

    if (os_status < 0)
    {
        return_code = OS_ERROR;
    }
    else if (os_status == 0)
    {
        return_code = OS_ERROR_TIMEOUT;
    }
    else
    {
        return_code = OS_SUCCESS;
    }

    return return_code;
}

/****************************************************************************************
                                SELECT API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectSingle_Impl(const OS_object_token_t *token, uint32 *SelectFlags, OS_time_t abs_timeout)
{
    int32                           return_code;
    fd_set                          wr_set;
    fd_set                          rd_set;
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    /*
     * If called on a stream_id which does not support this
     * operation, return immediately and do not invoke the system call
     */
    if (!impl->selectable)
    {
        return OS_ERR_OPERATION_NOT_SUPPORTED;
    }

    if (impl->fd >= FD_SETSIZE)
    {
        /* out of range of select() implementation */
        return OS_ERR_OPERATION_NOT_SUPPORTED;
    }

    if (*SelectFlags != 0)
    {
        FD_ZERO(&wr_set);
        FD_ZERO(&rd_set);
        if (*SelectFlags & OS_STREAM_STATE_READABLE)
        {
            FD_SET(impl->fd, &rd_set);
        }
        if (*SelectFlags & OS_STREAM_STATE_WRITABLE)
        {
            FD_SET(impl->fd, &wr_set);
        }

        return_code = OS_DoSelect(impl->fd, &rd_set, &wr_set, abs_timeout);

        if (return_code == OS_SUCCESS)
        {
            if (!FD_ISSET(impl->fd, &rd_set))
            {
                *SelectFlags &= ~OS_STREAM_STATE_READABLE;
            }
            if (!FD_ISSET(impl->fd, &wr_set))
            {
                *SelectFlags &= ~OS_STREAM_STATE_WRITABLE;
            }
        }
        else
        {
            *SelectFlags = 0;
        }
    }
    else
    {
        /* Nothing to check for, return immediately. */
        return_code = OS_SUCCESS;
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, OS_time_t abs_timeout)
{
    fd_set wr_set;
    fd_set rd_set;
    int    maxfd;
    int32  return_code;

    FD_ZERO(&rd_set);
    FD_ZERO(&wr_set);
    maxfd = -1;
    if (ReadSet != NULL)
    {
        return_code = OS_FdSet_ConvertIn_Impl(&maxfd, &rd_set, ReadSet);
        if (return_code != OS_SUCCESS)
        {
            return return_code;
        }
    }
    if (WriteSet != NULL)
    {
        return_code = OS_FdSet_ConvertIn_Impl(&maxfd, &wr_set, WriteSet);
        if (return_code != OS_SUCCESS)
        {
            return return_code;
        }
    }

    if (maxfd >= 0)
    {
        return_code = OS_DoSelect(maxfd, &rd_set, &wr_set, abs_timeout);
    }
    else
    {
        /*
         * This return code will be used if the set(s) were
         * both empty/NULL or otherwise did not contain valid filehandles.
         */
        return_code = OS_ERR_INVALID_ID;
    }

    if (return_code == OS_SUCCESS)
    {
        if (ReadSet != NULL)
        {
            OS_FdSet_ConvertOut_Impl(&rd_set, ReadSet);
        }
        if (WriteSet != NULL)
        {
            OS_FdSet_ConvertOut_Impl(&wr_set, WriteSet);
        }
    }

    return return_code;
}
