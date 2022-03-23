/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-filesys
 */

#ifndef UT_ADAPTOR_FILESYS_H
#define UT_ADAPTOR_FILESYS_H

#include "common_types.h"
#include "OCS_xbdBlkDev.h"

extern void *const  UT_Ref_OS_impl_filesys_table;
extern size_t const UT_Ref_OS_impl_filesys_table_SIZE;

void UT_FileSysTest_SetupFileSysEntry(osal_index_t id, OCS_BLK_DEV *blkdev, OCS_device_t xbddev, uint32 MaxParts);

#endif /* UT_ADAPTOR_FILESYS_H */
