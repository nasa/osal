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
    UT_NOMINAL(OS_API_Init());
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
    int32     i = 0, j = 0;
    osal_id_t fd;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_OpenCreate(NULL, "file", OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE),
              OS_INVALID_POINTER);
    UT_RETVAL(OS_OpenCreate(&fd, NULL, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path-arg */

    UT_RETVAL(OS_OpenCreate(&fd, g_invalidPath, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE),
              OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #3 Path-too-long-arg */

    UT_RETVAL(OS_OpenCreate(&fd, g_longPathName, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE),
              OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-too-long-arg */

    UT_RETVAL(OS_OpenCreate(&fd, g_longFileName, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE),
              OS_FS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #5 Invalid-permission-arg */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Create_InvPerm.txt", g_mntName);
    UT_RETVAL(OS_OpenCreate(&fd, g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, 123), OS_ERROR);

    /*-----------------------------------------------------*/
    /* #7 File-descriptors-full */

    for (i = 0; i <= OS_MAX_NUM_OPEN_FILES; i++)
    {
        memset(g_fNames[i], '\0', sizeof(g_fNames[i]));
        UT_os_sprintf(g_fNames[i], "%s/tmpFile%d.txt", g_mntName, (int)i);
        if (i == OS_MAX_NUM_OPEN_FILES)
        {
            UT_RETVAL(
                OS_OpenCreate(&g_fDescs[i], g_fNames[i], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY),
                OS_ERR_NO_FREE_IDS);
        }
        else if (!UT_SETUP(OS_OpenCreate(&g_fDescs[i], g_fNames[i], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE,
                                         OS_WRITE_ONLY)))
        {
            break;
        }
    }

    /* Reset test environment */
    for (j = 0; j < i && j < OS_MAX_NUM_OPEN_FILES; j++)
    {
        UT_TEARDOWN(OS_close(g_fDescs[j]));
        UT_TEARDOWN(OS_remove(g_fNames[j]));
    }

    /*-----------------------------------------------------*/
    /* #8 Nominal */

    UT_NOMINAL(OS_OpenCreate(&g_fDescs[5], g_fNames[5], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY));
    UT_NOMINAL(OS_OpenCreate(&g_fDescs[6], g_fNames[6], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY));

    /* Reset test environment */
    UT_TEARDOWN(OS_close(g_fDescs[5]));
    UT_TEARDOWN(OS_close(g_fDescs[6]));

    UT_TEARDOWN(OS_remove(g_fNames[5]));
    UT_TEARDOWN(OS_remove(g_fNames[6]));
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
    int32     i = 0, j = 0;
    osal_id_t fd;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_OpenCreate(&fd, NULL, OS_FILE_FLAG_NONE, OS_READ_WRITE), OS_INVALID_POINTER);
    UT_RETVAL(OS_OpenCreate(NULL, "file", OS_FILE_FLAG_NONE, OS_READ_WRITE), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path-arg */

    UT_RETVAL(OS_OpenCreate(&fd, g_invalidPath, OS_FILE_FLAG_NONE, OS_READ_WRITE), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #3 Path-too-long-arg */

    UT_RETVAL(OS_OpenCreate(&fd, g_longPathName, OS_FILE_FLAG_NONE, OS_READ_WRITE), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-too-long-arg */

    UT_RETVAL(OS_OpenCreate(&fd, g_longFileName, OS_FILE_FLAG_NONE, OS_READ_WRITE), OS_FS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #5 Invalid-permission-arg */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Open_InvPerm.txt", g_mntName);
    UT_RETVAL(OS_OpenCreate(&fd, g_fNames[0], OS_FILE_FLAG_NONE, 123), OS_ERROR);

    /*-----------------------------------------------------*/
    /* #7 File-descriptors-full */

    /* This first needs to create all the files */
    for (i = 0; i < OS_MAX_NUM_OPEN_FILES; i++)
    {
        memset(g_fNames[i], '\0', sizeof(g_fNames[i]));
        UT_os_sprintf(g_fNames[i], "%s/tmpFile%d.txt", g_mntName, (int)i);
        if (!UT_SETUP(
                OS_OpenCreate(&g_fDescs[i], g_fNames[i], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY)))
        {
            break;
        }

        if (!UT_SETUP(OS_close(g_fDescs[i]) != OS_SUCCESS))
        {
            break;
        }
    }

    if (i < OS_MAX_NUM_OPEN_FILES)
    {
        /* setup failure, stop test now (already reported) */
        return;
    }

    for (i = 0; i <= OS_MAX_NUM_OPEN_FILES; i++)
    {
        if (i == OS_MAX_NUM_OPEN_FILES)
        {
            UT_RETVAL(OS_OpenCreate(&g_fDescs[i], g_fNames[i], OS_FILE_FLAG_NONE, OS_WRITE_ONLY), OS_ERR_NO_FREE_IDS);
        }
        else if (!UT_SETUP(OS_OpenCreate(&g_fDescs[i], g_fNames[i], OS_FILE_FLAG_NONE, OS_WRITE_ONLY)))
        {
            break;
        }
    }

    /* Reset test environment */
    for (j = 0; j < i && j < OS_MAX_NUM_OPEN_FILES; j++)
    {
        UT_TEARDOWN(OS_close(g_fDescs[j]));
        UT_TEARDOWN(OS_remove(g_fNames[j]));
    }

    /*-----------------------------------------------------*/
    /* #8 Nominal */

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[5], g_fNames[5], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_SETUP(OS_close(g_fDescs[5]));

        if (UT_SETUP(
                OS_OpenCreate(&g_fDescs[6], g_fNames[6], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY)))
        {
            UT_SETUP(OS_close(g_fDescs[6]));

            UT_NOMINAL(
                OS_OpenCreate(&g_fDescs[5], g_fNames[5], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE));
            UT_NOMINAL(
                OS_OpenCreate(&g_fDescs[6], g_fNames[6], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY));

            UT_TEARDOWN(OS_close(g_fDescs[5]));
            UT_TEARDOWN(OS_close(g_fDescs[6]));

            UT_TEARDOWN(OS_remove(g_fNames[6]));
        }

        UT_TEARDOWN(OS_remove(g_fNames[5]));
    }
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
    char tmpBuff[UT_OS_IO_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* #1 Invalid-file-desc-arg */

    UT_RETVAL(OS_close(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_close(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Close_Nominal.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_NOMINAL(OS_close(g_fDescs[0]));

        UT_RETVAL(OS_write(g_fDescs[0], tmpBuff, sizeof(tmpBuff)), OS_ERR_INVALID_ID);
        UT_RETVAL(OS_read(g_fDescs[0], tmpBuff, sizeof(tmpBuff)), OS_ERR_INVALID_ID);

        /* Reset test environment */
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    size_t expected_len;

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Read_NullPtr.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        /*-----------------------------------------------------*/
        /* #1 Null-pointer-arg */
        UT_RETVAL(OS_read(g_fDescs[0], NULL, sizeof(g_readBuff)), OS_INVALID_POINTER);
        UT_RETVAL(OS_read(g_fDescs[0], g_readBuff, 0), OS_ERR_INVALID_SIZE);

        /* Reset test environment */
        UT_TEARDOWN(OS_close(g_fDescs[0]));
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }

    /*-----------------------------------------------------*/
    /* #2 Invalid-file-desc-arg */
    UT_RETVAL(OS_read(UT_OBJID_INCORRECT, g_readBuff, sizeof(g_readBuff)), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_read(OS_OBJECT_ID_UNDEFINED, g_readBuff, sizeof(g_readBuff)), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Read_Nominal.txt", g_mntName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        memset(g_writeBuff, '\0', sizeof(g_writeBuff));
        strcpy(g_writeBuff, "A HORSE! A HORSE! MY KINGDOM FOR A HORSE!");
        expected_len = strlen(g_writeBuff);

        UT_RETVAL(OS_write(g_fDescs[0], g_writeBuff, expected_len), expected_len);

        UT_SETUP(OS_close(g_fDescs[0]));

        if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_NONE, OS_READ_WRITE)))
        {
            memset(g_readBuff, '\0', sizeof(g_readBuff));
            UT_RETVAL(OS_read(g_fDescs[0], g_readBuff, sizeof(g_readBuff)), expected_len);
            UtAssert_StrCmp(g_readBuff, g_writeBuff, "%s == %s", g_readBuff, g_writeBuff);

            /* confirm that read returns 0 at end of file */
            UT_RETVAL(OS_read(g_fDescs[0], g_readBuff, sizeof(g_readBuff)), 0);

            UT_TEARDOWN(OS_close(g_fDescs[0]));
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    size_t expected_len;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Write_NullPtr.txt", g_mntName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_RETVAL(OS_write(g_fDescs[0], NULL, sizeof(g_writeBuff)), OS_INVALID_POINTER);
        UT_RETVAL(OS_write(g_fDescs[0], g_writeBuff, 0), OS_ERR_INVALID_SIZE);

        /* Reset test environment */
        UT_TEARDOWN(OS_close(g_fDescs[0]));
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }

    /*-----------------------------------------------------*/
    /* #2 Invalid-file-desc-arg */

    UT_RETVAL(OS_write(UT_OBJID_INCORRECT, g_writeBuff, sizeof(g_writeBuff)), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_write(OS_OBJECT_ID_UNDEFINED, g_writeBuff, sizeof(g_writeBuff)), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Write_Nominal.txt", g_mntName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_WRITE_ONLY)))
    {
        memset(g_writeBuff, '\0', sizeof(g_writeBuff));
        strcpy(g_writeBuff, "TO BE OR NOT TO BE, THAT IS A QUESTION.");
        expected_len = strlen(g_writeBuff);
        UT_RETVAL(OS_write(g_fDescs[0], g_writeBuff, expected_len), expected_len);

        UT_SETUP(OS_close(g_fDescs[0]));

        if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_NONE, OS_READ_ONLY)))
        {
            memset(g_readBuff, '\0', sizeof(g_readBuff));
            UT_RETVAL(OS_read(g_fDescs[0], g_readBuff, sizeof(g_readBuff)), expected_len);
            UtAssert_StrCmp(g_readBuff, g_writeBuff, "%s == %s", g_readBuff, g_writeBuff);

            /* confirm that read returns 0 at end of file */
            UT_RETVAL(OS_read(g_fDescs[0], g_readBuff, sizeof(g_readBuff)), 0);

            /* Reset test environment */
            UT_TEARDOWN(OS_close(g_fDescs[0]));
        }
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    size_t buffLen;
    int32  pos1 = 0, pos2 = 0, pos3 = 0;

    UT_RETVAL(OS_lseek(UT_OBJID_INCORRECT, 0, OS_SEEK_SET), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_lseek(OS_OBJECT_ID_UNDEFINED, 0, OS_SEEK_SET), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Invalid-whence-arg */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Seek_InvWhence.txt", g_mntName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_RETVAL(OS_lseek(g_fDescs[0], 0, 123456), OS_ERROR);

        /* Reset test environment */
        UT_TEARDOWN(OS_close(g_fDescs[0]));
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Seek_Nominal.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        memset(g_writeBuff, '\0', sizeof(g_writeBuff));
        strcpy(g_writeBuff, "THE BROWN FOX JUMPS OVER THE LAZY DOG.");
        buffLen = strlen(g_writeBuff);

        UT_RETVAL(OS_write(g_fDescs[0], g_writeBuff, buffLen), buffLen);

        UT_RETVAL(pos1 = OS_lseek(g_fDescs[0], 10, OS_SEEK_SET), 10);
        UT_RETVAL(pos2 = OS_lseek(g_fDescs[0], 7, OS_SEEK_CUR), 17);
        UT_RETVAL(pos3 = OS_lseek(g_fDescs[0], -16, OS_SEEK_END), 22);

        UtAssert_INT32_EQ(g_writeBuff[pos1], 'F');
        UtAssert_INT32_EQ(g_writeBuff[pos2], 'P');
        UtAssert_INT32_EQ(g_writeBuff[pos3], 'E');

        /* Reset test environment */
        UT_TEARDOWN(OS_close(g_fDescs[0]));
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    UT_RETVAL(OS_chmod(NULL, OS_READ_WRITE), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* allow API not implemented */
    UT_os_sprintf(g_fNames[0], "%s/chmod.txt", g_mntName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_SETUP(OS_close(g_fDescs[0]));

        /* change to read-only permission, this is allowed to return OS_ERR_NOT_IMPLEMENTED */
        if (UT_NOMINAL_OR_NOTIMPL(OS_chmod(g_fNames[0], OS_READ_ONLY)))
        {
            /* change it back */
            UT_NOMINAL(OS_chmod(g_fNames[0], OS_READ_WRITE));
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    os_fstat_t fstats1;
    os_fstat_t fstats2;
    size_t     expected_len;

    memset(&fstats1, 0, sizeof(fstats1));
    memset(&fstats2, 0, sizeof(fstats2));

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */
    UT_RETVAL(OS_stat(NULL, &fstats1), OS_INVALID_POINTER);
    UT_RETVAL(OS_stat(g_fNames[0], NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path-arg */

    UT_RETVAL(OS_stat(g_invalidPath, &fstats1), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #3 Path-too-long-arg */

    UT_RETVAL(OS_stat(g_longPathName, &fstats1), OS_FS_ERR_PATH_TOO_LONG);
    UT_RETVAL(OS_stat(g_longFileName, &fstats1), OS_FS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #5 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Stat_Nominal.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_NOMINAL(OS_stat(g_fNames[0], &fstats1));
        UtAssert_UINT32_EQ(fstats1.FileSize, 0);

        memset(g_writeBuff, '\0', sizeof(g_writeBuff));
        strcpy(g_writeBuff, "HOW NOW, BROWN COW?");
        expected_len = strlen(g_writeBuff);
        UT_RETVAL(OS_write(g_fDescs[0], g_writeBuff, expected_len), expected_len);
        UT_SETUP(OS_close(g_fDescs[0]));

        UT_NOMINAL(OS_stat(g_fNames[0], &fstats2));
        UtAssert_UINT32_EQ(fstats1.FileSize, 0);

        /* Reset test environment */
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    os_fstat_t fstats;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_remove(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path-arg */

    UT_RETVAL(OS_remove(g_invalidPath), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #3 Path-too-long-arg */

    UT_RETVAL(OS_remove(g_longPathName), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-too-long-arg */

    UT_RETVAL(OS_remove(g_longFileName), OS_FS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #6 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Remove_Nominal.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_SETUP(OS_close(g_fDescs[0]));

        UT_NOMINAL(OS_remove(g_fNames[0]));
    }

    UT_RETVAL(OS_stat(g_fNames[0], &fstats), OS_ERROR);

    /* removing again (nonexistent file) should return OS_ERROR */
    UT_RETVAL(OS_remove(g_fNames[0]), OS_ERROR);
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
    os_fstat_t fstats;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/oldName.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/newName.txt", g_mntName);

    UT_RETVAL(OS_rename(NULL, g_fNames[1]), OS_INVALID_POINTER);
    UT_RETVAL(OS_rename(g_fNames[0], NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path-arg */

    UT_RETVAL(OS_rename(g_invalidPath, g_fNames[1]), OS_FS_ERR_PATH_INVALID);
    UT_RETVAL(OS_rename(g_fNames[0], g_invalidPath), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #3 Path-too-long-arg */

    UT_RETVAL(OS_rename(g_longPathName, g_fNames[1]), OS_FS_ERR_PATH_TOO_LONG);
    UT_RETVAL(OS_rename(g_fNames[0], g_longPathName), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-too-long-arg */

    UT_RETVAL(OS_rename(g_longFileName, g_fNames[1]), OS_FS_ERR_NAME_TOO_LONG);
    UT_RETVAL(OS_rename(g_fNames[0], g_longFileName), OS_FS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #6 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/Rename_Nom_Old.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/Rename_Nom_New.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_SETUP(OS_close(g_fDescs[0]));
        UT_NOMINAL(OS_rename(g_fNames[0], g_fNames[1]));

        UT_RETVAL(OS_stat(g_fNames[0], &fstats), OS_ERROR);
        UT_RETVAL(OS_stat(g_fNames[1], &fstats), OS_SUCCESS);

        /* test with nonexistent source file */
        UT_RETVAL(OS_rename(g_fNames[0], g_fNames[1]), OS_ERROR);

        /* Reset test environment */
        UT_TEARDOWN(OS_remove(g_fNames[1]));
    }
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
    os_fstat_t fstats;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/oldName.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/newName.txt", g_mntName);

    UT_RETVAL(OS_cp(NULL, g_fNames[1]), OS_INVALID_POINTER);
    UT_RETVAL(OS_cp(g_fNames[0], NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path-arg */

    UT_RETVAL(OS_cp(g_invalidPath, g_fNames[1]), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #3 Path-too-long-arg */

    UT_RETVAL(OS_cp(g_longPathName, g_fNames[1]), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-too-long-arg */

    UT_RETVAL(OS_cp(g_longFileName, g_fNames[1]), OS_FS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #6 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/Cp_Nom_Old.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/Cp_Nom_New.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_SETUP(OS_close(g_fDescs[0]));

        /* validate pass-thru codes on second arg (must be done with a valid 1st arg) */

        UT_RETVAL(OS_cp(g_fNames[0], g_invalidPath), OS_FS_ERR_PATH_INVALID);
        UT_RETVAL(OS_cp(g_fNames[0], g_longPathName), OS_FS_ERR_PATH_TOO_LONG);
        UT_RETVAL(OS_cp(g_fNames[0], g_longFileName), OS_FS_ERR_NAME_TOO_LONG);

        UT_NOMINAL(OS_cp(g_fNames[0], g_fNames[1]));

        UT_RETVAL(OS_stat(g_fNames[0], &fstats), OS_SUCCESS);
        UT_RETVAL(OS_stat(g_fNames[1], &fstats), OS_SUCCESS);

        /* Reset test environment */
        UT_TEARDOWN(OS_remove(g_fNames[0]));

        /* Test with nonexistent source file */
        UT_RETVAL(OS_cp(g_fNames[0], g_fNames[1]), OS_ERROR);

        UT_TEARDOWN(OS_remove(g_fNames[1]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_mv(const char *src, const char *dest)
** Purpose: Moves the given file to a new specified file
** Parameters: *src - pointer to the absolute path of the file to be moved
**             *dest - pointer to the absolute path of the new file
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
    os_fstat_t fstats;

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/oldName.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/newName.txt", g_mntName);

    UT_RETVAL(OS_mv(NULL, g_fNames[1]), OS_INVALID_POINTER);
    UT_RETVAL(OS_mv(g_fNames[0], NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path-arg */

    UT_RETVAL(OS_mv(g_invalidPath, g_fNames[1]), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #3 Path-too-long-arg */

    UT_RETVAL(OS_mv(g_longPathName, g_fNames[1]), OS_FS_ERR_PATH_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-too-long-arg */

    UT_RETVAL(OS_mv(g_longFileName, g_fNames[1]), OS_FS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #6 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[0], '\0', sizeof(g_fNames[1]));
    UT_os_sprintf(g_fNames[0], "%s/Mv_Nom_Old.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/Mv_Nom_New.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        /* Close file before moving */
        UT_SETUP(OS_close(g_fDescs[0]));

        /* validate pass-thru codes on second arg (must be done with a valid 1st arg) */
        UT_RETVAL(OS_mv(g_fNames[0], g_invalidPath), OS_FS_ERR_PATH_INVALID);
        UT_RETVAL(OS_mv(g_fNames[0], g_longPathName), OS_FS_ERR_PATH_TOO_LONG);
        UT_RETVAL(OS_mv(g_fNames[0], g_longFileName), OS_FS_ERR_NAME_TOO_LONG);

        UT_NOMINAL(OS_mv(g_fNames[0], g_fNames[1]));

        UT_RETVAL(OS_stat(g_fNames[0], &fstats), OS_ERROR);
        UT_RETVAL(OS_stat(g_fNames[1], &fstats), OS_SUCCESS);

        /* test with nonexistent source file */
        UT_RETVAL(OS_mv(g_fNames[0], g_fNames[1]), OS_ERROR);

        /* Reset test environment */
        UT_TEARDOWN(OS_remove(g_fNames[1]));
    }
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
    /*-----------------------------------------------------*/
    /* #2 Invalid-file-desc-arg */

    UT_RETVAL(OS_ShellOutputToFile("ls", UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_ShellOutputToFile("ls", OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Output_Nominal.txt", g_mntName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        /*-----------------------------------------------------*/
        /* Null-pointer-arg */

        UT_RETVAL(OS_ShellOutputToFile(NULL, g_fDescs[0]), OS_INVALID_POINTER);

        /* Nominal */
        if (UT_NOMINAL_OR_NOTIMPL(OS_ShellOutputToFile("echo \"UT_os_outputtofile_test\"", g_fDescs[0])))
        {
            UT_RETVAL(OS_lseek(g_fDescs[0], 0, OS_SEEK_SET), 0);
            memset(g_readBuff, '\0', sizeof(g_readBuff));
            if (UT_SETUP(OS_read(g_fDescs[0], g_readBuff, sizeof(g_readBuff))))
            {
                UtAssert_True(strstr(g_readBuff, "UT_os_outputtofile_test") != NULL,
                              "Output file contains UT_os_outputtofile_test");
            }

            /*
             * Executing a command name "false" should fail, either because it is not a known
             * command, or if it is valid (e.g. a UNIX-like environment has /bin/false) the
             * command always fails.
             */
            UT_RETVAL(OS_ShellOutputToFile("false", g_fDescs[0]), OS_ERROR);
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_close(g_fDescs[0]));
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    const char *   fileName = "GetInfo_Nom.txt";

    /*-----------------------------------------------------*/

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/GetInfo_Null.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        /* #1 Null-pointer-arg */
        UT_RETVAL(OS_FDGetInfo(g_fDescs[0], NULL), OS_INVALID_POINTER);

        UT_TEARDOWN(OS_close(g_fDescs[0]));
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }

    /*-----------------------------------------------------*/
    /* #2 Invalid-file-desc-arg */

    UT_RETVAL(OS_FDGetInfo(UT_OBJID_INCORRECT, &fdProps), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_FDGetInfo(OS_OBJECT_ID_UNDEFINED, &fdProps), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/%s", g_mntName, fileName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_NOMINAL(OS_FDGetInfo(g_fDescs[0], &fdProps));

        UT_TEARDOWN(OS_close(g_fDescs[0]));
        UT_TEARDOWN(OS_remove(g_fNames[0]));

        UT_RETVAL(OS_FDGetInfo(g_fDescs[0], &fdProps), OS_ERR_INVALID_ID);
    }
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
    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_FileOpenCheck(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 File-not-opened */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/FChk_UnOpened.txt", g_mntName);

    UT_RETVAL(OS_FileOpenCheck(g_fNames[0]), OS_ERROR);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/FileChk_Nominal.txt", g_mntName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_NOMINAL(OS_FileOpenCheck(g_fNames[0]));

        UT_TEARDOWN(OS_close(g_fDescs[0]));
        UT_RETVAL(OS_FileOpenCheck(g_fNames[0]), OS_ERROR);

        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    /*-----------------------------------------------------*/
    /* #2 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    memset(g_fNames[1], '\0', sizeof(g_fNames[1]));
    memset(g_fNames[2], '\0', sizeof(g_fNames[2]));
    UT_os_sprintf(g_fNames[0], "%s/CloseAll_Nom_1.txt", g_mntName);
    UT_os_sprintf(g_fNames[1], "%s/CloseAll_Nom_2.txt", g_mntName);
    UT_os_sprintf(g_fNames[2], "%s/CloseAll_Nom_3.txt", g_mntName);

    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        if (UT_SETUP(
                OS_OpenCreate(&g_fDescs[1], g_fNames[1], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
        {
            if (UT_SETUP(OS_OpenCreate(&g_fDescs[2], g_fNames[2], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE,
                                       OS_READ_WRITE)))
            {
                UT_NOMINAL(OS_CloseAllFiles());

                UT_RETVAL(OS_FileOpenCheck(g_fNames[0]), OS_ERROR);
                UT_RETVAL(OS_FileOpenCheck(g_fNames[1]), OS_ERROR);
                UT_RETVAL(OS_FileOpenCheck(g_fNames[2]), OS_ERROR);

                UT_TEARDOWN(OS_remove(g_fNames[2]));
            }
            UT_TEARDOWN(OS_remove(g_fNames[1]));
        }
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
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
    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */
    UT_RETVAL(OS_CloseFileByName(NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Invalid-path-arg */

    UT_RETVAL(OS_CloseFileByName(g_invalidPath), OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    memset(g_fNames[0], '\0', sizeof(g_fNames[0]));
    UT_os_sprintf(g_fNames[0], "%s/Close_Nominal.txt", g_mntName);
    if (UT_SETUP(OS_OpenCreate(&g_fDescs[0], g_fNames[0], OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE)))
    {
        UT_NOMINAL(OS_CloseFileByName(g_fNames[0]));
        UT_RETVAL(OS_FileOpenCheck(g_fNames[0]), OS_ERROR);

        /* Reset test environment */
        UT_TEARDOWN(OS_remove(g_fNames[0]));
    }
}

/*================================================================================*
** End of File: ut_osfile_fileio_test.c
**================================================================================*/
