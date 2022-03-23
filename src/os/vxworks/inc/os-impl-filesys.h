/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_FILESYS_H
#define OS_IMPL_FILESYS_H

#include "osconfig.h"
#include "common_types.h"
#include <blkIo.h>
#include <xbdBlkDev.h>

typedef struct
{
    BLK_DEV *blkDev;
    device_t xbd;
    uint32   xbdMaxPartitions;
} OS_impl_filesys_internal_record_t;

extern OS_impl_filesys_internal_record_t OS_impl_filesys_table[OS_MAX_FILE_SYSTEMS];

#endif /* OS_IMPL_FILESYS_H */
