/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_IO_H
#define OS_IMPL_IO_H

#include "osconfig.h"
#include "common_types.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct
{
    int  fd;
    bool selectable;
} OS_impl_file_internal_record_t;

/*
 * The global file handle table.
 *
 * This is shared by all OSAL entities that perform low-level I/O.
 */
extern OS_impl_file_internal_record_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

#endif /* OS_IMPL_IO_H */
