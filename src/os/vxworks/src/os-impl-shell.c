/**
 * \file
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-impl-io.h"
#include "os-shared-shell.h"
#include "os-shared-file.h"
#include "os-shared-task.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

#include <shellLib.h>
#include <taskLib.h>
#include <sysLib.h>

#define OS_REDIRECTSTRSIZE           15
#define OS_SHELL_TMP_FILE_EXT        ".out"
#define OS_SHELL_TMP_FILE_EXT_LEN    4
#define OS_SHELL_CMD_TASK_STACK_SIZE 16384
#define OS_SHELL_CMD_TASK_PRIORITY   250

/*----------------------------------------------------------------
 *
 * Function: OS_ShellOutputToFile_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ShellOutputToFile_Impl(const OS_object_token_t *token, const char *Cmd)
{
    int32                           ReturnCode = OS_ERROR;
    int32                           Result;
    osal_id_t                       fdCmd;
    OS_impl_file_internal_record_t *out_impl;
    OS_impl_file_internal_record_t *cmd_impl;
    OS_object_token_t               cmd_token;
    char                            localShellName[OS_MAX_API_NAME];

    snprintf(localShellName, sizeof(localShellName), "shll_%08lx", OS_ObjectIdToInteger(OS_TaskGetId()));

    /* Create a file to write the command to (or write over the old one) */
    Result =
        OS_OpenCreate(&fdCmd, OS_SHELL_CMD_INPUT_FILE_NAME, OS_FILE_FLAG_CREATE | OS_FILE_FLAG_TRUNCATE, OS_READ_WRITE);

    if (Result < OS_SUCCESS)
    {
        return Result;
    }

    if (OS_ObjectIdGetById(OS_LOCK_MODE_NONE, OS_OBJECT_TYPE_OS_STREAM, fdCmd, &cmd_token) == OS_SUCCESS)
    {
        out_impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);
        cmd_impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, cmd_token);

        /* copy the command to the file, and then seek back to the beginning of the file */
        OS_write(fdCmd, Cmd, OS_strnlen(Cmd, OS_MAX_CMD_LEN));
        OS_lseek(fdCmd, 0, OS_SEEK_SET);

        /* Create a shell task the will run the command in the file, push output to OS_fd */
        Result = shellGenericInit("INTERPRETER=Cmd", 0, localShellName, NULL, false, false, cmd_impl->fd, out_impl->fd,
                                  out_impl->fd);
    }

    if (Result == OK)
    {
        /* Wait for the command to terminate */
        do
        {
            taskDelay(sysClkRateGet());
        } while (taskNameToId(localShellName) != ((TASK_ID)ERROR));

        ReturnCode = OS_SUCCESS;
    }

    /* Close the file descriptor */
    OS_close(fdCmd);

    return ReturnCode;

} /* end OS_ShellOutputToFile_Impl */
