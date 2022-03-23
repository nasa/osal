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
 *
 * Declarations and prototypes for directories
 */

#ifndef OSAPI_DIR_H
#define OSAPI_DIR_H

#include "osconfig.h"
#include "common_types.h"

/** @brief Directory entry */
typedef struct
{
    char FileName[OS_MAX_FILE_NAME];
} os_dirent_t;

/** @brief Access filename part of the dirent structure */
#define OS_DIRENTRY_NAME(x) ((x).FileName)

/*
 * Exported Functions
 */

/** @defgroup OSAPIDir OSAL Directory APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Opens a directory
 *
 * Prepares for reading the files within a directory
 *
 * @param[out] dir_id    Location to store handle ID of the directory @nonnull
 * @param[in]  path      The directory to open @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if dir_id or path is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the path argument exceeds the maximum length
 * @retval #OS_FS_ERR_PATH_INVALID if the path argument is not valid
 * @retval #OS_ERROR if the directory could not be opened
 */
int32 OS_DirectoryOpen(osal_id_t *dir_id, const char *path);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Closes an open directory
 *
 * The directory referred to by dir_id will be closed
 *
 * @param[in] dir_id    The handle ID of the directory
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the directory handle is invalid
 */
int32 OS_DirectoryClose(osal_id_t dir_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Rewinds an open directory
 *
 * Resets a directory read handle back to the first file.
 *
 * @param[in] dir_id    The handle ID of the directory
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_ID if the directory handle is invalid
 */
int32 OS_DirectoryRewind(osal_id_t dir_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Reads the next name in the directory
 *
 * Obtains directory entry data for the next file from an open directory
 *
 * @param[in]  dir_id    The handle ID of the directory
 * @param[out] dirent    Buffer to store directory entry information @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if dirent argument is NULL
 * @retval #OS_ERR_INVALID_ID if the directory handle is invalid
 * @retval #OS_ERROR at the end of the directory or if the OS call otherwise fails
 */
int32 OS_DirectoryRead(osal_id_t dir_id, os_dirent_t *dirent);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Makes a new directory
 *
 * Makes a directory specified by path.
 *
 * @param[in]  path      The new directory name @nonnull
 * @param[in]  access    The permissions for the directory (reserved for future use)
 *
 * @note Current implementations do not utilize the "access" parameter.  Applications
 * should still pass the intended value (#OS_READ_WRITE or #OS_READ_ONLY) to be compatible
 * with future implementations.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if path is NULL
 * @retval #OS_FS_ERR_PATH_TOO_LONG if the path is too long to be stored locally
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_ERROR if the OS call fails @covtest
 */
int32 OS_mkdir(const char *path, uint32 access);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Removes a directory from the file system.
 *
 * Removes a directory from the structure.
 * The directory must be empty prior to this operation.
 *
 * @param[in]  path      The directory to remove
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if path is NULL
 * @retval #OS_FS_ERR_PATH_INVALID if path cannot be parsed
 * @retval #OS_FS_ERR_PATH_TOO_LONG
 * @retval #OS_ERROR if the directory remove operation failed @covtest
 */
int32 OS_rmdir(const char *path);
/**@}*/

#endif /* OSAPI_DIR_H */
