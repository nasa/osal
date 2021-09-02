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

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* OS Constructs */
void TestMkfsMount(void);
void TestCreatRemove(void);
void TestOpenClose(void);
void TestChmod(void);
void TestReadWriteLseek(void);
void TestMkRmDirFreeBytes(void);
void TestOpenReadCloseDir(void);
void TestRename(void);
void TestStat(void);
void TestOpenFileAPI(void);
void TestUnmountRemount(void);

os_err_name_t errname;

/* *************************************** MAIN ************************************** */

void UtTest_Setup(void)
{
    osal_id_t fs_id;

    errname[0] = 0;

    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * This test case requires a fixed virtual dir for one test case.
     * Just map /test to a dir of the same name, relative to current dir.
     */
    if (OS_FileSysAddFixedMap(&fs_id, "./test", "/test") != OS_SUCCESS)
    {
        UtAssert_Abort("OS_FileSysAddFixedMap() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     *
     * It is done this way so that the output is logically grouped,
     * otherwise the entire thing would be lumped together
     * as a single test case.
     */
    UtTest_Add(TestMkfsMount, NULL, NULL, "TestMkfsMount");
    UtTest_Add(TestCreatRemove, NULL, NULL, "TestCreatRemove");
    UtTest_Add(TestOpenClose, NULL, NULL, "TestOpenClose");
    UtTest_Add(TestChmod, NULL, NULL, "TestChmod");
    UtTest_Add(TestReadWriteLseek, NULL, NULL, "TestReadWriteLseek");
    UtTest_Add(TestMkRmDirFreeBytes, NULL, NULL, "TestMkRmDirFreeBytes");
    UtTest_Add(TestOpenReadCloseDir, NULL, NULL, "TestOpenReadCloseDir");
    UtTest_Add(TestStat, NULL, NULL, "TestStat");
    UtTest_Add(TestOpenFileAPI, NULL, NULL, "TestOpenFileAPI");
    UtTest_Add(TestUnmountRemount, NULL, NULL, "TestUnmountRemount");
    UtTest_Add(TestRename, NULL, NULL, "TestRename");
}

void TestMkfsMount(void)
{
    int32 status;

    /* Make the file system */
    status = OS_mkfs(0, "/ramdev0", "RAM", OSAL_SIZE_C(512), OSAL_BLOCKCOUNT_C(200));
    UtAssert_True(status == OS_SUCCESS, "status after mkfs = %d", (int)status);

    status = OS_mount("/ramdev0", "/drive0");
    UtAssert_True(status == OS_SUCCESS, "status after mount = %d", (int)status);
}

void TestUnmountRemount(void)
{
    int32 status;

    /*
    ** try unmounting the drive, and then remounting it with a different name
    */
    status = OS_unmount("/drive0");
    UtAssert_True(status >= OS_SUCCESS, "status after unmount = %d", (int)status);

    status = OS_mount("/ramdev0", "/drive1");
    UtAssert_True(status == OS_SUCCESS, "status after reunmount = %d", (int)status);
}

/*---------------------------------------------------------------------------------------
 * Name Test CreatRemove()
 * This function tests the basic create / remove capabilities of the filesystem. It tries
 *  to create files of reasonable length, OS_MAX_FILE_NAME length, and OS_MAX_FILE_NAME+1
 *  length.
 *
 *  The function then tries to remove the files created above, as well as remove a file
 *  that was never on the drive to begin with.
---------------------------------------------------------------------------------------*/

void TestCreatRemove(void)
{
    char      filename[OS_MAX_PATH_LEN];
    char      maxfilename[OS_MAX_PATH_LEN];
    char      longfilename[OS_MAX_PATH_LEN + 10];
    int32     status;
    osal_id_t fd;
    int       i;

    /* Short file name */
    strncpy(filename, "/drive0/a", sizeof(filename));

    /* Create max file name (OS_MAX_FILE_NAME including terminating null) */
    strncpy(maxfilename, "/drive0/", sizeof(maxfilename));
    for (i = strlen(maxfilename); i < (OS_MAX_FILE_NAME - 1); i++)
    {
        maxfilename[i] = 'm';
    }

    /* Create longer than max file name */
    strncpy(longfilename, "/drive0/", sizeof(longfilename));
    for (i = strlen(longfilename); i < (sizeof(longfilename) - 1); i++)
    {
        longfilename[i] = 'm';
    }

    /* create a file with short name */
    status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status == OS_SUCCESS, "fd after creat short name length file = %d", (int)status);

    /* close the first file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close short name length file = %d", (int)status);

    /* create a file with max name size */
    status = OS_OpenCreate(&fd, maxfilename, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status == OS_SUCCESS, "fd after creat max name length file = %d", (int)status);

    /* close the second file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close max name length file = %d", (int)status);

    /* remove the file from the drive */
    status = OS_remove(filename);
    UtAssert_True(status == OS_SUCCESS, "status after short name length file remove = %d", (int)status);

    /* remove the file from the drive */
    status = OS_remove(maxfilename);
    UtAssert_True(status == OS_SUCCESS, "status after remove max name length file = %d", (int)status);

    /* try creating with file name too big, should fail */
    status = OS_OpenCreate(&fd, longfilename, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status < OS_SUCCESS, "status after create file name too long = %d", (int)status);
    UtAssert_True(!OS_ObjectIdDefined(fd), "fd(%lu) not defined", OS_ObjectIdToInteger(fd));

    /* try removing with file name too big. Should Fail */
    status = OS_remove(longfilename);
    UtAssert_True(status < OS_SUCCESS, "status after remove file name too long = %d", (int)status);

    /* try removing file that no longer exists. Should Fail */
    status = OS_remove(filename);
    UtAssert_True(status < OS_SUCCESS, "status after remove file doesn't exist = %d", (int)status);

    /* Similar to previous but with a bad mount point (gives different error code) */
    status = OS_remove("/x");
    UtAssert_True(status == OS_FS_ERR_PATH_INVALID, "status after remove bad mount = %d", (int)status);
}

/*---------------------------------------------------------------------------------------
 *  Name: TestOpenClose
 * This function tests the basic functionality of OS_open and OS_close.
---------------------------------------------------------------------------------------*/
void TestOpenClose(void)
{
    char      filename[OS_MAX_PATH_LEN];
    int32     status;
    osal_id_t fd;

    strncpy(filename, "/drive0/Filename1", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = 0;

    /* create a file of reasonable length (but over 8 chars) */
    status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d", (int)status);

    /*
    ** try to close the file
    */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);

    /*  reopen the file */
    status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_NONE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d", (int)status);

    /*
    ** try to close the file again
    */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);

    /*
    ** try to close the file again. Should Fail
    */
    status = OS_close(fd);
    UtAssert_True(status != OS_SUCCESS, "status after close = %d", (int)status);

    /*try to close a file not on the system. Should Fail */
    status = OS_close(OS_OBJECT_ID_UNDEFINED);
    UtAssert_True(status != OS_SUCCESS, "status after close = %d", (int)status);

    /*  open a file that was never in the system */
    status = OS_OpenCreate(&fd, "/drive0/FileNotHere", OS_FILE_FLAG_NONE, OS_READ_ONLY);
    UtAssert_True(status < OS_SUCCESS, "status after open = %d", (int)status);
    UtAssert_True(!OS_ObjectIdDefined(fd), "fd(%lu) not defined", OS_ObjectIdToInteger(fd));

    /* try removing the file from the drive  to end the function */
    status = OS_remove(filename);
    UtAssert_True(status == OS_SUCCESS, "status after remove = %d", (int)status);
}

/*---------------------------------------------------------------------------------------
 * Name TestChmod
---------------------------------------------------------------------------------------*/
void TestChmod(void)
{
    char      filename[OS_MAX_PATH_LEN];
    int32     status;
    osal_id_t fd;

    /*Make a file to test on. Start in Read only mode */
    strncpy(filename, "/drive0/Filename1", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = 0;
    status                         = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_CREATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d", (int)status);
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);

    /*Testing Write Only */
    status = OS_chmod(filename, OS_WRITE_ONLY);
    if (status != OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_True(status == OS_SUCCESS, "status after chmod = %d", (int)status);
        status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_NONE, OS_WRITE_ONLY);
        UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d", (int)status);
        status = OS_close(fd);
        UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);
    }
    else
    {
        UtAssert_NA("OS_chmod not implemented for write only");
    }

    /*Testing Read Only */
    status = OS_chmod(filename, OS_READ_ONLY);
    if (status != OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_True(status == OS_SUCCESS, "status after chmod = %d", (int)status);
        status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_NONE, OS_READ_ONLY);
        UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d", (int)status);
        status = OS_close(fd);
        UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);
    }
    else
    {
        UtAssert_NA("OS_chmod not implemented for read only");
    }

    /*Testing Read Write */
    status = OS_chmod(filename, OS_READ_WRITE);
    if (status != OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_True(status == OS_SUCCESS, "status after chmod = %d", (int)status);
        status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_NONE, OS_READ_WRITE);
        UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d", (int)status);
        status = OS_close(fd);
        UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);
    }
    else
    {
        UtAssert_NA("OS_chmod not implemented for read write");
    }

    /*Removing the file */
    status = OS_remove(filename);
    UtAssert_True(status == OS_SUCCESS, "status after remove = %d", (int)status);
}

/*---------------------------------------------------------------------------------------
 * Name TestReadWriteLseek
---------------------------------------------------------------------------------------*/
void TestReadWriteLseek(void)
{
    char      filename[OS_MAX_PATH_LEN];
    char      buffer[30];
    char      copyofbuffer[30];
    char      seekbuffer[30];
    char      newbuffer[30];
    size_t    offset;
    size_t    size;
    int32     status;
    osal_id_t fd;

    strncpy(filename, "/drive0/Filename1", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = 0;

    strcpy(buffer, "ValueToWriteInTheFile");
    strcpy(copyofbuffer, buffer); /* hold a copy of the buffer */
    offset = 12;

    /* start copying offset chars in buffer into seekbuffer */
    strcpy(seekbuffer, &buffer[offset]);

    /* create a file of reasonable length (but over 8 chars) */

    /* Open In R/W mode */
    status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d", (int)status);

    /* Write the string including null character */
    size = strlen(buffer) + 1;

    /* test write portion of R/W mode */
    status = OS_write(fd, (void *)buffer, size);
    UtAssert_True(status == size, "status after write = %d size = %lu", (int)status, (unsigned long)size);

    strcpy(buffer, "");

    /* lseek back to the beginning of the file */
    status = OS_lseek(fd, 0, 0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek = %d", (int)status);

    /*Read what we wrote to the file */
    status = OS_read(fd, (void *)buffer, size);
    UtAssert_True(status == size, "status after read = %d size = %lu", (int)status, (unsigned long)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strcmp(buffer, copyofbuffer) == 0, "Read: %s, Written: %s", buffer, copyofbuffer);
    }

    /* close the file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);

    /*  open a file again, but only in READ mode */
    status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_NONE, OS_READ_ONLY);
    UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d", (int)status);

    /* test write in READ ONLY mode */
    status = OS_write(fd, (void *)buffer, size);
    UtAssert_True(status < OS_SUCCESS, "status after write = %d", (int)status);

    /* try to read in READ ONLY MODE */

    status = OS_read(fd, (void *)buffer, size);
    UtAssert_True(status == size, "status after read = %d size = %lu", (int)status, (unsigned long)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strcmp(buffer, copyofbuffer) == 0, "Read: %s, Written: %s", buffer, copyofbuffer);
    }

    /* seek to a spot in the middle of the file to test lseek */
    status = OS_lseek(fd, offset, 0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek = %d", (int)status);

    /* now try to read out only the last chars of the file */

    status = OS_read(fd, (void *)newbuffer, OSAL_SIZE_C(size - offset));
    UtAssert_True(status == (size - offset), "status after read = %d size = %lu", (int)status,
                  (unsigned long)(size - offset));
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strncmp(newbuffer, seekbuffer, size - offset) == 0, "Read: %s, Written: %s", newbuffer,
                      seekbuffer);
    }

    /* close the file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);

    /*  open a file again, but only in WRITE mode */
    status = OS_OpenCreate(&fd, filename, OS_FILE_FLAG_NONE, OS_WRITE_ONLY);
    UtAssert_True(status >= OS_SUCCESS, "status after reopen = %d", (int)status);

    /* test write in WRITE ONLY mode */
    status = OS_write(fd, (void *)buffer, size);
    UtAssert_True(status == size, "status after write = %d size = %lu", (int)status, (unsigned long)size);

    /* try to read in WRITE ONLY MODE */
    status = OS_read(fd, (void *)buffer, size);
    UtAssert_True(status < OS_SUCCESS, "status after read = %d", (int)status);

    /* seek to a spot in the middle of the file to test lseek */
    status = OS_lseek(fd, offset, 0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek = %d", (int)status);

    /* now try to read out only the last chars of the file */
    status = OS_read(fd, (void *)newbuffer, OSAL_SIZE_C(size - offset));
    UtAssert_True(status < OS_SUCCESS, "status after read = %d", (int)status);

    /* close the file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);

    /* try removing the file from the drive */
    status = OS_remove(filename);
    UtAssert_True(status == OS_SUCCESS, "status after remove = %d", (int)status);
}

/*---------------------------------------------------------------------------------------
 *  Name TestMkRmDir()
---------------------------------------------------------------------------------------*/
void TestMkRmDirFreeBytes(void)
{
    int32        status;
    char         filename1[OS_MAX_PATH_LEN];
    char         filename2[OS_MAX_PATH_LEN];
    char         dir1[OS_MAX_PATH_LEN];
    char         dir2[OS_MAX_PATH_LEN];
    char         buffer1[OS_MAX_PATH_LEN];
    char         buffer2[OS_MAX_PATH_LEN];
    char         copybuffer1[OS_MAX_PATH_LEN];
    char         copybuffer2[OS_MAX_PATH_LEN];
    osal_id_t    fd1;
    osal_id_t    fd2;
    size_t       size;
    OS_statvfs_t statbuf;

    /* make the directory names for testing, as well as the filenames and the buffers
     * to put in the files */
    strcpy(dir1, "/drive0/DIRECTORY_ONE");
    strcpy(dir2, "/drive0/directory_two");
    strcpy(filename1, "/drive0/DIRECTORY_ONE/MyFile1");
    strcpy(filename2, "/drive0/directory_two/MyFile2");
    strcpy(buffer1, "111111111111");
    strcpy(copybuffer1, buffer1);
    strcpy(buffer2, "222222222222");
    strcpy(copybuffer2, buffer2);

    /* NOTE: The blocks free call is not necessarily implemented on all filesystems.
     * So the response of OS_ERR_NOT_IMPLEMENTED is acceptable. */
    status = OS_FileSysStatVolume("/drive0", &statbuf);
    UtAssert_True(status == OS_ERR_NOT_IMPLEMENTED || status == OS_SUCCESS,
                  "Checking Free Blocks: status=%d blocks=%lu", (int)status, (unsigned long)statbuf.blocks_free);

    /* make the two directories */
    status = OS_mkdir(dir1, OS_READ_WRITE);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 1 = %d", (int)status);

    status = OS_mkdir(dir2, OS_READ_WRITE);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 2 = %d", (int)status);

    /* now create two files in the two directories (1 file per directory) */

    status = OS_OpenCreate(&fd1, filename1, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d", (int)status);

    status = OS_OpenCreate(&fd2, filename2, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 2 = %d", (int)status);

    /* write the proper buffers into each of the files */
    size   = strlen(buffer1);
    status = OS_write(fd1, buffer1, size);
    UtAssert_True(status == size, "status after write 1 = %d size = %lu", (int)status, (unsigned long)size);

    size   = strlen(buffer2);
    status = OS_write(fd2, buffer2, size);
    UtAssert_True(status == size, "status after write 2 = %d size = %lu", (int)status, (unsigned long)size);

    /* lseek back to the beginning of the file */
    status = OS_lseek(fd1, 0, 0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek 1 = %d", (int)status);

    status = OS_lseek(fd2, 0, 0);
    UtAssert_True(status >= OS_SUCCESS, "status after lseek 2 = %d", (int)status);

    memset(buffer1, 0, sizeof(buffer1));
    memset(buffer2, 0, sizeof(buffer2));
    status = OS_FileSysStatVolume("/drive0", &statbuf);
    UtAssert_True(status == OS_ERR_NOT_IMPLEMENTED || status == OS_SUCCESS,
                  "Checking Free Blocks: status=%d blocks=%lu", (int)status, (unsigned long)statbuf.blocks_free);

    /* read back out of the files what we wrote into them */
    size   = strlen(copybuffer1);
    status = OS_read(fd1, (void *)buffer1, size);
    UtAssert_True(status == size, "status after read 1 = %d size = %lu", (int)status, (unsigned long)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strncmp(buffer1, copybuffer1, size) == 0, "Read: %s, Written: %s", buffer1, copybuffer1);
    }

    size   = strlen(copybuffer2);
    status = OS_read(fd2, (void *)buffer2, size);
    UtAssert_True(status == size, "status after read 2 = %d size = %lu", (int)status, (unsigned long)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strncmp(buffer2, copybuffer2, size) == 0, "Read: %s, Written: %s", buffer1, copybuffer1);
    }

    /* close the files */
    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d", (int)status);

    status = OS_close(fd2);
    UtAssert_True(status == OS_SUCCESS, "status after close 2 = %d", (int)status);

    /* try removing the file from the drive */
    status = OS_remove(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove 1 = %d", (int)status);

    status = OS_remove(filename2);
    UtAssert_True(status == OS_SUCCESS, "status after remove 2 = %d", (int)status);

    status = OS_rmdir(dir1);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 1 = %d", (int)status);

    status = OS_rmdir(dir2);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 2 = %d", (int)status);

    status = OS_FileSysStatVolume("/drive0", &statbuf);
    UtAssert_True(status == OS_ERR_NOT_IMPLEMENTED || status == OS_SUCCESS,
                  "Checking Free Blocks: status=%d blocks=%lu", (int)status, (unsigned long)statbuf.blocks_free);
}

/*---------------------------------------------------------------------------------------
 * Name TestOpenReadCloseDir();
---------------------------------------------------------------------------------------*/
void TestOpenReadCloseDir(void)
{
    int32       status;
    char        filename1[OS_MAX_PATH_LEN];
    char        filename2[OS_MAX_PATH_LEN];
    char        dir0[OS_MAX_PATH_LEN];
    char        dir1[OS_MAX_PATH_LEN];
    char        dir2[OS_MAX_PATH_LEN];
    char        buffer1[OS_MAX_PATH_LEN];
    char        buffer2[OS_MAX_PATH_LEN];
    size_t      size;
    osal_id_t   fd1;
    osal_id_t   fd2;
    osal_id_t   dirh;
    os_dirent_t dirent;

    /* make the directory names for testing, as well as the filenames and the buffers
     * to put in the files */
    strcpy(dir0, "/drive0");
    strcpy(dir1, "/drive0/DIRECTORY_ONE");
    strcpy(dir2, "/drive0/directory_two");
    strcpy(filename1, "/drive0/DIRECTORY_ONE/MyFile1");
    strcpy(filename2, "/drive0/directory_two/MyFile2");
    strcpy(buffer1, "111111111111");
    strcpy(buffer2, "222222222222");

    /* make the two directories */

    status = OS_mkdir(dir1, OS_READ_WRITE);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 1 = %d", (int)status);

    status = OS_mkdir(dir2, OS_READ_WRITE);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 2 = %d", (int)status);

    /* now create two files in the two directories (1 file per directory) */

    status = OS_OpenCreate(&fd1, filename1, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d", (int)status);

    status = OS_OpenCreate(&fd2, filename2, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 2 = %d", (int)status);

    /* write the proper buffers into each of the files */
    size   = strlen(buffer1);
    status = OS_write(fd1, buffer1, size);
    UtAssert_True(status == size, "status after write 1 = %d size = %lu", (int)status, (unsigned long)size);

    size   = strlen(buffer2);
    status = OS_write(fd2, buffer2, size);
    UtAssert_True(status == size, "status after write 2 = %d size = %lu", (int)status, (unsigned long)size);

    /* close the files */

    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d", (int)status);

    status = OS_close(fd2);
    UtAssert_True(status == OS_SUCCESS, "status after close 2 = %d", (int)status);

    /* Now both of the directories are there and have files in them. Also both files
     have stuff written in them */

    /* try to open the base directory "/" */
    /* New version of test 1 - uses full abstraction API */

    status = OS_DirectoryOpen(&dirh, dir0);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryOpen Id=%lx Rc=%d", OS_ObjectIdToInteger(dirh), (int)status);

    /* try to read the two folders that are in the "/" */
    /* Have to make it a loop to see if we can find the directories in question */
    while (true)
    {
        status = OS_DirectoryRead(dirh, &dirent);
        UtAssert_True(status == OS_SUCCESS, "OS_DirectoryRead Rc=%d Name=%s", (int)status, OS_DIRENTRY_NAME(dirent));
        if (status != OS_SUCCESS)
        {
            break;
        }

        if (strcmp(OS_DIRENTRY_NAME(dirent), "DIRECTORY_ONE") == 0)
        {
            break;
        }
    }

    UtAssert_True(status == OS_SUCCESS, "DIRECTORY_ONE found");

    /* Advance to end of dir */
    while (status == OS_SUCCESS)
    {
        status = OS_DirectoryRead(dirh, &dirent);
    }

    status = OS_DirectoryClose(dirh);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryClose Rc=%d", (int)status);

    /* New version of test 2 - uses full abstraction API */

    status = OS_DirectoryOpen(&dirh, dir0);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryOpen Id=%lx Rc=%d", OS_ObjectIdToInteger(dirh), (int)status);

    while (true)
    {
        status = OS_DirectoryRead(dirh, &dirent);
        UtAssert_True(status == OS_SUCCESS, "OS_DirectoryRead Rc=%d Name=%s", (int)status, OS_DIRENTRY_NAME(dirent));
        if (status != OS_SUCCESS)
        {
            break;
        }

        if (strcmp(OS_DIRENTRY_NAME(dirent), "directory_two") == 0)
        {
            break;
        }
    }

    UtAssert_True(status == OS_SUCCESS, "directory_two found");

    status = OS_DirectoryClose(dirh);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryClose Rc=%d", (int)status);

    /* Now test the open/ read close for one of the sub directories */

    /* New version of test 4 - uses full abstraction API */
    status = OS_DirectoryOpen(&dirh, dir1);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryOpen Id=%lx Rc=%d", OS_ObjectIdToInteger(dirh), (int)status);

    /* try to read the next file within the first directory "MyFile1" */
    while (true)
    {
        status = OS_DirectoryRead(dirh, &dirent);
        UtAssert_True(status == OS_SUCCESS, "OS_DirectoryRead Rc=%d Name=%s", (int)status, OS_DIRENTRY_NAME(dirent));
        if (status != OS_SUCCESS)
        {
            break;
        }

        if (strcmp(OS_DIRENTRY_NAME(dirent), "MyFile1") == 0)
        {
            break;
        }
    }

    UtAssert_True(status == OS_SUCCESS, "MyFile1 found");

    /* Close the file */

    status = OS_DirectoryClose(dirh);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryClose Rc=%d", (int)status);

    /* New version of test 5 - uses full abstraction API */
    status = OS_DirectoryOpen(&dirh, dir2);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryOpen Id=%lx Rc=%d", OS_ObjectIdToInteger(dirh), (int)status);

    /* try to read the next file within the first directory "MyFile2" */
    while (true)
    {
        status = OS_DirectoryRead(dirh, &dirent);
        UtAssert_True(status == OS_SUCCESS, "OS_DirectoryRead Rc=%d Name=%s", (int)status, OS_DIRENTRY_NAME(dirent));
        if (status != OS_SUCCESS)
        {
            break;
        }

        if (strcmp(OS_DIRENTRY_NAME(dirent), "MyFile2") == 0)
        {
            break;
        }
    }

    UtAssert_True(status == OS_SUCCESS, "MyFile2 found");

    /*Close the file */
    status = OS_DirectoryClose(dirh);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryClose Rc=%d", (int)status);

    /* Test case for bug #181 - make sure that a directory used as a mount point
     * is able to be opened.  This should not require a trailing
     * slash (i.e. /test rather than /test/) */
    status = OS_DirectoryOpen(&dirh, "/test");
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryOpen /test Id=%lx Rc=%d", OS_ObjectIdToInteger(dirh), (int)status);

    /*Close the file */
    status = OS_DirectoryClose(dirh);
    UtAssert_True(status >= OS_SUCCESS, "OS_DirectoryClose /test Rc=%d", (int)status);

    /* remove the files */
    status = OS_remove(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove 1 = %d", (int)status);

    status = OS_remove(filename2);
    UtAssert_True(status == OS_SUCCESS, "status after remove 2 = %d", (int)status);

    /* remove the directories */
    status = OS_rmdir(dir1);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 1 = %d", (int)status);

    status = OS_rmdir(dir2);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 2 = %d", (int)status);
}

/*---------------------------------------------------------------------------------------
 * Name TestRename
---------------------------------------------------------------------------------------*/
void TestRename(void)
{
    int32 status;
    char  filename1[OS_MAX_PATH_LEN];
    char  dir0[OS_MAX_PATH_LEN];
    char  dir1[OS_MAX_PATH_LEN];

    char buffer1[OS_MAX_PATH_LEN];
    char copybuffer1[OS_MAX_PATH_LEN];

    char newdir1[OS_MAX_PATH_LEN];
    char midname1[OS_MAX_PATH_LEN];
    char newfilename1[OS_MAX_PATH_LEN];

    osal_id_t fd1;
    size_t    size;

    /* make the directory names for testing, as well as the filenames and the buffers
     * to put in the files */
    strcpy(dir0, "/drive1/");
    strcpy(dir1, "/drive1/DIRECTORY_ONE");
    strcpy(filename1, "/drive1/DIRECTORY_ONE/MyFile1");
    strcpy(buffer1, "111111111111");
    strcpy(copybuffer1, buffer1);

    strcpy(newdir1, "/drive1/NEW_DIR_ONE");
    strcpy(newfilename1, "/drive1/NEW_DIR_ONE/NEW_myfile1");

    strcpy(midname1, "/drive1/NEW_DIR_ONE/MyFile1");

    /* make the directory */

    status = OS_mkdir(dir1, OS_READ_WRITE);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 1 = %d", (int)status);

    /* now create a file in the directory */

    status = OS_OpenCreate(&fd1, filename1, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d", (int)status);

    /* write the proper buffers into  the file */

    size   = strlen(buffer1);
    status = OS_write(fd1, buffer1, size);
    UtAssert_True(status == size, "status after write 1 = %d size = %lu", (int)status, (unsigned long)size);

    /* close the file */

    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d", (int)status);

    /* now there is 1 subdirectory with 1 file in it */

    /* Rename the directory */

    status = OS_rename(dir1, newdir1);
    UtAssert_True(status == OS_SUCCESS, "status after rename 1 = %d", (int)status);

    status = OS_rename(midname1, newfilename1);
    UtAssert_True(status == OS_SUCCESS, "status after rename 2 = %d", (int)status);

    memset(buffer1, 0, sizeof(buffer1));

    /* try to read the new file out */

    status = OS_OpenCreate(&fd1, newfilename1, OS_FILE_FLAG_NONE, OS_READ_ONLY);
    UtAssert_True(status >= OS_SUCCESS, "status after open 1 = %d", (int)status);

    size   = strlen(copybuffer1);
    status = OS_read(fd1, buffer1, size);
    UtAssert_True(status == size, "status after read 1 = %d size = %lu", (int)status, (unsigned long)size);
    if (status >= OS_SUCCESS)
    {
        UtAssert_True(strncmp(buffer1, copybuffer1, size) == 0, "Read and Written Results are equal");
    }

    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d", (int)status);

    /* try removing the file from the drive */
    /* note - if this is not done, subsequent runs of this test will fail */
    status = OS_remove(newfilename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove 1 = %d", (int)status);

    status = OS_rmdir(newdir1);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 1 = %d", (int)status);
}

/*---------------------------------------------------------------------------------------
 *  Name TestStat()
---------------------------------------------------------------------------------------*/
void TestStat(void)
{
    int32 status;

    char       filename1[OS_MAX_PATH_LEN];
    char       dir1[OS_MAX_PATH_LEN];
    char       dir1slash[OS_MAX_PATH_LEN];
    char       buffer1[OS_MAX_PATH_LEN];
    os_fstat_t StatBuff;
    osal_id_t  fd1;
    size_t     size;

    strcpy(dir1, "/drive0/DirectoryName");
    strcpy(dir1slash, dir1);
    strcat(dir1slash, "/");
    strcpy(filename1, "/drive0/DirectoryName/MyFile1");
    strcpy(buffer1, "111111111111");

    /* make the directory */
    status = OS_mkdir(dir1, OS_READ_WRITE);
    UtAssert_True(status == OS_SUCCESS, "status after mkdir 1 = %d", (int)status);

    /* now create a file  */
    status = OS_OpenCreate(&fd1, filename1, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d", (int)status);

    /* Write some data into the file */

    size   = strlen(buffer1);
    status = OS_write(fd1, buffer1, size);
    UtAssert_True(status == size, "status after write 1 = %d size = %lu", (int)status, (unsigned long)size);

    status = OS_close(fd1);
    UtAssert_True(status == OS_SUCCESS, "status after close 1 = %d", (int)status);

    /*
    ** Make the stat calls
    */
    status = OS_stat(dir1slash, &StatBuff);
    UtAssert_True(status == OS_SUCCESS, "status after stat 1 = %d", (int)status);

    status = OS_stat(dir1, &StatBuff);
    UtAssert_True(status == OS_SUCCESS, "status after stat 2 = %d", (int)status);

    status = OS_stat(filename1, &StatBuff);
    UtAssert_True(status == OS_SUCCESS, "status after stat 3 = %d", (int)status);

    /* Clean up */
    status = OS_remove(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove 1 = %d", (int)status);

    status = OS_rmdir(dir1);
    UtAssert_True(status == OS_SUCCESS, "status after rmdir 1 = %d", (int)status);
}

/*---------------------------------------------------------------------------------------
 *  Name: TestOpenFileAPI
 *  This function tests the misc open File API:
 *    OS_FileOpenCheck(char *Filename);
 *    OS_CloseAllFiles(void);
 *    OS_CloseFileByName(char *Filename);
 *
---------------------------------------------------------------------------------------*/
void TestOpenFileAPI(void)
{
    char      filename1[OS_MAX_PATH_LEN];
    char      filename2[OS_MAX_PATH_LEN];
    char      filename3[OS_MAX_PATH_LEN];
    int       status;
    osal_id_t fd;

    strcpy(filename1, "/drive0/Filename1");
    strcpy(filename2, "/drive0/Filename2");
    strcpy(filename3, "/drive0/Filename3");

    /* Create/open a file */
    status = OS_OpenCreate(&fd, filename1, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 1 = %d", (int)status);

    /* Create/open a file */
    status = OS_OpenCreate(&fd, filename2, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 2 = %d", (int)status);

    /* Create/open a file */
    status = OS_OpenCreate(&fd, filename3, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat 3 = %d", (int)status);

    /*
    ** Try OS_FileOpenCheck
    */
    status = OS_FileOpenCheck(filename1);
    UtAssert_True(status >= OS_SUCCESS, "status after OS_FileOpenCheck 1 = %d", (int)status);

    /*
    ** Try OS_CloseFileByName
    */
    status = OS_CloseFileByName(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after OS_CloseFileByName 1 = %d", (int)status);

    /*
    ** Try OS_CloseAllFiles
    */
    status = OS_CloseAllFiles();
    UtAssert_True(status == OS_SUCCESS, "status after OS_CloseAllFiles = %d", (int)status);

    /*
    ** Try OS_CloseFileByName with a file that is already closed
    */
    status = OS_CloseFileByName(filename2);
    UtAssert_True(status < OS_SUCCESS, "status after OS_CloseFileByName 2 = %d", (int)status);

    /* Try removing the files from the drive to end the function */
    status = OS_remove(filename1);
    UtAssert_True(status == OS_SUCCESS, "status after remove filename1 = %d", (int)status);
    status = OS_remove(filename2);
    UtAssert_True(status == OS_SUCCESS, "status after remove filename2 = %d", (int)status);
    status = OS_remove(filename3);
    UtAssert_True(status == OS_SUCCESS, "status after remove filename3 = %d", (int)status);
}
