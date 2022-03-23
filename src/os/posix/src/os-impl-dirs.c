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
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "os-posix.h"
#include "os-impl-dirs.h"
#include "os-shared-dir.h"

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

/*
 * The directory handle table.
 */
OS_impl_dir_internal_record_t OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];

/****************************************************************************************
                         IMPLEMENTATION-SPECIFIC ROUTINES
             These are specific to this particular operating system
 ****************************************************************************************/

/* --------------------------------------------------------------------------------------
    Name: OS_Posix_DirAPI_Impl_Init

    Purpose: Directory table initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Posix_DirAPI_Impl_Init(void)
{
    memset(OS_impl_dir_table, 0, sizeof(OS_impl_dir_table));
    return OS_SUCCESS;
} /* end OS_Posix_DirAPI_Impl_Init */
