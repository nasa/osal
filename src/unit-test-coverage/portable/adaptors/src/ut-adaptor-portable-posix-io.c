/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-portable-posix-io.h"

#include "os-impl-io.h"

void UT_PortablePosixIOTest_ResetImpl(osal_index_t local_id)
{
    OS_impl_filehandle_table[local_id].fd         = -1;
    OS_impl_filehandle_table[local_id].selectable = false;
}

void UT_PortablePosixIOTest_Set_Selectable(osal_index_t local_id, bool is_selectable)
{
    OS_impl_filehandle_table[local_id].selectable = is_selectable;
}

void UT_PortablePosixIOTest_Set_FD(osal_index_t local_id, int fd)
{
    OS_impl_filehandle_table[local_id].fd = fd;
}

bool UT_PortablePosixIOTest_Get_Selectable(osal_index_t local_id)
{
    return OS_impl_filehandle_table[local_id].selectable;
}
