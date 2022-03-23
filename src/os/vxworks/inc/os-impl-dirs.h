/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_DIRS_H
#define OS_IMPL_DIRS_H

#include "osconfig.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * In VxWorks 6.x the system mkdir() function only has a path argument
 * In VxWorks 7 it is now POSIX compilant and adds a mode argument
 *
 * This macro simply discards the second argument, allowing code to use
 * mkdir() in a consistent, POSIX compliant fashion.
 */
#ifdef OSAL_VXWORKS6_COMPATIBILITY
#define mkdir(path, mode) mkdir(path)
#endif

typedef struct
{
    DIR *dp;
} OS_impl_dir_internal_record_t;

/*
 * The directory handle table.
 */
extern OS_impl_dir_internal_record_t OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];

#endif /* OS_IMPL_DIRS_H */
