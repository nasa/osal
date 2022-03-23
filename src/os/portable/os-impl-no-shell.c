/**
 * \file
 *
 * No shell implementation, returns OS_ERR_NOT_IMPLEMENTED for calls
 */

#include "os-shared-shell.h"

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
    return OS_ERR_NOT_IMPLEMENTED;
}
