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

const char OS_TEST_SHELL_COMMAND[] = "echo";
const char OS_TEST_SHELL_STRING[]  = "ValueToEchoInTheFile";

/*
 * Extra chars to allow for quoting and whitespace.
 *
 * There needs to be spaces between arguments, and depending
 * on the shell config and its behavior it may echo extra whitespace
 * and/or other chars, and a newline itself may be multiple chars (CR+LF).
 *
 * This allows up to this many extra chars surrounding the test string.
 */
#define OS_TEST_SHELL_MAX_EXTRA_CHARS 8

/*
 * Sizes for the echo buffer and command buffer
 * (both include some extra chars)
 */
#define OS_TEST_SHELL_ECHO_BUFFER_SIZE (sizeof(OS_TEST_SHELL_STRING) + OS_TEST_SHELL_MAX_EXTRA_CHARS)
#define OS_TEST_SHELL_CMD_BUFFER_SIZE  (sizeof(OS_TEST_SHELL_COMMAND) + OS_TEST_SHELL_ECHO_BUFFER_SIZE)

/* *************************************** MAIN ************************************** */

void TestOutputToFile(void)
{
    char      cmd[OS_TEST_SHELL_CMD_BUFFER_SIZE + 1]; /* +1 char for term byte */
    char      filename[OS_MAX_PATH_LEN];
    char      buffer[OS_TEST_SHELL_ECHO_BUFFER_SIZE + 1]; /* +1 char for term byte */
    size_t    size;
    int32     filepos;
    int32     status;
    osal_id_t fd;

    OS_mkfs(NULL, "/ramdev0", "RAM", 512, 20);
    OS_mount("/ramdev0", "/drive0");

    strncpy(filename, "/drive0/Filename1", sizeof(filename) - 1);
    filename[sizeof(filename) - 1] = 0;

    /* Open In R/W mode */
    status = OS_OpenCreate(&fd, OS_TEST_SHELL_FILENAME, OS_FILE_FLAG_CREATE, OS_READ_WRITE);
    UtAssert_True(status >= OS_SUCCESS, "status after creat = %d", (int)status);

    /* assemble command */
    snprintf(cmd, sizeof(cmd), "%s \"%s\"", OS_TEST_SHELL_COMMAND, OS_TEST_SHELL_STRING);

    status = OS_ShellOutputToFile(cmd, fd);
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("Shell Commands not implemented");
    }
    else
    {
        UtAssert_True(status >= OS_SUCCESS, "status after shell output to file = %d", (int)status);

        /* output content should be at least this size */
        size = sizeof(OS_TEST_SHELL_STRING) - 1;

        /* Use lseek to determine size of the file */
        filepos = OS_lseek(fd, 0, OS_SEEK_END);
        UtAssert_True(filepos >= size, "size of output file=%d, echoed string len=%lu", (int)filepos,
                      (unsigned long)size);

        /*
         * Different shells vary in how the echoed output actually appears in the file.
         *
         * Depending on config it may echo the command itself in addition to the output,
         * and there may be whitespace/newlines that should be ignored.
         *
         * Either way the content should appear toward the _end_ of the file.
         */
        if (filepos < OS_TEST_SHELL_ECHO_BUFFER_SIZE)
        {
            filepos = 0;
        }
        else
        {
            filepos -= OS_TEST_SHELL_ECHO_BUFFER_SIZE;
        }

        status = OS_lseek(fd, filepos, OS_SEEK_SET);
        UtAssert_True(status == filepos, "lseek to position %d, result=%d", (int)filepos, (int)status);

        /* Read what we wrote to the file */
        /* By memset() first and reading one byte less than the buffer, the result is ensured to be terminated */
        memset(buffer, 0, sizeof(buffer));
        status = OS_read(fd, buffer, sizeof(buffer) - 1);
        UtAssert_True(status >= size, "status after read = %d, min valid size = %lu", (int)status, (unsigned long)size);

        /* The test should pass if the expected string is anywhere in the buffer */
        UtAssert_True(strstr(buffer, OS_TEST_SHELL_STRING) != NULL, "Read: \'%s\', Expected: \'%s\'", buffer,
                      OS_TEST_SHELL_STRING);
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

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestOutputToFile, NULL, NULL, "ShellTest");
}
