/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_SHELL_H
#define OS_SHARED_SHELL_H

#include "osapi-shell.h"
#include "os-shared-globaldefs.h"

/****************************************************************************************
                 SHELL API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*----------------------------------------------------------------
   Function: OS_ShellOutputToFile_Impl

    Purpose: Takes a shell command in and writes the output of that command to the specified file

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ShellOutputToFile_Impl(const OS_object_token_t *token, const char *Cmd);

#endif /* OS_SHARED_SHELL_H */
