/**
 * \file
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-impl-files.h"
#include "os-shared-file.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
/* The file/stream table is referenced by multiple entities, i.e. sockets, select, etc */
OS_impl_file_internal_record_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

/****************************************************************************************
                         IMPLEMENTATION-SPECIFIC ROUTINES
             These are specific to this particular operating system
 ****************************************************************************************/

/* --------------------------------------------------------------------------------------
    Name: OS_Rtems_StreamAPI_Impl_Init

    Purpose: File/Stream subsystem global initialization

    Returns: OS_SUCCESS if success
 ---------------------------------------------------------------------------------------*/
int32 OS_Rtems_StreamAPI_Impl_Init(void)
{
    uint32 local_id;

    /*
     * init all filehandles to -1, which is always invalid.
     * this isn't strictly necessary but helps when debugging.
     */
    for (local_id = 0; local_id < OS_MAX_NUM_OPEN_FILES; ++local_id)
    {
        OS_impl_filehandle_table[local_id].fd = -1;
    }

    return OS_SUCCESS;
} /* end OS_Rtems_StreamAPI_Impl_Init */
