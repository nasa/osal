/**
 * \file
 *
 * \ingroup  rtems
 *
 */

#ifndef OS_IMPL_DIRS_H
#define OS_IMPL_DIRS_H

#include "osconfig.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct
{
    DIR *dp;
} OS_impl_dir_internal_record_t;

/*
 * The directory handle table.
 */
extern OS_impl_dir_internal_record_t OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];

#endif /* OS_IMPL_DIRS_H */
