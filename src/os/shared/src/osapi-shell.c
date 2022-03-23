/**
 * \file
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "os-shared-shell.h"
#include "os-shared-file.h"
#include "os-shared-idmap.h"

/*----------------------------------------------------------------
 *
 * Function: OS_ShellOutputToFile
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ShellOutputToFile(const char *Cmd, osal_id_t filedes)
{
    OS_object_token_t token;
    int32             return_code;

    /* Check Parameters */
    OS_CHECK_POINTER(Cmd);

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_STREAM, filedes, &token);
    if (return_code == OS_SUCCESS)
    {
        return_code = OS_ShellOutputToFile_Impl(&token, Cmd);
        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_ShellOutputToFile */
