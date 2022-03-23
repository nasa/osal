/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_FILES_H
#define OS_IMPL_FILES_H

#include "os-impl-io.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <unistd.h>

/*
 * VxWorks does not have UID/GID so these are defined as 0.
 */
#define OS_IMPL_SELF_EUID 0
#define OS_IMPL_SELF_EGID 0

/*
 * Do not set any additional flags for regular files
 */
#define OS_IMPL_REGULAR_FILE_FLAGS 0

#endif /* OS_IMPL_FILES_H */
