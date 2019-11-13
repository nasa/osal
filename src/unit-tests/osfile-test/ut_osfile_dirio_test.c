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

#define UT_OS_FILE_MAX_DIRS  5

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern UT_OsLogInfo_t  g_logInfo;

extern char* g_fsAddrPtr;

extern char  g_longPathName[OS_MAX_PATH_LEN+5];

extern char* g_mntName;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

char  g_dirName[UT_OS_SM_TEXT_LEN];
char  g_fileName[UT_OS_SM_TEXT_LEN];

char  g_subdirNames[UT_OS_FILE_MAX_DIRS][UT_OS_SM_TEXT_LEN];
const char* g_tgtSubdirs[UT_OS_FILE_MAX_DIRS] = {"subdir1", "subdir2"};

char  g_dirItems[UT_OS_FILE_MAX_DIRS][UT_OS_SM_TEXT_LEN];

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_read_n_sort_dirs(uint32);

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/* Test code template for testing a single OSAL API with multiple test cases */

#if 0
void UT_os_sample_test()
{
    /* Must declare these variables for each function. They can be renamed.
     * They're referenced in the macros used to track test cases and their results. */
    int32 idx = 0;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc = NULL;

    /* Call this once at the beginning of the function to initialize the test variables. */
    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*
     * For each test case,
     *   1. Assign testDesc a brief description of the test
     *   2. Setup the test environment, if necessary
     *   3. Run the test
     *   4. Log result by calling UT_OS_SET_TEST_RESULT_MACRO
     *   4. Reset the test environment, if neccessary
     *
     * NOTE: "Not implemented" is always checked first but not
     *       being included as a test case.
     *       "Nominal" test case is always the last test case.
     *-----------------------------------------------------*/

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    /* TODO: Setup the test environment, if necessary */

    if (OS_xxx() == OS_FS_UNIMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_sample_test_exit_tag;
    }

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(NULL,...) == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#2 Name-too-long-arg";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(aVeryLoooooongName) == OS_ERR_NAME_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* TODO: Reset the test environment here, if necessary */

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* TODO: Setup the test environment here, if necessary */

    if (OS_xxx(...) != OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)

    /* TODO: Reset the test environment here, if necessary */

UT_os_sample_test_exit_tag:
    /* Call these macros at the very end of the function to close out the test variables
     * and get it added to the global list being tracked. */
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_xxx", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}
#endif

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_mkdir(const char *path, uint32 access)
** Purpose: Creates a directory specified by path
** Parameters: *path - pointer to the absolute pathname of the directory to be created
**             access - directory access mode (unused)
** Returns: OS_FS_SUCCESS if succeeded
**          OS_FS_UNIMPLEMENTED if not implemented
**          OS_FS_ERR_INVALID_POINTER if the pointer passed in is null
**          OS_FS_ERR_PATH_TOO_LONG if the path is too long
**          OS_FS_ERR_PATH_INVALID if the path is invalid
**          OS_FS_ERROR if the OS call failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_FS_UNIMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
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
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Call this routine to create a directory under /cf mount point
**   2) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   3) Call OS_creat to create and open a file inside the directory created in #1
**   4) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**--------------------------------------------------------------------------------*/
void UT_os_makedir_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 idx=0, fileDesc=0;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_mkdir(NULL, 755) == OS_FS_UNIMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_makedir_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_mkdir(NULL, 755) == OS_FS_ERR_INVALID_POINTER)
        /* cppcheck-suppress syntaxError */
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if (OS_mkdir(g_longPathName, 755) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Invalid-path-arg";

    if (OS_mkdir("tmpDir", 755) == OS_FS_ERR_PATH_INVALID)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/mkdir_Nominal", g_mntName);
    if (OS_mkdir(g_dirName, 755) != OS_FS_SUCCESS)
    {
        testDesc = "#5 Nominal - File-system-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_makedir_test_exit_tag;
    }

    memset(g_fileName, '\0', sizeof(g_fileName));
    UT_os_sprintf(g_fileName, "%s/mkdir_File.txt", g_dirName);
    fileDesc = OS_creat(g_fileName, OS_READ_WRITE);
    if (fileDesc >= 0)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_close(fileDesc);
    OS_remove(g_fileName);
    OS_rmdir(g_dirName);

UT_os_makedir_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_mkdir", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_DirectoryOpen(uint32 *dir_id, const char *path)
** Purpose: Opens the specified directory for reading
** Parameters: *dir_id - pointer to directory id (set by this function)
**             *path - pointer to the absolute pathname of the directory to be opened
** Returns: OS_SUCCESS if succeeded
**          OS_FS_UNIMPLEMENTED if not implemented
**          OS_INVALID_POINTER if either pointer passed in is NULL
**          OS_TranslatePath error response if failed
**          OS_ObjectIdAllocateNew error response if failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_FS_UNIMPLEMENTED, then exit test
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
**        (a) OS_FS_SUCCESS
**   3) Call this routine with directory name used in #1 as argument
**   4) Expect the returned value to be
**        (a) a directory descriptor pointer that is __not__ NULL
**--------------------------------------------------------------------------------*/
void UT_os_opendir_test()
{
    int32 idx=0;
    uint32 dirh;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_DirectoryOpen(&dirh, NULL) == OS_FS_UNIMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_opendir_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_DirectoryOpen(&dirh, NULL) == OS_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if (OS_DirectoryOpen(&dirh, g_longPathName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Invalid-path-arg";

    if (OS_DirectoryOpen(&dirh, "/drive0/tmpDir") != OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/opendir_Nominal", g_mntName);
    if (OS_mkdir(g_dirName, 755) != OS_FS_SUCCESS)
    {
        testDesc = "#5 Nominal - Dir-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_opendir_test_exit_tag;
    }

    if (OS_DirectoryOpen(&dirh, g_dirName) == OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_DirectoryClose(dirh);
    OS_rmdir(g_dirName);

UT_os_opendir_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_opendir", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_DirectoryClose(uint32 dir_id)
** Purpose: Closes the specified directory for reading
** Parameters: dir_id - directory id that was returned from OS_DirectoryOpen()
** Returns: OS_SUCCESS if succeeded
**          OS_FS_UNIMPLEMENTED if not implemented
**          OS_ObjectIdGetById return if failed
**          OS_DirClose_Impl return if failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_FS_UNIMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call OS_mkdir() to create a directory under /cf mount point
**   2) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   3) Call OS_DirectoryOpen() with directory name used in #1 as argument
**   4) Expect the returned value to be
**        (a) a non-zero directory id
**   5) Call this routine with the directory descriptor pointer returned in #3 as argument
**   6) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   7) Call OS_DirectoryRead() with the directory descriptor pointer returned in #3 as argument
**   8) Expect to not get OS_SUCCESS (closed directory)
**--------------------------------------------------------------------------------*/
void UT_os_closedir_test()
{
    int32 idx=0;
    uint32 dirh;
    os_dirent_t* dirEntry=NULL;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_DirectoryClose(0) == OS_FS_UNIMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_closedir_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/closeDir3", g_mntName);
    if (OS_mkdir(g_dirName, 755) != OS_FS_SUCCESS)
    {
        testDesc = "#2 Nominal - Dir-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_closedir_test_exit_tag;
    }

    if (OS_DirectoryOpen(&dirh, g_dirName) != OS_SUCCESS)
    {
        testDesc = "#2 Nominal - Dir-open failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        goto UT_os_closedir_test_exit_tag;
    }

    if (OS_DirectoryClose(dirh) != OS_SUCCESS)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        goto UT_os_closedir_test_exit_tag;
    }

    if (OS_DirectoryRead(dirh, dirEntry) != OS_SUCCESS)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /* Reset test environment */
    OS_rmdir(g_dirName);

UT_os_closedir_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_closedir", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_DirectoryRead(uint32 dir_id, os_dirent_t *dirent)
** Purpose: Provides information about a directory
** Parameters: dir_id - directory id from OS_DirectoryOpen
**             *dirent - pointer to the directory entry (set by this function)
** Returns: OS_SUCCESS if succeeded
**          OS_FS_UNIMPLEMENTED if not implemented
**          OS_FS_ERR_INVALID_POINTER if pointer passed in is NULL
**          OS_ObjectIdGetById error response if failed
**          OS_Unlock_Global_Impl error response if failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_FS_UNIMPLEMENTED, then exit test
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
**        (a) OS_FS_SUCCESS
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
    int32 idx=0;
    uint32 dirh;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    strcpy(g_subdirNames[0], " ");
    strcpy(g_subdirNames[1], " ");

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_DirectoryRead(0, NULL) == OS_FS_UNIMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_readdir_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_DirectoryRead(0, NULL) == OS_FS_ERR_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/readdir_Nominal", g_mntName);
    if (OS_mkdir(g_dirName, 755) != OS_FS_SUCCESS)
    {
        testDesc = "#3 Nominal - Dir-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_readdir_test_exit_tag;
    }

    memset(g_subdirNames[0], '\0', sizeof(g_subdirNames[0]));
    UT_os_sprintf(g_subdirNames[0], "%s/%s", g_dirName, g_tgtSubdirs[0]);
    if (OS_mkdir(g_subdirNames[0], 755) != OS_FS_SUCCESS)
    {
        testDesc = "#3 Nominal - Dir-create(subdir1) failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        goto UT_os_readdir_test_exit_tag;
    }

    memset(g_subdirNames[1], '\0', sizeof(g_subdirNames[1]));
    UT_os_sprintf(g_subdirNames[1], "%s/%s", g_dirName, g_tgtSubdirs[1]);
    if (OS_mkdir(g_subdirNames[1], 755) != OS_FS_SUCCESS)
    {
        testDesc = "#3 Nominal - Dir-create(subdir2) failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        goto UT_os_readdir_test_exit_tag;
    }

    if (OS_DirectoryOpen(&dirh, g_dirName) != OS_SUCCESS)
    {
        testDesc = "#3 Nominal - Dir-open failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        goto UT_os_readdir_test_exit_tag;
    }

    UT_os_read_n_sort_dirs(dirh);

    if ((strcmp(g_dirItems[2], g_tgtSubdirs[0]) == 0) &&
        (strcmp(g_dirItems[3], g_tgtSubdirs[1]) == 0))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

UT_os_readdir_test_exit_tag:
    /* Reset test environment */
    OS_DirectoryClose(dirh);
    OS_rmdir(g_subdirNames[0]);
    OS_rmdir(g_subdirNames[1]);
    OS_rmdir(g_dirName);

    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_readdir", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_DirectoryRewind(uint32 dir_id)
** Purpose: Rewinds the directory to the beginning
** Parameters: dir_id - directory id from OS_DirectoryOpen
** Returns: OS_SUCCESS if succeeded
**          OS_FS_UNIMPLEMENTED if not implemented
**          OS_ObjectIdGetById error response if failed
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_FS_UNIMPLEMENTED, then exit test
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
**        (a) OS_FS_SUCCESS
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
**        (b) a the directory name to be "."
**--------------------------------------------------------------------------------*/
void UT_os_rewinddir_test()
{
    int32 idx=0;
    uint32 dirh;
    UT_OsApiInfo_t apiInfo;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    strcpy(g_subdirNames[0], " ");
    strcpy(g_subdirNames[1], " ");

    /*-----------------------------------------------------*/
    testDesc = "API Not implemented";

    if (OS_DirectoryRewind(0) == OS_FS_UNIMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_rewinddir_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/rewinddir_Nominal", g_mntName);
    if (OS_mkdir(g_dirName, 755) != OS_FS_SUCCESS)
    {
        testDesc = "#2 Nominal - Dir-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        goto UT_os_rewinddir_test_exit_tag;
    }

    memset(g_subdirNames[0], '\0', sizeof(g_subdirNames[0]));
    UT_os_sprintf(g_subdirNames[0], "%s/%s", g_dirName, g_tgtSubdirs[0]);
    if (OS_mkdir(g_subdirNames[0], 755) != OS_FS_SUCCESS)
    {
        testDesc = "#2 Nominal - Dir-create(subdir1) failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        goto UT_os_rewinddir_test_exit_tag;
    }

    memset(g_subdirNames[1], '\0', sizeof(g_subdirNames[1]));
    UT_os_sprintf(g_subdirNames[1], "%s/%s", g_dirName, g_tgtSubdirs[1]);
    if (OS_mkdir(g_subdirNames[1], 755) != OS_FS_SUCCESS)
    {
        testDesc = "#2 Nominal - Dir-create(subdir2) failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        goto UT_os_rewinddir_test_exit_tag;
    }

    if (OS_DirectoryOpen(&dirh, g_dirName) != OS_SUCCESS)
    {
        testDesc = "#2 Nominal - Dir-open failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        goto UT_os_rewinddir_test_exit_tag;
    }

    UT_os_read_n_sort_dirs(dirh);

    if ((strcmp(g_dirItems[2], g_tgtSubdirs[0]) != 0) ||
        (strcmp(g_dirItems[3], g_tgtSubdirs[1]) != 0))
    {
        testDesc = "#2 Nominal - Dir-read failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)

        goto UT_os_rewinddir_test_exit_tag;
    }

    OS_DirectoryRewind(dirh);

    UT_os_read_n_sort_dirs(dirh);

    if ((strcmp(g_dirItems[2], g_tgtSubdirs[0]) != 0) ||
        (strcmp(g_dirItems[3], g_tgtSubdirs[1]) != 0))
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)

UT_os_rewinddir_test_exit_tag:
    /* Reset test environment */
    OS_DirectoryClose(dirh);
    OS_rmdir(g_subdirNames[0]);
    OS_rmdir(g_subdirNames[1]);
    OS_rmdir(g_dirName);

    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_rewinddir", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_rmdir(const char *path)
** Purpose: Removes the specified directory
** Parameters: *path - pointer to the absolute pathname of the directory to be removed
** Returns: OS_FS_ERR_INVALID_POINTER if pointer passed in is NULL
**          OS_FS_ERR_PATH_TOO_LONG if path is too long
**          OS_FS_ERR_PATH_INVALID if path is invalid
**          OS_FS_ERROR if OS call failed
**          OS_FS_SUCCESS if succeeded
**          OS_FS_UNIMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_FS_UNIMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_INVALID_POINTER
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
**        (a) OS_FS_ERROR
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Call OS_mkdir to create a directory under /cf mount point
**   2) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   3) Call OS_creat() to create and open a file under the directory created in #1
**   4) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   5) Call this routine with directory name used in #1
**   6) Expect the returned value to be
**        (a) OS_FS_SUCCESS
**   7) Call OS_close() with the file descriptor returned in #3 as argument
**   8) Call OS_remove() with the file name used in #3 as argument
**   9) Call OS_creat() to create and open another file under the directory deleted in #5
**  10) Expect the returned value to be
**        (a) a file descriptor value less than 0
**--------------------------------------------------------------------------------*/
void UT_os_removedir_test()
{
    UT_OsApiInfo_t apiInfo;
    int32 idx=0, fileDesc=0;
    const char* testDesc=NULL;

    UT_OS_CLEAR_API_INFO_MACRO(apiInfo, idx)

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_rmdir(NULL) == OS_FS_UNIMPLEMENTED)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_NA)
        goto UT_os_removedir_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_rmdir(NULL) == OS_FS_ERR_INVALID_POINTER)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#2 Path-too-long-arg";

    if (OS_rmdir(g_longPathName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#3 Invalid-path-arg";

    if (OS_rmdir("tmpDir") == OS_FS_ERR_PATH_INVALID)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

    /*-----------------------------------------------------*/
    testDesc = "#4 OS-call-failure";

    UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_UOF)

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    memset(g_dirName, '\0', sizeof(g_dirName));
    UT_os_sprintf(g_dirName, "%s/rmdir_Nominal", g_mntName);
    if (OS_mkdir(g_dirName, 755) != OS_FS_SUCCESS)
    {
        testDesc = "#5 Nominal - Dir-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
        goto UT_os_removedir_test_exit_tag;
    }

    memset(g_fileName, '\0', sizeof(g_fileName));
    UT_os_sprintf(g_fileName, "%s/rmdir_File1.txt", g_dirName);
    fileDesc = OS_creat(g_fileName, OS_READ_WRITE);
    if (fileDesc < 0)
    {
        testDesc = "#5 Nominal - File-create failed";
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_TSF)
    }

    /* Must close and remove all files before the directory can be removed */
    OS_close(fileDesc);
    OS_remove(g_fileName);

    if (OS_rmdir(g_dirName) != OS_FS_SUCCESS)
    {
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)
        goto UT_os_removedir_test_exit_tag;
    }

    memset(g_fileName, '\0', sizeof(g_fileName));
    UT_os_sprintf(g_fileName, "%s/rmdir_File2.txt", g_dirName);
    fileDesc = OS_creat(g_fileName, OS_READ_WRITE);
    if (fileDesc < 0)
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_PASSED)
    else
        UT_OS_SET_TEST_RESULT_MACRO(apiInfo, idx, testDesc, UT_OS_FAILED)

UT_os_removedir_test_exit_tag:
    UT_OS_SET_API_NAME_AND_TEST_COUNT_MACRO(apiInfo, "OS_rmdir", idx)
    UT_OS_LOG_API_MACRO(apiInfo)
}

/*--------------------------------------------------------------------------------*
 * Internal helper function
**--------------------------------------------------------------------------------*/
void UT_os_read_n_sort_dirs(uint32 dirh)
{
    int i=0;
    os_dirent_t dirEntry;

    for (i=0; i < UT_OS_FILE_MAX_DIRS; i++)
        strcpy(g_dirItems[i], " ");

    while (OS_DirectoryRead(dirh, &dirEntry) == OS_SUCCESS)
    {
        if (strcmp(OS_DIRENTRY_NAME(dirEntry), ".") == 0)
            strcpy(g_dirItems[0], ".");
        else if (strcmp(OS_DIRENTRY_NAME(dirEntry), "..") == 0)
            strcpy(g_dirItems[1], "..");
        else if (strcmp(OS_DIRENTRY_NAME(dirEntry), g_tgtSubdirs[0]) == 0)
            strcpy(g_dirItems[2], g_tgtSubdirs[0]);
        else if (strcmp(OS_DIRENTRY_NAME(dirEntry), g_tgtSubdirs[1]) == 0)
            strcpy(g_dirItems[3], g_tgtSubdirs[1]);
    }
}

/*================================================================================*
** End of File: ut_osfile_dirio_test.c
**================================================================================*/
