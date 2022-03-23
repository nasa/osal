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
   Function: OS_DirCreate_Impl

    Purpose: Create a directory in the local filesystem

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirCreate_Impl(const char *local_path, uint32 access);

/*----------------------------------------------------------------
   Function: OS_DirOpen_Impl

    Purpose: Open a directory and prepare to read the entries

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirOpen_Impl(const OS_object_token_t *token, const char *local_path);

/*----------------------------------------------------------------
   Function: OS_DirClose_Impl

    Purpose: Close a directory

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirClose_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_DirRead_Impl

    Purpose: Read the next entry from a directory handle

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirRead_Impl(const OS_object_token_t *token, os_dirent_t *dirent);

/*----------------------------------------------------------------
   Function: OS_DirRewind_Impl

    Purpose: Rewind a directory handle back to the start

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirRewind_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_DirRemove_Impl

    Purpose: Remove a directory in the local filesystem

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_DirRemove_Impl(const char *local_path);

#endif /* OS_SHARED_DIR_H */
