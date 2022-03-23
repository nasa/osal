/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for errno.h
 */

#ifndef OVERRIDE_ERRNO_H
#define OVERRIDE_ERRNO_H

#include "OCS_errno.h"

/* ----------------------------------------- */
/* mappings for declarations in errno.h */
/* ----------------------------------------- */
#define EINTR     OCS_EINTR
#define EAGAIN    OCS_EAGAIN
#define EINVAL    OCS_EINVAL
#define EEXIST    OCS_EEXIST
#define EMSGSIZE  OCS_EMSGSIZE
#define ETIMEDOUT OCS_ETIMEDOUT
#define ESPIPE    OCS_ESPIPE
#define ENOTSUP   OCS_ENOTSUP
#define ENOSYS    OCS_ENOSYS
#define EROFS     OCS_EROFS

#define errno OCS_errno

#endif /* OVERRIDE_ERRNO_H */
