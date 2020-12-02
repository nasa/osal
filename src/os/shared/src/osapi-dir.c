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
 * \file     osapi-dir.c
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
#include "os-shared-dir.h"
#include "os-shared-idmap.h"

/*
 * Sanity checks on the user-supplied configuration
 * The relevent OS_MAX limit should be defined and greater than zero
 */
#if !defined(OS_MAX_NUM_OPEN_DIRS) || (OS_MAX_NUM_OPEN_DIRS <= 0)
#error "osconfig.h must define OS_MAX_NUM_OPEN_DIRS to a valid value"
#endif

/*
 * Global data for the API
 */
enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_NUM_OPEN_DIRS,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_DIR
};

OS_dir_internal_record_t OS_dir_table[LOCAL_NUM_OBJECTS];

/****************************************************************************************
                                  DIRECTORY API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_DirAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Init function for OS-independent layer
 *
 *-----------------------------------------------------------------*/
int32 OS_DirAPI_Init(void)
{
    memset(OS_dir_table, 0, sizeof(OS_dir_table));
    return OS_SUCCESS;
} /* end OS_DirAPI_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_mkdir
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_mkdir(const char *path, uint32 access)
{
    int32 return_code;
    char  local_path[OS_MAX_LOCAL_PATH_LEN];

    return_code = OS_TranslatePath(path, local_path);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_DirCreate_Impl(local_path, access);
    }

    return return_code;

} /* end OS_mkdir */

/*----------------------------------------------------------------
 *
 * Function: OS_DirectoryOpen
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirectoryOpen(osal_id_t *dir_id, const char *path)
{
    char                      local_path[OS_MAX_LOCAL_PATH_LEN];
    OS_object_token_t         token;
    OS_dir_internal_record_t *dir;
    int32                     return_code;

    if (dir_id == NULL || path == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_TranslatePath(path, local_path);
    if (return_code == OS_SUCCESS)
    {
        /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
        return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, NULL, &token);
        if (return_code == OS_SUCCESS)
        {
            dir = OS_OBJECT_TABLE_GET(OS_dir_table, token);

            /* Reset the table entry and save the name */
            OS_OBJECT_INIT(token, dir, dir_name, path);

            /* Now call the OS-specific implementation.  */
            return_code = OS_DirOpen_Impl(&token, local_path);

            /* Check result, finalize record, and unlock global table. */
            return_code = OS_ObjectIdFinalizeNew(return_code, &token, dir_id);
        }
    }

    return return_code;
} /* end OS_DirectoryOpen */

/*----------------------------------------------------------------
 *
 * Function: OS_DirectoryClose
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirectoryClose(osal_id_t dir_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Make sure the file descriptor is legit before using it */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, dir_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_DirClose_Impl(&token);

        /* Complete the operation via the common routine */
        return_code = OS_ObjectIdFinalizeDelete(return_code, &token);
    }

    return return_code;
} /* end OS_DirectoryClose */

/*----------------------------------------------------------------
 *
 * Function: OS_DirectoryRead
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirectoryRead(osal_id_t dir_id, os_dirent_t *dirent)
{
    OS_object_token_t token;
    int32             return_code;

    if (dirent == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /* Make sure the file descriptor is legit before using it */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, dir_id, &token);
    if (return_code == OS_SUCCESS)
    {
        /*
         * Call the underlying implementation to perform the read
         *
         * NOTE: This does not map "virtual mount points" that
         * may appear in the dir listing back to the virtualized
         * name.  For instance, if the (real) /eeprom directory
         * is virtualized to /cf via the OS_VolumeTable, and one
         * reads the "/" directory, the application will see the
         * real name (eeprom) and not the virtualized name (cf).
         */
        return_code = OS_DirRead_Impl(&token, dirent);

        OS_ObjectIdRelease(&token);
    }

    return return_code;

} /* end OS_DirectoryRead */

/*----------------------------------------------------------------
 *
 * Function: OS_DirectoryRewind
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_DirectoryRewind(osal_id_t dir_id)
{
    OS_object_token_t token;
    int32             return_code;

    /* Make sure the file descriptor is legit before using it */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, dir_id, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_DirRewind_Impl(&token);
    }

    return return_code;
} /* end OS_DirectoryRewind */

/*----------------------------------------------------------------
 *
 * Function: OS_rmdir
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_rmdir(const char *path)
{
    int32 return_code;
    char  local_path[OS_MAX_LOCAL_PATH_LEN];

    return_code = OS_TranslatePath(path, local_path);
    if (return_code == OS_SUCCESS)
    {
        OS_DirRemove_Impl(local_path);
    }

    return return_code;

} /* end OS_rmdir */
