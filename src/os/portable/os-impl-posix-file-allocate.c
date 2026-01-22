/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_FileAllocate_Impl(const OS_object_token_t *token, osal_offset_t offset, osal_offset_t len)
{
    OS_impl_file_internal_record_t *impl;
    int32                           Status;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    if (posix_fallocate(impl->fd, offset, len) >= 0)
    {
        Status = OS_SUCCESS;
    }
    else if (errno == EOPNOTSUPP)
    {
        Status = OS_ERR_OPERATION_NOT_SUPPORTED;
    }
    else if (errno == EFBIG || errno == ENOSPC)
    {
        Status = OS_ERR_OUTPUT_TOO_LARGE;
    }
    else if (errno == ESPIPE || errno == ENODEV)
    {
        Status = OS_ERR_INCORRECT_OBJ_TYPE;
    }
    else
    {
        Status = OS_ERROR;
    }

    return Status;
}
