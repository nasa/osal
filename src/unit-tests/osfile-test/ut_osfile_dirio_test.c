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

/*================================================================================*
** File:  ut_osfile_dirio_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osfile_dirio_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_FILE_MAX_DIRS        5
#define UT_OS_FILE_NUM_DIR_ENTRIES 2

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern char *g_fsAddrPtr;

extern char g_longPathName[UT_OS_PATH_BUFF_SIZE];

extern char *g_mntName;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

char g_dirName[UT_OS_PATH_BUFF_SIZE];
char g_fileName[UT_OS_PATH_BUFF_SIZE];

char        g_subdirNames[UT_OS_FILE_MAX_DIRS][UT_OS_PATH_BUFF_SIZE];
const char *g_tgtSubdirs[UT_OS_FILE_NUM_DIR_ENTRIES] = {"subdir1", "subdir2"};

typedef struct
{
    char DirItem[UT_OS_FILE_BUFF_SIZE];
} UT_DirEntry_t;

char g_dirItems[UT_OS_FILE_NUM_DIR_ENTRIES][UT_OS_FILE_BUFF_SIZE];

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_read_n_sort_dirs(osal_id_t);

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_mkdir(const char *path, uint32 access)
** Purpose: Creates a directory specified by path
** Parameters: *path - pointer to the absolute pathname of the directory to be created
**             access - directory access mode (unused)
** Returns: OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
**          OS_INVALID_POINTER if the pointer passed in is null
**          OS_FS_ERR_PATH_TOO_LONG if the path is too long
**          OS_FS_ERR_PATH_INVALID if the path is invalid
**          OS_ERROR if the OS call failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with a really long path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #4: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Call this routine to create a directory under /cf mount point
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_creat to create and open a file inside the directory created in #1
**   4) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**--------------------------------------------------------------------------------*/
void UT_os_makedir_test()
{
    osal_id_t fileDesc = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_mkdir(NULL, OS_READ_WRITE), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_mkdir(g_longPathName, OS_READ_WRITE), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Invalid-path-arg */

    UT_RETVAL(OS_mkdir("tmpDir", OS_READ_WRITE), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/mkdir_Nominal", g_mntName);
    UT_NOMINAL(OS_mkdir(g_dirName, OS_READ_WRITE));

    memset(g_fileName, '\0', sizeof(g_fileName));
    UT_os_sprintf(g_fileName, "%s/mkdir_File.txt", g_dirName);
    UT_NOMINAL(OS_OpenCreate(&fileDesc, g_fileName, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE));

    /* Reset test environment */
    UT_TEARDOWN(OS_close(fileDesc));
    UT_TEARDOWN(OS_remove(g_fileName));
    UT_TEARDOWN(OS_rmdir(g_dirName));
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_DirectoryOpen(uint32 *dir_id, const char *path)
** Purpose: Opens the specified directory for reading
** Parameters: *dir_id - pointer to directory id (set by this function)
**             *path - pointer to the absolute pathname of the directory to be opened
** Returns: OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
**          OS_INVALID_POINTER if either pointer passed in is NULL
**          OS_TranslatePath error response if failed
**          OS_ObjectIdAllocateNew error response if failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) NULL
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with a really long path as argument
**   2) Expect the returned value to be
**        (a) NULL
** -----------------------------------------------------
** Test #3: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) NULL
** -----------------------------------------------------
** Test #4: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) NULL
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Call OS_mkdir to create a directory under /cf mount point
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with directory name used in #1 as argument
**   4) Expect the returned value to be
**        (a) a directory descriptor pointer that is __not__ NULL
**--------------------------------------------------------------------------------*/
void UT_os_opendir_test()
{
    osal_id_t dirh = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_DirectoryOpen(&dirh, NULL), OS_INVALID_POINTER);
    UT_RETVAL(OS_DirectoryOpen(NULL, "/drive0/test"), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_DirectoryOpen(&dirh, g_longPathName), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Invalid-path-arg */

    UT_RETVAL(OS_DirectoryOpen(&dirh, "/drive0/tmpDir"), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    memset(g_dirName, '\0', sizeof(g_dirName));

    /* Non-existent directory should not succeed */
    UT_os_sprintf(g_dirName, "%s/notexist", g_mntName);
    UT_RETVAL(OS_DirectoryOpen(&dirh, g_dirName), OS_ERROR);

    UT_os_sprintf(g_dirName, "%s/opendir_Nominal", g_mntName);
    if (UT_SETUP(OS_mkdir(g_dirName, OS_READ_WRITE)))
    {
        UT_NOMINAL(OS_DirectoryOpen(&dirh, g_dirName));

        /* Reset test environment */
        UT_TEARDOWN(OS_DirectoryClose(dirh));
        UT_TEARDOWN(OS_rmdir(g_dirName));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_DirectoryClose(uint32 dir_id)
** Purpose: Closes the specified directory for reading
** Parameters: dir_id - directory id that was returned from OS_DirectoryOpen()
** Returns: OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
**          OS_ObjectIdGetById return if failed
**          OS_DirClose_Impl return if failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call OS_mkdir() to create a directory under /cf mount point
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_DirectoryOpen() with directory name used in #1 as argument
**   4) Expect the returned value to be
**        (a) a non-zero directory id
**   5) Call this routine with the directory descriptor pointer returned in #3 as argument
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call OS_DirectoryRead() with the directory descriptor pointer returned in #3 as argument
**   8) Expect to not get OS_SUCCESS (closed directory)
**--------------------------------------------------------------------------------*/
void UT_os_closedir_test()
{
    osal_id_t   dirh = OS_OBJECT_ID_UNDEFINED;
    os_dirent_t dirEntry;

    UT_RETVAL(OS_DirectoryClose(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_DirectoryClose(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Nominal */

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/closeDir3", g_mntName);
    if (UT_SETUP(OS_mkdir(g_dirName, OS_READ_WRITE)))
    {
        if (UT_SETUP(OS_DirectoryOpen(&dirh, g_dirName)))
        {
            UT_NOMINAL(OS_DirectoryClose(dirh));
            UT_RETVAL(OS_DirectoryRead(dirh, &dirEntry), OS_ERR_INVALID_ID);
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_rmdir(g_dirName));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_DirectoryRead(uint32 dir_id, os_dirent_t *dirent)
** Purpose: Provides information about a directory
** Parameters: dir_id - directory id from OS_DirectoryOpen
**             *dirent - pointer to the directory entry (set by this function)
** Returns: OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
**          OS_INVALID_POINTER if pointer passed in is NULL
**          OS_ObjectIdGetById error response if failed
**          OS_Unlock_Global_Impl error response if failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) NULL
** -----------------------------------------------------
** Test #2: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) NULL
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_mkdir() to create a directory
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_DirectoryOpen() with directory name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call this routine with the directory descriptor pointer returned in #3
**   6) Expect the returned value to be
**        (a) a directory entry pointer __and__
**        (b) the directory name to be "."
**   7) Call this routine again with the directory descriptor pointer returned in #3
**   8) Expect the returned value to be
**        (a) a directory entry pointer __and__
**        (b) the directory name to be ".."
**--------------------------------------------------------------------------------*/
void UT_os_readdir_test()
{
    osal_id_t   dirh = OS_OBJECT_ID_UNDEFINED;
    os_dirent_t dirent;

    strcpy(g_subdirNames[0], " ");
    strcpy(g_subdirNames[1], " ");

    /*-----------------------------------------------------*/
    /* Invalid ID */

    UT_RETVAL(OS_DirectoryRead(UT_OBJID_INCORRECT, &dirent), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_DirectoryRead(OS_OBJECT_ID_UNDEFINED, &dirent), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/readdir_Nominal", g_mntName);
    if (UT_SETUP(OS_mkdir(g_dirName, OS_READ_WRITE)))
    {
        memset(g_subdirNames[0], '\0', sizeof(g_subdirNames[0]));
        UT_os_sprintf(g_subdirNames[0], "%s/%s", g_dirName, g_tgtSubdirs[0]);
        if (UT_SETUP(OS_mkdir(g_subdirNames[0], OS_READ_WRITE)))
        {
            memset(g_subdirNames[1], '\0', sizeof(g_subdirNames[1]));
            UT_os_sprintf(g_subdirNames[1], "%s/%s", g_dirName, g_tgtSubdirs[1]);
            if (UT_SETUP(OS_mkdir(g_subdirNames[1], OS_READ_WRITE)))
            {
                if (UT_SETUP(OS_DirectoryOpen(&dirh, g_dirName)))
                {
                    /*-----------------------------------------------------*/
                    /* Null-pointer-arg */
                    UT_RETVAL(OS_DirectoryRead(dirh, NULL), OS_INVALID_POINTER);

                    /*-----------------------------------------------------*/
                    /* Nominal (via subfunction) */
                    UT_os_read_n_sort_dirs(dirh);

                    /* Reset test environment */
                    UT_TEARDOWN(OS_DirectoryClose(dirh));
                }

                UT_TEARDOWN(OS_rmdir(g_subdirNames[1]));
            }

            UT_TEARDOWN(OS_rmdir(g_subdirNames[0]));
        }

        UT_TEARDOWN(OS_rmdir(g_dirName));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_DirectoryRewind(uint32 dir_id)
** Purpose: Rewinds the directory to the beginning
** Parameters: dir_id - directory id from OS_DirectoryOpen
** Returns: OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
**          OS_ObjectIdGetById error response if failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) NULL
** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call OS_mkdir() to create a directory
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_opendir() with directory name used in #1 as argument
**   4) Expect the returned value to be
**        (a) a directory descriptor pointer
**   5) Call OS_readdir() with the directory descriptor pointer returned in #3
**   6) Expect the returned value to be
**        (a) a directory entry pointer __and__
**        (b) the directory name to be "."
**   7) Call OS_readdir() again with the directory descriptor pointer returned in #3
**   8) Expect the returned value to be
**        (a) a directory entry pointer __and__
**        (b) the directory name to be ".."
**   9) Call this routine with the directory descriptor pointer returned in #3
**  10) Call OS_readdir() the 3rd time with the directory descriptor pointer returned in #3
**  11) Expect the returned value to be
**        (a) a directory entry pointer __and__
**        (b) the directory name to be "."
**--------------------------------------------------------------------------------*/
void UT_os_rewinddir_test()
{
    osal_id_t dirh = OS_OBJECT_ID_UNDEFINED;

    strcpy(g_subdirNames[0], " ");
    strcpy(g_subdirNames[1], " ");

    /*-----------------------------------------------------*/
    /* Invalid ID */

    UT_RETVAL(OS_DirectoryRewind(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_DirectoryRewind(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Nominal */

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/rewinddir_Nominal", g_mntName);
    if (UT_SETUP(OS_mkdir(g_dirName, OS_READ_WRITE)))
    {
        memset(g_subdirNames[0], '\0', sizeof(g_subdirNames[0]));
        UT_os_sprintf(g_subdirNames[0], "%s/%s", g_dirName, g_tgtSubdirs[0]);
        if (UT_SETUP(OS_mkdir(g_subdirNames[0], OS_READ_WRITE)))
        {
            memset(g_subdirNames[1], '\0', sizeof(g_subdirNames[1]));
            UT_os_sprintf(g_subdirNames[1], "%s/%s", g_dirName, g_tgtSubdirs[1]);
            if (UT_SETUP(OS_mkdir(g_subdirNames[1], OS_READ_WRITE)))
            {
                if (UT_SETUP(OS_DirectoryOpen(&dirh, g_dirName)))
                {
                    UT_os_read_n_sort_dirs(dirh);

                    UT_NOMINAL(OS_DirectoryRewind(dirh));

                    UT_os_read_n_sort_dirs(dirh);

                    /* Reset test environment */
                    UT_TEARDOWN(OS_DirectoryClose(dirh));
                }

                UT_TEARDOWN(OS_rmdir(g_subdirNames[1]));
            }

            UT_TEARDOWN(OS_rmdir(g_subdirNames[0]));
        }

        UT_TEARDOWN(OS_rmdir(g_dirName));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_rmdir(const char *path)
** Purpose: Removes the specified directory
** Parameters: *path - pointer to the absolute pathname of the directory to be removed
** Returns: OS_INVALID_POINTER if pointer passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if path is too long
**          OS_FS_ERR_PATH_INVALID if path is invalid
**          OS_ERROR if OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Path-too-long-arg condition
**   1) Call this routine with a really long path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #3: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #4: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Call OS_mkdir to create a directory under /cf mount point
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_creat() to create and open a file under the directory created in #1
**   4) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   5) Call this routine with directory name used in #1
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call OS_close() with the file descriptor returned in #3 as argument
**   8) Call OS_remove() with the file name used in #3 as argument
**   9) Call OS_creat() to create and open another file under the directory deleted in #5
**  10) Expect the returned value to be
**        (a) a file descriptor value less than 0
**--------------------------------------------------------------------------------*/
void UT_os_removedir_test()
{
    osal_id_t fileDesc = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_rmdir(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Path-too-long-arg */

    UT_RETVAL(OS_rmdir(g_longPathName), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Invalid-path-arg */

    UT_RETVAL(OS_rmdir("tmpDir"), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/rmdir_Nominal", g_mntName);
    if (UT_SETUP(OS_mkdir(g_dirName, OS_READ_WRITE)))
    {
        memset(g_fileName, '\0', sizeof(g_fileName));
        UT_os_sprintf(g_fileName, "%s/rmdir_File1.txt", g_dirName);
        if (UT_SETUP(OS_OpenCreate(&fileDesc, g_fileName, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
        {
            UT_RETVAL(OS_rmdir(g_dirName), OS_ERROR);

            /* Must close and remove all files before the directory can be removed */
            UT_TEARDOWN(OS_close(fileDesc));
            UT_TEARDOWN(OS_remove(g_fileName));

            UT_NOMINAL(OS_rmdir(g_dirName));

            memset(g_fileName, '\0', sizeof(g_fileName));
            UT_os_sprintf(g_fileName, "%s/rmdir_File2.txt", g_dirName);
            UT_RETVAL(OS_OpenCreate(&fileDesc, g_fileName, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE),
                      OS_ERROR);
        }
    }
}

/*--------------------------------------------------------------------------------
 * Internal helper function
 *--------------------------------------------------------------------------------*/
void UT_os_read_n_sort_dirs(osal_id_t dirh)
{
    os_dirent_t dirEntry;
    uint32      NumMatched;
    uint32      NumEntries;
    uint32      Check;
    int32       Status;
    const char *Name;

    memset(g_dirItems, 0, sizeof(g_dirItems));
    memset(&dirEntry, 0, sizeof(dirEntry));

    NumMatched = 0;
    NumEntries = 0;

    while (NumMatched < UT_OS_FILE_NUM_DIR_ENTRIES && NumEntries == NumMatched)
    {
        UT_NOMINAL(OS_DirectoryRead(dirh, &dirEntry));

        Name = OS_DIRENTRY_NAME(dirEntry);

        /* Ignore UNIX-style special entries (. and ..) */
        if (Name[0] != '.')
        {
            ++NumEntries;
            UtPrintf("OS_DirectoryRead() name=%s\n", Name);
            for (Check = 0; Check < UT_OS_FILE_NUM_DIR_ENTRIES; ++Check)
            {
                if (strcmp(Name, g_tgtSubdirs[Check]) == 0)
                {
                    strcpy(g_dirItems[Check], Name);
                    ++NumMatched;
                }
            }
        }
    }

    /* asserts that the expected number of regular entries was found */
    UtAssert_UINT32_EQ(NumMatched, UT_OS_FILE_NUM_DIR_ENTRIES);

    /*
     * now continue to read to verify behavior at end of directory.
     * since there is no guaranteed order in directories, also need to
     * ignore the special entries here, in case they appear now.
     */
    do
    {
        Status = OS_DirectoryRead(dirh, &dirEntry);
        Name   = OS_DIRENTRY_NAME(dirEntry);
    } while (Status == OS_SUCCESS && Name[0] == '.');

    /* Final return value should have been OS_ERROR, indicating end of directory */
    UtAssert_True(Status == OS_ERROR, "OS_DirectoryRead() (%d) == OS_ERROR (at end)", (int)Status);
}

/*================================================================================*
** End of File: ut_osfile_dirio_test.c
**================================================================================*/
