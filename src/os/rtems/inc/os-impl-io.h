/**
 * \file
 *
 * \ingroup  rtems
 *
 */

#ifndef OS_IMPL_IO_H
#define OS_IMPL_IO_H

#include "osconfig.h"
#include <stdbool.h>
#include <unistd.h>

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

#endif /* OS_IMPL_IO_H */
