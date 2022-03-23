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
#include "os-impl-dirs.h"
#include "os-shared-dir.h"

/*
 * The directory handle table.
 */
OS_impl_dir_internal_record_t OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_DirAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_DirAPI_Impl_Init(void)
{
    memset(OS_impl_dir_table, 0, sizeof(OS_impl_dir_table));
    return OS_SUCCESS;
} /* end OS_VxWorks_DirAPI_Impl_Init */
