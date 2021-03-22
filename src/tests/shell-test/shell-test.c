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
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

#define OS_TEST_SHELL_FILENAME "/drive0/shell_test.txt"

/* *************************************** MAIN ************************************** */

void TestOutputToFile(void)
{
    char      cmd[33];
    char      filename[OS_MAX_PATH_LEN];
    char      buffer[21];
    char      copyofbuffer[21];
    size_t    size;
    int32     status;
    osal_id_t fd;

    OS_mkfs(NULL, "/ramdev0", "RAM", 512, 20);
    OS_mount("/ramdev0", "/drive0");

    strncpy(filename, "/drive0/Filename1", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = 0;

    strcpy(buffer, "ValueToEchoInTheFile");
    strcpy(copyofbuffer, buffer); /* hold a copy of the buffer */

    /* Open In R/W mode */
    status = OS_OpenCreate(&fd, OS_TEST_SHELL_FILENAME, OS_FILE_FLAG_CREATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d", (int)status);

    /* Write the string */
    size = strlen(buffer);

    snprintf(cmd, sizeof(cmd), "echo -n \"%s\"", buffer);

    status = OS_ShellOutputToFile(cmd, fd);
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("Shell Commands not implemented");
    }
    else
    {
        UtAssert_True(status >= OS_SUCCESS, "status after shell output to file = %d", (int)status);

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
    }

    /* close the file */
    status = OS_close(fd);
    UtAssert_True(status == OS_SUCCESS, "status after close = %d", (int)status);

    /* try removing the file from the drive */
    status = OS_remove(OS_TEST_SHELL_FILENAME);
    UtAssert_True(status == OS_SUCCESS, "status after remove = %d", (int)status);

} /* end TestOutputToFile */

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestOutputToFile, NULL, NULL, "ShellTest");
}
