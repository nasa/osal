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
#include "os-impl-files.h"
#include "os-shared-file.h"

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
OS_impl_file_internal_record_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_StreamAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_StreamAPI_Impl_Init(void)
{
    osal_index_t local_id;

    /*
     * init all filehandles to -1, which is always invalid.
     * this isn't strictly necessary but helps when debugging.
     */
    for (local_id = 0; local_id < OS_MAX_NUM_OPEN_FILES; ++local_id)
    {
        OS_impl_filehandle_table[local_id].fd         = -1;
        OS_impl_filehandle_table[local_id].selectable = false;
    }

    return OS_SUCCESS;
} /* end OS_VxWorks_StreamAPI_Impl_Init */
