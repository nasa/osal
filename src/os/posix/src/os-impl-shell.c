/**
 * \file
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "os-posix.h"
#include "os-impl-io.h"

#include "os-shared-file.h"
#include "os-shared-shell.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

/****************************************************************************************
                         IMPLEMENTATION-SPECIFIC ROUTINES
             These are specific to this particular operating system
 ****************************************************************************************/

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
    pid_t                           cpid;
    osal_index_t                    local_id;
    int                             wstat;
    const char *                    shell = getenv("SHELL");
    OS_impl_file_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    if (shell == NULL)
    {
        shell = "/bin/sh";
    }

    cpid = fork();
    if (cpid < 0)
    {
        OS_DEBUG("%s(): Error during fork(): %s\n", __func__, strerror(errno));
        return OS_ERROR;
    }

    if (cpid == 0)
    {
        /* child process */
        dup2(impl->fd, STDOUT_FILENO);
        dup2(impl->fd, STDERR_FILENO);

        /* close all _other_ filehandles */
        for (local_id = 0; local_id < OS_MAX_NUM_OPEN_FILES; ++local_id)
        {
            if (OS_ObjectIdIsValid(OS_global_stream_table[local_id].active_id))
            {
                close(OS_impl_filehandle_table[local_id].fd);
            }
        }

        execl(shell, "sh", "-c", Cmd, NULL); /* does not return if successful */
        exit(EXIT_FAILURE);
    }

    if (waitpid(cpid, &wstat, 0) != cpid)
    {
        OS_DEBUG("%s(): Error during waitpid(): %s\n", __func__, strerror(errno));
        return OS_ERROR;
    }

    if (!WIFEXITED(wstat) || WEXITSTATUS(wstat) != 0)
    {
        OS_DEBUG("%s(): Error from child process: %d\n", __func__, wstat);
        return OS_ERROR;
    }

    return OS_SUCCESS;
} /* end OS_ShellOutputToFile_Impl */
