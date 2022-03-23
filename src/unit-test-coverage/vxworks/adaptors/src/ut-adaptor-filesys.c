/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-filesys.h"

#include "os-vxworks.h"
#include "os-impl-filesys.h"

void *const  UT_Ref_OS_impl_filesys_table      = OS_impl_filesys_table;
size_t const UT_Ref_OS_impl_filesys_table_SIZE = sizeof(OS_impl_filesys_table);

void UT_FileSysTest_SetupFileSysEntry(osal_index_t id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts)
{
    OS_impl_filesys_table[id].blkDev           = blkdev;
    OS_impl_filesys_table[id].xbd              = xbddev;
    OS_impl_filesys_table[id].xbdMaxPartitions = MaxParts;
}
