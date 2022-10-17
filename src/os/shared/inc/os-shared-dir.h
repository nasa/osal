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
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_DIR_H
#define OS_SHARED_DIR_H

#include "osapi-dir.h"
#include "os-shared-globaldefs.h"

/* directory objects */
typedef struct
{
    char dir_name[OS_MAX_PATH_LEN];
} OS_dir_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_dir_internal_record_t OS_dir_table[OS_MAX_NUM_OPEN_DIRS];

/*
 * Directory API abstraction layer
 *
 */

/*---------------------------------------------------------------------------------------
   Name: OS_DirAPI_Init

   Purpose: Initialize the OS-independent layer for directory resources

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_DirAPI_Init(void);

/*----------------------------------------------------------------

    Purpose: Create a directory in the local filesystem

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirCreate_Impl(const char *local_path, uint32 access);

/*----------------------------------------------------------------

    Purpose: Open a directory and prepare to read the entries

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirOpen_Impl(const OS_object_token_t *token, const char *local_path);

/*----------------------------------------------------------------

    Purpose: Close a directory

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirClose_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Read the next entry from a directory handle

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirRead_Impl(const OS_object_token_t *token, os_dirent_t *dirent);

/*----------------------------------------------------------------

    Purpose: Rewind a directory handle back to the start

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirRewind_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------

    Purpose: Remove a directory in the local filesystem

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirRemove_Impl(const char *local_path);

#endif /* OS_SHARED_DIR_H */
