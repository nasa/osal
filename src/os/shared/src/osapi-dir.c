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
   LOCAL_OBJID_TYPE = OS_OBJECT_TYPE_OS_DIR
};

OS_dir_internal_record_t    OS_dir_table          [LOCAL_NUM_OBJECTS];


#ifndef OSAL_OMIT_DEPRECATED
/*
 * A type safe(-ish) way of encoding the 32-bit dir ID inside a pointer.
 * This is because the old API used a DIR* object directly and this is intended
 * to retain compatibility with code written against that API.
 */
typedef union
{
   os_dirp_t dirp;
   uint32 dir_id;
} OS_Dirp_Xltr_t;

#endif


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
int32 OS_mkdir (const char *path, uint32 access)
{
   int32 return_code;
   char local_path[OS_MAX_LOCAL_PATH_LEN];

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
int32 OS_DirectoryOpen(uint32 *dir_id, const char *path)
{
    char local_path[OS_MAX_LOCAL_PATH_LEN];
    OS_common_record_t *record;
    uint32 local_id;
    int32 return_code;

    if (dir_id == NULL || path == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_TranslatePath(path, local_path);
    if (return_code == OS_SUCCESS)
    {
       /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
       return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, NULL, &local_id, &record);
       if(return_code == OS_SUCCESS)
       {
          /* Save all the data to our own internal table */
          memset(&OS_dir_table[local_id], 0, sizeof (OS_dir_internal_record_t));
          strncpy(OS_dir_table[local_id].dir_name, path, OS_MAX_PATH_LEN - 1);

          /* Now call the OS-specific implementation.  */
          return_code = OS_DirOpen_Impl(local_id, local_path);

          /* Check result, finalize record, and unlock global table. */
          return_code = OS_ObjectIdFinalizeNew(return_code, record, dir_id);
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
int32 OS_DirectoryClose(uint32 dir_id)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Make sure the file descriptor is legit before using it */
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_EXCLUSIVE, LOCAL_OBJID_TYPE, dir_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
       return_code = OS_DirClose_Impl(local_id);

       /* Complete the operation via the common routine */
       return_code = OS_ObjectIdFinalizeDelete(return_code, record);
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
int32 OS_DirectoryRead(uint32 dir_id, os_dirent_t *dirent)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   if (dirent == NULL)
   {
       return OS_INVALID_POINTER;
   }

   /* Make sure the file descriptor is legit before using it */
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, dir_id, &local_id, &record);
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
      return_code = OS_DirRead_Impl(local_id, dirent);

      OS_Unlock_Global(LOCAL_OBJID_TYPE);
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
int32 OS_DirectoryRewind(uint32 dir_id)
{
   OS_common_record_t *record;
   uint32 local_id;
   int32 return_code;

   /* Make sure the file descriptor is legit before using it */
   return_code = OS_ObjectIdGetById(OS_LOCK_MODE_NONE, LOCAL_OBJID_TYPE, dir_id, &local_id, &record);
   if (return_code == OS_SUCCESS)
   {
      return_code = OS_DirRewind_Impl(local_id);
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
int32  OS_rmdir (const char *path)
{
   int32 return_code;
   char local_path [OS_MAX_LOCAL_PATH_LEN];

   return_code = OS_TranslatePath(path, local_path);
   if (return_code == OS_SUCCESS)
   {
      OS_DirRemove_Impl(local_path);
   }

   return return_code;

} /* end OS_rmdir */

/*
 * Compatibility layers for old-style API
 */
#ifndef OSAL_OMIT_DEPRECATED

/*----------------------------------------------------------------

 * Function: OS_opendir
 *
 *  Purpose: Open a directory.  Deprecated function.
 *
 *-----------------------------------------------------------------*/
os_dirp_t OS_opendir (const char *path)
{
    OS_Dirp_Xltr_t  dirdescptr;

    dirdescptr.dirp = NULL;
    OS_DirectoryOpen(&dirdescptr.dir_id, path);

    return dirdescptr.dirp;
} /* end OS_opendir */


/*----------------------------------------------------------------
 *
 * Function: OS_closedir
 *
 *  Purpose: closes a directory.  Deprecated function.
 *
 *-----------------------------------------------------------------*/
int32 OS_closedir (os_dirp_t directory)
{
    OS_Dirp_Xltr_t  dirdescptr;

    if (directory == NULL)
    {
        return OS_INVALID_POINTER;
    }
    dirdescptr.dirp = directory;

    return (OS_DirectoryClose(dirdescptr.dir_id));
} /* end OS_closedir */

/*----------------------------------------------------------------
 *
 * Function: OS_readdir
 *
 *  Purpose: read a directory.  Deprecated function.
 *
 *  NOTE: this function is not thread-safe, and it cannot ever be thread-safe,
 *  which is one reason why this is deprecated.
 *
 *-----------------------------------------------------------------*/
os_dirent_t *OS_readdir (os_dirp_t directory)
{
    OS_Dirp_Xltr_t  dirdescptr;
    uint32 array_idx;
    os_dirent_t *tempptr;

    dirdescptr.dirp = directory;
    OS_ConvertToArrayIndex(dirdescptr.dir_id, &array_idx);

    tempptr = &OS_dir_table[array_idx].dirent_object;
    if (OS_DirectoryRead(dirdescptr.dir_id, tempptr) != OS_SUCCESS)
    {
        tempptr = NULL;
    }

    return tempptr;
} /* end OS_readdir */

/*----------------------------------------------------------------
 *
 * Function: OS_rewinddir
 *
 *  Purpose: Rewinds the directory pointer.  Deprecated Function.
 *
 *-----------------------------------------------------------------*/
void  OS_rewinddir (os_dirp_t directory )
{
    OS_Dirp_Xltr_t  dirdescptr;

    dirdescptr.dirp = directory;

    OS_DirectoryRewind(dirdescptr.dir_id);
} /* end OS_rewinddir */

#endif

