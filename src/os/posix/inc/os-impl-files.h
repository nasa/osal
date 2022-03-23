/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_FILES_H
#define OS_IMPL_FILES_H

#include "os-impl-io.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * These two constants (EUID and EGID) are local cache of the
 * euid and egid of the user running the OSAL application.  They
 * assist the "stat" implementation in determination of permissions.
 *
 * For an OS that does not have multiple users, these could be
 * defined as 0.  Otherwise they should be populated via the system
 * geteuid/getegid calls.
 */
extern uid_t OS_IMPL_SELF_EUID;
extern gid_t OS_IMPL_SELF_EGID;

extern const int OS_IMPL_REGULAR_FILE_FLAGS;

#endif /* OS_IMPL_FILES_H */
