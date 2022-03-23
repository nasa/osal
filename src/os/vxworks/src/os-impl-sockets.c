/**
 * \file
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-vxworks.h"
#include "os-shared-idmap.h"
#include "os-impl-io.h"
#include "os-impl-sockets.h"

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_ModuleAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
void OS_VxWorks_SetSocketFlags_Impl(const OS_object_token_t *token)
{
    OS_impl_file_internal_record_t *impl;
    int                             os_flags;

    impl = OS_OBJECT_TABLE_GET(OS_impl_filehandle_table, *token);

    /* Use ioctl/FIONBIO on this platform, rather than standard fcntl() */
    os_flags = 1;
    if (ioctl(impl->fd, FIONBIO, &os_flags) == -1)
    {
        /* No recourse if ioctl fails - just report the error and move on. */
        OS_DEBUG("ioctl(FIONBIO): %s\n", strerror(errno));
    }

    impl->selectable = true;
}
