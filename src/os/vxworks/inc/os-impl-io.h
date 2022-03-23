/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_IO_H
#define OS_IMPL_IO_H

#include "osconfig.h"
#include "common_types.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

typedef struct
{
    int  fd;
    bool selectable;
} OS_impl_file_internal_record_t;

/*
 * The global file handle table.
 *
 * This table is shared across multiple units (files, sockets, etc) and they will share
 * the same file handle table from the basic file I/O.
 */
extern OS_impl_file_internal_record_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

/*
 * VxWorks needs to cast the argument to "write()" to avoid a warning.
 * This can be turned off in a future version if the vendor fixes the
 * prototype to be standards-compliant
 */
#define GENERIC_IO_CONST_DATA_CAST (void *)

#endif /* OS_IMPL_IO_H */
