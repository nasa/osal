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

/*================================================================================*
** File:  ut_osfile_fileio_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osfile_fileio_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_FILE_LIST_LEN (OS_MAX_NUM_OPEN_FILES + 10)

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern char *g_fsAddrPtr;

extern char g_longPathName[UT_OS_PATH_BUFF_SIZE];
extern char g_longFileName[UT_OS_PATH_BUFF_SIZE];
extern char g_invalidPath[UT_OS_PATH_BUFF_SIZE];

extern char *g_mntName;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

char      g_fNames[UT_OS_FILE_LIST_LEN][UT_OS_FILE_BUFF_SIZE];
int32     g_fStatus[UT_OS_FILE_LIST_LEN];
osal_id_t g_fDescs[UT_OS_FILE_LIST_LEN];

char g_readBuff[UT_OS_IO_BUFF_SIZE];
char g_writeBuff[UT_OS_IO_BUFF_SIZE];

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_FS_Init(void)
** Purpose: Initializes the tables that the OS file-system uses to keep track of information
**          about objects
** Parameters: None
** Returns: OS_ERROR on an unsuccessful inits
**          OS_SUCCESS on a successful inits
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Init-not-call-first condition
**   1) Don't call this routine first
**   2) Call TBD routine(s)
**   3) Expect the returned value from those routines to be
**       (a) __not__ OS_SUCCESS
*** -----------------------------------------------------
** Test #2: Nominal condition
**   1) Call this routine
**   2) Expect the returned value to be
**       (a) OS_SUCCESS (although results are not directly observable)
**   3) Call TBD routine(s)
**   4) Expect the returned value from those routines to be
**       (a) OS_SUCCESS
*--------------------------------------------------------------------------------*/
void UT_os_initfs_test()
{
    int32       res = 0;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "#1 Init-not-call-first - Test case not applicable on platform";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    /* Call to OS_FS_Init is inside OS_API_Init */
    res = OS_API_Init();
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        testDesc = "API not implemented";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
    }
    else if (res == OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    }
    else
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    }

    return;
}

/*--------------------------------------------------------------------------------*
** Purpose: Test OS_OpenCreate for creating files
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_creat() is implemented, then OS_close() and OS_remove() should
**            also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #3: Path-too-long-arg condition
**   1) Call this routine with a path of length greater than OS_MAX_PATH_LEN as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #4: Name-too-long-arg condition
**   1) Call this routine with just a file name (no absolute path) as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #5: Invalid-permission-arg condition
**   1) Call this routine with a value of neither OS_WRITE_ONLY or OS_READ_WRITE
**   2) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #6: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #7: File-descriptors-full condition
**   1) Call this routine OS_MAX_NUM_OPEN_FILES+1 times
**   2) Expect the returned value, of all but the last call, to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Expect the returned value, of the last call, to be
**        (b) OS_ERR_NO_FREE_IDS
** -----------------------------------------------------
** Test #8: Nominal condition
**   1) Call this routine twice with different file names and access modes
**   2) Expect both returned values to be
**        (a) A file descriptor value greater than or equal to 0
**   3) Call OS_close() on both files opened in #1
**   4) Expect both returned values to be
**        (a) OS_SUCCESS
**   5) Call OS_remove() on both files closed in #3
**   6) Expect both returned values to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_createfile_test()
{
    const char *testDesc;
    int32       res = 0, i = 0, j = 0;
    osal_id_t   fd;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_OpenCreate(NULL, NULL, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_createfile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_OpenCreate(NULL, "file", OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    if (OS_OpenCreate(&fd, NULL, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path-arg";

    if (OS_OpenCreate(&fd, g_invalidPath, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE) ==
        OS_FS_ERR_PATH_INVALID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Path-too-long-arg";

    if (OS_OpenCreate(&fd, g_longPathName, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE) ==
        OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-too-long-arg";

    if (OS_OpenCreate(&fd, g_longFileName, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE) ==
        OS_FS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 Invalid-permission-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Create_InvPerm.txt", g_mntName);
    res = OS_OpenCreate(&fd, g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, 123);
    if (res == OS_ERROR)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_remove(g_fNames[0]);

    /*-----------------------------------------------------*/
    testDesc = "#6 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#7 File-descriptors-full";

    for (i = 0; i <= OS_MAX_NUM_OPEN_FILES; i++)
    {
        memset(g_fNames[i], '\0', sizeof(g_fNames[i]));
        UT_os_sprintf(g_fNames[i], "%s/tmpFile%d.txt", g_mntName, (int)i);
        g_fStatus[i] =
            OS_OpenCreate(&g_fDescs[i], g_fNames[i], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY);
        if (g_fStatus[i] < 0)
            break;
    }

    if ((i == OS_MAX_NUM_OPEN_FILES) && (g_fStatus[i] == OS_ERR_NO_FREE_IDS))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    for (j = 0; j < i; j++)
    {
        OS_close(g_fDescs[j]);
        OS_remove(g_fNames[j]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#8 Nominal";

    g_fStatus[5] = OS_OpenCreate(&g_fDescs[5], g_fNames[5], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY);
    g_fStatus[6] = OS_OpenCreate(&g_fDescs[6], g_fNames[6], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY);

    if ((OS_close(g_fDescs[5]) != OS_SUCCESS) || (OS_close(g_fDescs[6]) != OS_SUCCESS) ||
        (OS_remove(g_fNames[5]) != OS_SUCCESS) || (OS_remove(g_fNames[6]) != OS_SUCCESS))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);

UT_os_createfile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Purpose: Tests OS_OpenCreate for opening files
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_creat() is implemented, then OS_close() and OS_remove() should
**            also be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-arg condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #3: Path-too-long-arg condition
**   1) Call this routine with a path of length greater than OS_MAX_PATH_LEN as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #4: Name-too-long-arg condition
**   1) Call this routine with just a file name (no absolute path) as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #5: Invalid-permission-arg condition
**   1) Call this routine with a value of neither OS_WRITE_ONLY or OS_READ_WRITE
**   2) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #6: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #7: File-descriptors-full condition
**   1) Call this routine OS_MAX_NUM_OPEN_FILES+1 times
**   2) Expect the returned value, of all but the last call, to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Expect the returned value, of the last call, to be
**        (b) OS_ERR_NO_FREE_IDS
** -----------------------------------------------------
** Test #8: Nominal condition
**   1) Call this routine twice with different file names and access modes
**   2) Expect both returned values to be
**        (a) A file descriptor value greater than or equal to 0
**   3) Call OS_close() on both files opened in #1
**   4) Expect both returned values to be
**        (a) OS_SUCCESS
**   5) Call OS_remove() on both files closed in #3
**   6) Expect both returned values to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_openfile_test()
{
    const char *testDesc;
    int32       res = 0, i = 0, j = 0, continueFlg = 0;
    osal_id_t   fd;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_OpenCreate(&fd, NULL, OS_FILE_FLAG_NONE, OS_READ_WRITE) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_openfile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_OpenCreate(&fd, NULL, OS_FILE_FLAG_NONE, OS_READ_WRITE) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    if (OS_OpenCreate(NULL, "file", OS_FILE_FLAG_NONE, OS_READ_WRITE) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path-arg";

    if (OS_OpenCreate(&fd, g_invalidPath, OS_FILE_FLAG_NONE, OS_READ_WRITE) == OS_FS_ERR_PATH_INVALID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Path-too-long-arg";

    if (OS_OpenCreate(&fd, g_longPathName, OS_FILE_FLAG_NONE, OS_READ_WRITE) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-too-long-arg";

    if (OS_OpenCreate(&fd, g_longFileName, OS_FILE_FLAG_NONE, OS_READ_WRITE) == OS_FS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 Invalid-permission-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Open_InvPerm.txt", g_mntName);
    res = OS_OpenCreate(&fd, g_fNames[0], OS_FILE_FLAG_NONE, 123);
    if (res == OS_ERROR)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_remove(g_fNames[0]);

    /*-----------------------------------------------------*/
    testDesc = "#6 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#7 File-descriptors-full";

    continueFlg = 1;
    for (i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        memset(g_fNames[i], '\0', sizeof(g_fNames[i]));
        UT_os_sprintf(g_fNames[i], "%s/tmpFile%d.txt", g_mntName, (int)i);
        g_fStatus[i] =
            OS_OpenCreate(&g_fDescs[i], g_fNames[i], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY);
        if (g_fStatus[i] < OS_SUCCESS)
        {
            testDesc = "#7 File-descriptors-full - File-create failed";
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
            continueFlg = 0;
            break;
        }

        if (continueFlg && (OS_close(g_fDescs[i]) != OS_SUCCESS))
        {
            testDesc = "#7 File-descriptors-full - File-close failed";
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
            continueFlg = 0;
            break;
        }
    }

    if (continueFlg)
    {
        for (i = 0; i <= OS_MAX_NUM_OPEN_FILES; i++)
        {
            g_fStatus[i] = OS_OpenCreate(&g_fDescs[i], g_fNames[i], OS_FILE_FLAG_NONE, OS_WRITE_ONLY);
            if (g_fStatus[i] < 0)
                break;
        }

        if ((i == OS_MAX_NUM_OPEN_FILES) && (g_fStatus[i] < OS_SUCCESS))
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        for (j = 0; j < i; j++)
        {
            OS_close(g_fDescs[j]);
            OS_remove(g_fNames[j]);
        }
    }

    /*-----------------------------------------------------*/
    testDesc = "#8 Nominal";

    g_fStatus[5] = OS_OpenCreate(&g_fDescs[5], g_fNames[5], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    g_fStatus[6] = OS_OpenCreate(&g_fDescs[6], g_fNames[6], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY);
    if ((g_fStatus[5] < OS_SUCCESS) || (g_fStatus[6] < OS_SUCCESS))
    {
        testDesc = "#8 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_openfile_test_exit_tag;
    }

    if ((OS_close(g_fDescs[5]) != OS_SUCCESS) || (OS_close(g_fDescs[6]) != OS_SUCCESS))
    {
        testDesc = "#8 Nominal - File-close failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_openfile_test_exit_tag;
    }

    g_fStatus[5] = OS_OpenCreate(&g_fDescs[5], g_fNames[5], OS_FILE_FLAG_NONE, OS_READ_WRITE);
    g_fStatus[6] = OS_OpenCreate(&g_fDescs[6], g_fNames[6], OS_FILE_FLAG_NONE, OS_WRITE_ONLY);

    if ((OS_close(g_fDescs[5]) != OS_SUCCESS) || (OS_close(g_fDescs[6]) != OS_SUCCESS) ||
        (OS_remove(g_fNames[5]) != OS_SUCCESS) || (OS_remove(g_fNames[6]) != OS_SUCCESS))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);

UT_os_openfile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_close(int32 filedes)
** Purpose: Closes a file of a given file descriptor
** Parameters: filedes - a file descriptor value
** Returns: OS_ERR_INVALID_ID if the file descriptor passed in invalid
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_close() is implemented, then OS_open() should also be implemented.
** -----------------------------------------------------
** Test #1: Invalid-file-desc-arg condition
**   1) Call this routine 3 times with a file descriptors of 0, -1, and (OS_MAX_NUM_OPEN_FILES+5)
**      respectively as argument
**   2) Expect all three returned values to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #2: OS-call-failure condition
**   1) Setup the test to fail OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call this routine with the file descriptor returned in #1
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_read() and OS_write() with the file descriptor returned in #1
**   6) Expect both returned value to be
**        (a) OS_ERR_INVALID_ID
**--------------------------------------------------------------------------------*/
void UT_os_closefile_test()
{
    const char *testDesc;
    char        tmpBuff[UT_OS_IO_BUFF_SIZE];

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_close(UT_OBJID_INCORRECT) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_closefile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-file-desc-arg";

    if (OS_close(UT_OBJID_INCORRECT) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Close_Nominal.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#3 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_closefile_test_exit_tag;
    }

    if ((OS_close(g_fDescs[0]) != OS_SUCCESS) ||
        (OS_write(g_fDescs[0], tmpBuff, sizeof(tmpBuff)) != OS_ERR_INVALID_ID) ||
        (OS_read(g_fDescs[0], tmpBuff, sizeof(tmpBuff)) != OS_ERR_INVALID_ID))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);

    /* Reset test environment */
    OS_remove(g_fNames[0]);

UT_os_closefile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_read(int32 filedes, void *buffer, uint32 nbytes)
** Purpose: Reads nBytes from the file of a given file descriptors and write that
**          nBytes to the given buffer
** Parameters: filedes - a file descriptor
**             *buffer - pointer that will hold the data read from file
**             nbytes - the number of bytes to be read from file
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the file descriptor passed in is invalid
**          OS_ERROR if the OS call failed
**          The number of bytes read if succeeded
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
** Test #2: Invalid-file-desc-arg condition
**   1) Call this routine 3 times with a file descriptors of 0, -1, and (OS_MAX_NUM_OPEN_FILES+5)
**      respectively as argument
**   2) Expect all three returned values to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to fail OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call OS_write() to write something to the file opened in #1
**   4) Expect the returned value to be
**        (a) number of bytes written that is equal to the number of bytes in the write buffer
**   5) Call OS_close() to flush and close the file opened in #1
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call OS_open() with file name used in #1 as argument
**   8) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   9) Call this routine with the file descriptor returned in #7
**  10) Expect the returned value to be
**        (a) the number of bytes read that is equal to the number of bytes written in #3 __and__
**        (b) the read buffer content is the same as the write buffer content in #3
**--------------------------------------------------------------------------------*/
void UT_os_readfile_test()
{
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_read(UT_OBJID_INCORRECT, NULL, 0) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_readfile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Read_NullPtr.txt", g_mntName);
    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#1 Null-pointer-arg - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        if (OS_read(g_fDescs[0], NULL, sizeof(g_readBuff)) == OS_INVALID_POINTER)
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        OS_close(g_fDescs[0]);
        OS_remove(g_fNames[0]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-file-desc-arg";

    if (OS_read(UT_OBJID_INCORRECT, g_readBuff, sizeof(g_readBuff)) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Read_Nominal.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#4 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_readfile_test_exit_tag;
    }

    memset(g_writeBuff, '\0', sizeof(g_writeBuff));
    strcpy(g_writeBuff, "A HORSE! A HORSE! MY KINGDOM FOR A HORSE!");
    if (OS_write(g_fDescs[0], g_writeBuff, strlen(g_writeBuff)) != strlen(g_writeBuff))
    {
        testDesc = "#4 Nominal - File-write failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);

        /* Reset test environment */
        OS_close(g_fDescs[0]);
        OS_remove(g_fNames[0]);

        goto UT_os_readfile_test_exit_tag;
    }

    if (OS_close(g_fDescs[0]) != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-close failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);

        /* Reset test environment */
        OS_remove(g_fNames[0]);

        goto UT_os_readfile_test_exit_tag;
    }

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_NONE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#4 Nominal - File-open failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);

        /* Reset test environment */
        OS_remove(g_fNames[0]);

        goto UT_os_readfile_test_exit_tag;
    }

    memset(g_readBuff, '\0', sizeof(g_readBuff));
    if ((OS_read(g_fDescs[0], g_readBuff, strlen(g_writeBuff)) == strlen(g_writeBuff)) &&
        (strncmp(g_readBuff, g_writeBuff, strlen(g_writeBuff)) == 0))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_close(g_fDescs[0]);
    OS_remove(g_fNames[0]);

    UtPrintf("OS_read() success test -- Write to file:\n\t%s\n", g_writeBuff);
    ;
    UtPrintf("OS_read() success test -- Read from file:\n\t%s\n", g_readBuff);
    ;

UT_os_readfile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_write(int32 filedes, void *buffer, uint32 nbytes)
** Purpose: Writes nBytes from the given buffer to the file of a given file descriptor
** Parameters: filedes - a file descriptor
**             *buffer - pointer that holds the data to be written to file
**             nbytes - the maximum number of bytes to copy to file
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the file descriptor passed in is invalid
**          OS_ERROR if the OS call failed
**          The number of bytes written if succeeded
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
** Test #2: Invalid-file-desc-arg condition
**   1) Call this routine 3 times with a file descriptors of 0, -1, and (OS_MAX_NUM_OPEN_FILES+5)
**      respectively as argument
**   2) Expect all three returned values to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to fail OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call this routine to write something to the file opened in #1
**   4) Expect the returned value to be
**        (a) number of bytes written that is equal to the number of bytes in the write buffer
**   5) Call OS_close() to flush and close the file opened in #1
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call OS_open() with file name used in #1 as argument
**   8) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   9) Call OS_read() with the file descriptor returned in #7
**  10) Expect the returned value to be
**        (a) the number of bytes read that is equal to the number of bytes written in #3 __and__
**        (b) the read buffer content is the same as the write buffer content in #3
**--------------------------------------------------------------------------------*/
void UT_os_writefile_test()
{
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_write(UT_OBJID_INCORRECT, NULL, 0) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_writefile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Write_NullPtr.txt", g_mntName);
    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#1 Null-pointer-arg - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        if (OS_write(g_fDescs[0], NULL, sizeof(g_writeBuff)) == OS_INVALID_POINTER)
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        OS_close(g_fDescs[0]);
        OS_remove(g_fNames[0]);
    }

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-file-desc-arg";

    if (OS_write(UT_OBJID_INCORRECT, g_writeBuff, sizeof(g_writeBuff)) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Write_Nominal.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < OS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_writefile_test_exit_tag;
    }

    memset(g_writeBuff, '\0', sizeof(g_writeBuff));
    strcpy(g_writeBuff, "TO BE OR NOT TO BE, THAT IS A QUESTION.");
    if (OS_write(g_fDescs[0], g_writeBuff, strlen(g_writeBuff)) != strlen(g_writeBuff))
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        /* Reset test environment */
        OS_close(g_fDescs[0]);
        OS_remove(g_fNames[0]);

        goto UT_os_writefile_test_exit_tag;
    }

    if (OS_close(g_fDescs[0]) != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-close failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);

        /* Reset test environment */
        OS_remove(g_fNames[0]);

        goto UT_os_writefile_test_exit_tag;
    }

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_NONE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#4 Nominal - File-open failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);

        /* Reset test environment */
        OS_remove(g_fNames[0]);

        goto UT_os_writefile_test_exit_tag;
    }

    memset(g_readBuff, '\0', sizeof(g_readBuff));
    if ((OS_read(g_fDescs[0], g_readBuff, strlen(g_writeBuff)) == strlen(g_writeBuff)) &&
        (strncmp(g_readBuff, g_writeBuff, strlen(g_writeBuff)) == 0))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_close(g_fDescs[0]);
    OS_remove(g_fNames[0]);

    UtPrintf("OS_write() success test -- Write to file:\n\t%s\n", g_writeBuff);
    ;
    UtPrintf("OS_write() success test -- Read from file:\n\t%s\n", g_readBuff);
    ;

UT_os_writefile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_lseek(int32 filedes, int32 offset, uint32 whence)
** Purpose: Moves the read/write pointer of a given file descriptor to the given offset
** Parameters: filedes - file descriptor of the given file
**             offset - number of bytes to offset the read/write pointer from its position
**                      dictated by whence
**             whence - tells offset where to begin offsetting
**                         OS_SEEK_SET - starts at beginning of file
**                         OS_SEEK_CUR - starts at the current read/write pointer
**                         OS_SEEK_END - starts at the end of the file
** Returns: OS_ERR_INVALID_ID is the file descriptor passed in is invalid
**          OS_ERROR if the OS call failed or the whence value is invalid
**          The new offset from the beginning of the given file
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
** -----------------------------------------------------
** Test #1: Invalid-file-desc-arg condition
**   1) Call this routine 3 times with a file descriptors of 0, -1, and (OS_MAX_NUM_OPEN_FILES+5)
**      respectively as argument
**   2) Expect all three returned values to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #2: Invalid-whence-arg condition
**   1) Call this routine with invalid "whence" value as argument
**   2) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to fail OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call OS_write() to write something to the file opened in #1
**   4) Expect the returned value to be
**        (a) the number of bytes that is equal to the number of bytes in the write buffer __and__
**        (b) the file position is now at the end of the file
**   5) Call this routine repeatedly with the file descriptor returned in #1, and different
**      offset and whence value each time
**   6) Expect the returned value each time to be
**        (a) the expected file position value given the arguments
**--------------------------------------------------------------------------------*/
void UT_os_lseekfile_test()
{
    const char *testDesc;
    size_t      buffLen;
    int32       pos1 = 0, pos2 = 0, pos3 = 0;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_lseek(UT_OBJID_INCORRECT, 0, OS_SEEK_CUR) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_lseekfile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-file-desc-arg";

    if (OS_lseek(UT_OBJID_INCORRECT, 0, OS_SEEK_SET) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-whence-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Seek_InvWhence.txt", g_mntName);
    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#2 Invalid-whence-arg - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        if (OS_lseek(g_fDescs[0], 0, 123456) == OS_ERROR)
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

        OS_close(g_fDescs[0]);
    }

    /* Reset test environment */
    OS_remove(g_fNames[0]);

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Seek_Nominal.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#4 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_lseekfile_test_exit_tag;
    }

    memset(g_writeBuff, '\0', sizeof(g_writeBuff));
    strcpy(g_writeBuff, "THE BROWN FOX JUMPS OVER THE LAZY DOG.");
    buffLen = strlen(g_writeBuff);

    if (OS_write(g_fDescs[0], g_writeBuff, buffLen) != buffLen)
    {
        testDesc = "#4 Nominal - File-write failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_lseekfile_test_exit_tag;
    }

    pos1 = OS_lseek(g_fDescs[0], 10, OS_SEEK_SET);
    pos2 = OS_lseek(g_fDescs[0], 7, OS_SEEK_CUR);
    pos3 = OS_lseek(g_fDescs[0], -16, OS_SEEK_END);
    if ((pos1 < 0) || (g_writeBuff[pos1] != 'F') || (pos2 < 0) || (g_writeBuff[pos2] != 'P') || (pos3 < 0) ||
        (g_writeBuff[pos3] != 'E'))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);

    /* Reset test environment */
    OS_close(g_fDescs[0]);
    OS_remove(g_fNames[0]);

UT_os_lseekfile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Purpose: Test OS_chmod
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**--------------------------------------------------------------------------------*/
void UT_os_chmodfile_test()
{
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_chmod(NULL, 0644) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_chmodfile_test_exit_tag;
    }

UT_os_chmodfile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_stat(const char *path, os_fstat_t *filestats)
** Purpose: Returns file information about a given file name
** Parameters: *path - pointer to the file/name of a given file
**             *filestats - pointer that will hold file information
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_FS_ERR_PATH_INVALID if the path is invalid
**          OS_FS_ERR_PATH_TOO_LONG if the path name is too long
**          OS_ERROR if the OS call failed
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
** Test #2: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #3: Path-too-long-arg condition
**   1) Call this routine with a path of length greater than OS_MAX_PATH_LEN as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #4: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #5: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call this routine with the file name used in #1
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_write() to cause a file modification to occur
**   6) Expect the returned value to be
**        (a) number of bytes written to be equal to the length of write buffer
**   7) Call OS_close() to flush and close the file written to in #5
**   8) Expect the returned value to be
**        (a) OS_SUCCESS
**   9) Call this routine again with the file name used in #1
**  10) Expect the returned value to be
**        (a) OS_SUCCESS __and__
**        (b) fstats1 returned in #3 and fstats2 returned in #9 to be not equal
**--------------------------------------------------------------------------------*/
void UT_os_statfile_test()
{
    const char *testDesc;
    os_fstat_t  fstats1, fstats2;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_stat(NULL, NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_statfile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if ((OS_stat(NULL, &fstats1) == OS_INVALID_POINTER) && (OS_stat(g_fNames[0], NULL) == OS_INVALID_POINTER))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path-arg";

    if (OS_stat(g_invalidPath, &fstats1) == OS_FS_ERR_PATH_INVALID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Path-too-long-arg";

    if (OS_stat(g_longPathName, &fstats1) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Stat_Nominal.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#5 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_statfile_test_exit_tag;
    }

    if (OS_stat(g_fNames[0], &fstats1) != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_statfile_test_exit_tag;
    }

    memset(g_writeBuff, '\0', sizeof(g_writeBuff));
    strcpy(g_writeBuff, "HOW NOW, BROWN COW?");
    if (OS_write(g_fDescs[0], g_writeBuff, strlen(g_writeBuff)) != strlen(g_writeBuff))
    {
        testDesc = "#5 Nominal - File-write failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_statfile_test_exit_tag;
    }

    if (OS_close(g_fDescs[0]) != OS_SUCCESS)
    {
        testDesc = "#5 Nominal - File-close failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_statfile_test_exit_tag;
    }

    if (OS_stat(g_fNames[0], &fstats2) != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_statfile_test_exit_tag;
    }

    if (memcmp(&fstats1, &fstats2, sizeof(fstats1)) != 0)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_remove(g_fNames[0]);

UT_os_statfile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_remove(const char *path)
** Purpose: Removes the given file name
** Parameters: *path - pointer to the path/name of the file to be removed
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_FS_ERR_PATH_INVALID if the path is invalid
**          OS_FS_ERR_PATH_TOO_LONG if the path name is too long
**          OS_FS_ERR_NAME_TOO_LONG if the name is too long
**          OS_ERROR if the OS call failed
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
** Test #2: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #3: Path-too-long-arg condition
**   1) Call this routine with a path of length greater than OS_MAX_PATH_LEN as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #4: Name-too-long-arg condition
**   1) Call this routine with just a file name (no absolute path) as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #5: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #6: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call this routine with path/file name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_stat() to get file status on the deleted file
**   6) Expect the returned values to be
**        (a) OS_ERROR
**--------------------------------------------------------------------------------*/
void UT_os_removefile_test()
{
    os_fstat_t  fstats;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_remove(NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_removefile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_remove(NULL) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path-arg";

    if (OS_remove(g_invalidPath) == OS_FS_ERR_PATH_INVALID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Path-too-long-arg";

    if (OS_remove(g_longPathName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-too-long-arg";

    if (OS_remove(g_longFileName) == OS_FS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#6 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Remove_Nominal.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#6 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_removefile_test_exit_tag;
    }

    OS_close(g_fDescs[0]);

    if (OS_remove(g_fNames[0]) != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_removefile_test_exit_tag;
    }

    if (OS_stat(g_fNames[0], &fstats) == OS_ERROR)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

UT_os_removefile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_rename(const char *old, const char *new)
** Purpose: Renames the given file name to the new file name
** Parameters: *old - pointer to the path/name of the file to be renamed
**             *new - pointer to the new path/name of the file
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_FS_ERR_PATH_TOO_LONG if the path name is too long
**          OS_FS_ERR_NAME_TOO_LONG if the name is too long
**          OS_ERROR if the OS call failed
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
** Test #2: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #3: Path-too-long-arg condition
**   1) Call this routine with a path of length greater than OS_MAX_PATH_LEN as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #4: Name-too-long-arg condition
**   1) Call this routine with just a file name (no absolute path) as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #5: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #6: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call this routine with path/file name used in #1 as argument for old name
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_stat() to get file status on the old name used in #3
**   6) Expect the returned values to be
**        (a) OS_ERROR
**--------------------------------------------------------------------------------*/
void UT_os_renamefile_test()
{
    os_fstat_t  fstats;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_rename(NULL, NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_renamefile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/oldName.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/newName.txt", g_mntName);

    if ((OS_rename(NULL, g_fNames[1]) == OS_INVALID_POINTER) && (OS_rename(g_fNames[0], NULL) == OS_INVALID_POINTER))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path-arg";

    if (OS_rename(g_invalidPath, g_invalidPath) == OS_FS_ERR_PATH_INVALID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Path-too-long-arg";

    if (OS_rename(g_longPathName, g_longPathName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-too-long-arg";

    if (OS_rename(g_longFileName, g_longFileName) == OS_FS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#6 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[0], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/Rename_Nom_Old.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/Rename_Nom_New.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#6 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_renamefile_test_exit_tag;
    }

    if (OS_rename(g_fNames[0], g_fNames[1]) != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_renamefile_test_exit_tag;
    }

    if (OS_stat(g_fNames[0], &fstats) == OS_ERROR)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_close(g_fDescs[0]);
    OS_remove(g_fNames[1]);

UT_os_renamefile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_cp(const char *src, const char *dest)
** Purpose: Copies the given file to a new specified file
** Parameters: *src - pointer to the absolute path of the file to be copied
**             *dest - pointer to the absolute path of the new file
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_FS_ERR_PATH_INVALID if the path is invalid
**          OS_FS_ERR_PATH_TOO_LONG if the path name is too long
**          OS_FS_ERR_NAME_TOO_LONG if the name is too long
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
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
** Test #2: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #3: Path-too-long-arg condition
**   1) Call this routine with a path of length greater than OS_MAX_PATH_LEN as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #4: Name-too-long-arg condition
**   1) Call this routine with just a file name (no absolute path) as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #5: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #6: Nominal condition
**   1) Call OS_stat() with a non-existing file name as argument
**   2) Expect the returned value to be
**        (a) OS_ERROR
**   3) Call OS_creat() to create and open a file
**   4) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   5) Call this routine with file name used in #3 as old file and file name used
**      in #1 as new file
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call OS_stat() again as in #1
**   8) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_copyfile_test()
{
    os_fstat_t  fstats;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_cp(NULL, NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_copyfile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/oldName.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/newName.txt", g_mntName);

    if ((OS_cp(NULL, g_fNames[1]) == OS_INVALID_POINTER) && (OS_cp(g_fNames[0], NULL) == OS_INVALID_POINTER))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path-arg";

    if (OS_cp(g_invalidPath, g_invalidPath) == OS_FS_ERR_PATH_INVALID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Path-too-long-arg";

    if (OS_cp(g_longPathName, g_longPathName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-too-long-arg";

    if (OS_cp(g_longFileName, g_longFileName) == OS_FS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#6 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/Cp_Nom_Old.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/Cp_Nom_New.txt", g_mntName);

    if (OS_stat(g_fNames[1], &fstats) != OS_ERROR)
    {
        testDesc = "#6 Nominal - File-stat failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_copyfile_test_exit_tag;
    }

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#6 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_copyfile_test_exit_tag;
    }

    if (OS_close(g_fDescs[0]) != OS_SUCCESS)
    {
        testDesc = "#6 Nominal - File-close failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_copyfile_test_exit_tag;
    }

    if (OS_cp(g_fNames[0], g_fNames[1]) != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_copyfile_test_exit_tag;
    }

    if (OS_stat(g_fNames[1], &fstats) == OS_SUCCESS)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_remove(g_fNames[0]);
    OS_remove(g_fNames[1]);

UT_os_copyfile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_mv(const char *src, const char *dest)
** Purpose: Moves the given file to a new specified file
** Parameters: *src - pointer to the absolute path of the file to be moved
**             *dest - pointer to the aboslute path of the new file
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_FS_ERR_INVALID_PATH if path is invalid
**          OS_FS_ERR_PATH_TOO_LONG if the path name is too long
**          OS_FS_ERR_NAME_TOO_LONG if the name is too long
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
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
** Test #2: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #3: Path-too-long-arg condition
**   1) Call this routine with a path of length greater than OS_MAX_PATH_LEN as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_TOO_LONG
** -----------------------------------------------------
** Test #4: Name-too-long-arg condition
**   1) Call this routine with just a file name (no absolute path) as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #5: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #6: Nominal condition
**   1) Call OS_stat() with a non-existing file name as argument
**   2) Expect the returned value to be
**        (a) OS_ERROR
**   3) Call OS_creat() to create and open a file
**   4) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   5) Call this routine with file name used in #3 as old file and file name used
**      in #1 as new file
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call OS_stat() again as in #1
**   8) Expect the returned value to be
**        (a) OS_SUCCESS
**   9) Call OS_stat() on the file name used in #3
**  10) Expect the returned value to be
**        (a) not OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_movefile_test()
{
    os_fstat_t  fstats;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_mv(NULL, NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_movefile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/oldName.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/newName.txt", g_mntName);

    if ((OS_mv(NULL, g_fNames[1]) == OS_INVALID_POINTER) && (OS_mv(g_fNames[0], NULL) == OS_INVALID_POINTER))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path-arg";

    if (OS_mv(g_invalidPath, g_invalidPath) == OS_FS_ERR_PATH_INVALID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Path-too-long-arg";

    if (OS_mv(g_longPathName, g_longPathName) == OS_FS_ERR_PATH_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-too-long-arg";

    if (OS_mv(g_longFileName, g_longFileName) == OS_FS_ERR_NAME_TOO_LONG)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#6 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[0], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/Mv_Nom_Old.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/Mv_Nom_New.txt", g_mntName);

    if (OS_stat(g_fNames[1], &fstats) != OS_ERROR)
    {
        testDesc = "#6 Nominal - File-stat failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_movefile_test_exit_tag;
    }

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#6 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_movefile_test_exit_tag;
    }

    /* Close file before moving */
    if (OS_close(g_fDescs[0]) != OS_SUCCESS)
    {
        testDesc = "#6 Nominal - File-close failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_movefile_test_exit_tag;
    }

    if (OS_mv(g_fNames[0], g_fNames[1]) != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_movefile_test_exit_tag;
    }

    if ((OS_stat(g_fNames[1], &fstats) == OS_SUCCESS) && (OS_stat(g_fNames[0], &fstats) != OS_SUCCESS))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_remove(g_fNames[1]);

UT_os_movefile_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_ShellOutputToFile(char *Cmd, int32 OS_fd)
** Purpose: Passes a command to the OS shell and directs the output from the command
**          to the given file descriptor
** Parameters: *Cmd - pointer to the command to pass to the OS
**             OS_fd - file descriptor to which the command output is written to
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the file descriptor passed in is invalid
**          OS_ERROR if the OS call failed
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
** Test #2: Invalid-file-desc-arg condition
**   1) Call this routine 3 times with a file descriptors of 0, -1, and (OS_MAX_NUM_OPEN_FILES+5)
**      respectively as argument
**   2) Expect all three returned values to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to fail OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_creat() to create and open a file for writing
**   2) Expect the returned value to be
**        (a) a file descriptor greater than or equal to 0
**   3) Call this routine with file descriptor returned in #1 and
**      command "echo $HOME" as arguments
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_lseek() with file descriptor returned in #1 to rewind to  the
**      beginning of file to get ready for reading
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call OS_read() with file descriptor returned in #1 to read from beginning of file
**   8) Expect the returned value to be
**        (a) number of bytes greater than 0 __and__
**        (b) the read string is the same as the string returned from getevn("HOME") call
**--------------------------------------------------------------------------------*/
void UT_os_outputtofile_test()
{
    int         res;
    const char *cmd = NULL;
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_ShellOutputToFile(NULL, OS_OBJECT_ID_UNDEFINED) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_outputtofile_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_ShellOutputToFile(NULL, OS_OBJECT_ID_UNDEFINED) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-file-desc-arg";

    if (OS_ShellOutputToFile("ls", UT_OBJID_INCORRECT) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Output_Nominal.txt", g_mntName);
    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#4 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_outputtofile_test_exit_tag;
    }

    cmd = "echo \"UT_os_outputtofile_test\"";
    res = OS_ShellOutputToFile(cmd, g_fDescs[0]);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_outputtofile_test_exit_tag;
    }

    if (res != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_outputtofile_test_exit_tag;
    }

    if (OS_lseek(g_fDescs[0], 0, OS_SEEK_SET) != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-lseek failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
    }
    else
    {
        memset(g_readBuff, '\0', sizeof(g_readBuff));
        if (OS_read(g_fDescs[0], g_readBuff, sizeof(g_readBuff)) <= 0)
        {
            testDesc = "#4 Nominal - File-read failed";
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        }
        else if (strstr(g_readBuff, "UT_os_outputtofile_test") != NULL)
        {
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        }
        else
        {
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        }
    }

UT_os_outputtofile_test_exit_tag:

    /* Reset test environment */
    OS_close(g_fDescs[0]);
    OS_remove(g_fNames[0]);

    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_FDGetInfo(int32 filedesc, OS_file_prop_t *fd_prop)
** Purpose: Returns file descriptor information about a given file descriptor
** Parameters: filedesc - a file descriptor
**             *fd_prop - pointer that will hold the file descriptor's data
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the file descriptor passed in is invalid
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
** Test #2: Invalid-file-desc-arg condition
**   1) Call this routine 3 times with a file descriptors of 0, -1, and (OS_MAX_NUM_OPEN_FILES+5)
**      respectively as argument
**   2) Expect all three returned values to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to fail OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor greater than or equal to 0
**   3) Call this routine with the file descriptor returned in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS __and__
**        (b) file descriptor property shows IsValid is TRUE __and__
**        (c) file path is the same as the file path used in #1
**   5) Call OS_close() with file descriptor returned in #1 as argument
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**   7) Call this routine with the file descriptor returned in #1 as argument
**   8) Expect the returned value to be
**        (a) OS_ERR_INVALID_ID
**--------------------------------------------------------------------------------*/
void UT_os_getfdinfo_test()
{
    OS_file_prop_t fdProps;
    const char *   testDesc;
    const char *   fileName = "GetInfo_Nom.txt";

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_FDGetInfo(OS_OBJECT_ID_UNDEFINED, NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_getfdinfo_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/GetInfo_Null.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#1 Null-pointer-arg - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        if (OS_FDGetInfo(g_fDescs[0], NULL) != OS_INVALID_POINTER)
        {
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        else
        {
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        }

        OS_close(g_fDescs[0]);
    }
    OS_remove(g_fNames[0]);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-file-desc-arg";

    if (OS_FDGetInfo(UT_OBJID_INCORRECT, &fdProps) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/%s", g_mntName, fileName);
    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#4 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_getfdinfo_test_exit_tag;
    }

    memset(&fdProps, 0x00, sizeof(fdProps));
    if (OS_FDGetInfo(g_fDescs[0], &fdProps) != OS_SUCCESS || strcmp(fdProps.Path, g_fNames[0]) != 0)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_getfdinfo_test_exit_tag;
    }

    if (OS_close(g_fDescs[0]) != OS_SUCCESS)
    {
        testDesc = "#4 Nominal - File-close failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_getfdinfo_test_exit_tag;
    }

    memset(&fdProps, 0x00, sizeof(fdProps));
    if (OS_FDGetInfo(g_fDescs[0], &fdProps) == OS_ERR_INVALID_ID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_remove(g_fNames[0]);

UT_os_getfdinfo_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_FileOpenCheck(char *Filename)
** Purpose: Determines if a given file is opened
** Parameters: *Filename - pointer to the name of the file to check
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERROR if the file is not opened
**          OS_SUCCESS if the file is opened
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
** Test #2: File-not-opened condition
**   1) Call this routine with some non-existing filename as argument
**   2) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_creat() to create and open some file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call this routine with file name used in #1
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_checkfileopen_test()
{
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_FileOpenCheck(NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_checkfileopen_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_FileOpenCheck(NULL) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 File-not-opened";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/FChk_UnOpened.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#2 File-not-opened - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        if (OS_close(g_fDescs[0]) != OS_SUCCESS)
        {
            testDesc = "#2 File-not-opened - File-close failed";
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        }
        else if (OS_FileOpenCheck(g_fNames[0]) != OS_ERROR)
        {
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        }
        else
        {
            UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
        }
    }

    /* Reset test environment */
    OS_remove(g_fNames[0]);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/FileChk_Nominal.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#3 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_checkfileopen_test_exit_tag;
    }

    if (OS_FileOpenCheck(g_fNames[0]) == OS_SUCCESS)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_close(g_fDescs[0]);
    OS_remove(g_fNames[0]);

UT_os_checkfileopen_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_CloseAllFiles(void)
** Purpose: Closes all opened files
** Parameters: None
** Returns: OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
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
**   1) Call OS_creat() 3 times to create and open some files
**   2) Expect all returned values to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call this routine
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_FileOpenCheck() on the file descriptors returned in #1
**   6) Expect all returned values to be
**        (a) OS_ERROR
**--------------------------------------------------------------------------------*/
void UT_os_closeallfiles_test()
{
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_CloseAllFiles() == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_closeallfiles_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#2 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    memset(g_fNames[2], '\0', sizeof(g_fNames[2]));
    UT_os_sprintf(g_fNames[0], "%s/CloseAll_Nom_1.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/CloseAll_Nom_2.txt", g_mntName);
    UT_os_sprintf(g_fNames[2], "%s/CloseAll_Nom_3.txt", g_mntName);

    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    g_fStatus[1] = OS_OpenCreate(&g_fDescs[1], g_fNames[1], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    g_fStatus[2] = OS_OpenCreate(&g_fDescs[2], g_fNames[2], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if ((g_fStatus[0] < 0) || (g_fStatus[1] < 0) || (g_fStatus[2] < 0))
    {
        testDesc = "#2 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_closeallfiles_test_exit_tag;
    }

    if (OS_CloseAllFiles() != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_closeallfiles_test_exit_tag;
    }

    if ((OS_FileOpenCheck(g_fNames[0]) == OS_ERROR) && (OS_FileOpenCheck(g_fNames[1]) == OS_ERROR) &&
        (OS_FileOpenCheck(g_fNames[2]) == OS_ERROR))
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_remove(g_fNames[0]);
    OS_remove(g_fNames[1]);
    OS_remove(g_fNames[2]);

UT_os_closeallfiles_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_CloseFileByName(char *Filename)
** Purpose: Closes a given file
** Parameters: *Filename - pointer to the name of the file to be closed
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_FS_ERR_PATH_INVALID if the path is invalid
**          OS_ERROR if the OS call failed
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
** Test #2: Invalid-path-arg condition
**   1) Call this routine with a non-existing path as argument
**   2) Expect the returned value to be
**        (a) OS_FS_ERR_PATH_INVALID
** -----------------------------------------------------
** Test #3: OS-call-failure condition
**   1) Setup the test to fail the OS call inside this routine
**   2) Call this routine
**   3) Expect the returned value to be
**        (a) OS_ERROR
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_creat() to create and open a file
**   2) Expect the returned value to be
**        (a) a file descriptor value greater than or equal to 0
**   3) Call this routine with the file name used in #1
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_FileOpenCheck() with the file name used in #1
**   6) Expect the returned value to be
**        (a) OS_ERROR
**--------------------------------------------------------------------------------*/
void UT_os_closefilebyname_test()
{
    const char *testDesc;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    if (OS_CloseFileByName(NULL) == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_closefilebyname_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Null-pointer-arg";

    if (OS_CloseFileByName(NULL) == OS_INVALID_POINTER)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-path-arg";

    if (OS_CloseFileByName(g_invalidPath) == OS_FS_ERR_PATH_INVALID)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 OS-call-failure";

    UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Close_Nominal.txt", g_mntName);
    g_fStatus[0] = OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    if (g_fStatus[0] < 0)
    {
        testDesc = "#4 Nominal - File-create failed";
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_TSF);
        goto UT_os_closefilebyname_test_exit_tag;
    }

    if (OS_CloseFileByName(g_fNames[0]) != OS_SUCCESS)
    {
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);
        goto UT_os_closefilebyname_test_exit_tag;
    }

    if (OS_FileOpenCheck(g_fNames[0]) == OS_ERROR)
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT(testDesc, UTASSERT_CASETYPE_FAILURE);

    /* Reset test environment */
    OS_remove(g_fNames[0]);

UT_os_closefilebyname_test_exit_tag:
    return;
}

/*================================================================================*
** End of File: ut_osfile_fileio_test.c
**================================================================================*/
