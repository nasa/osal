/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for errno.h
 */

#ifndef OCS_ERRNO_H
#define OCS_ERRNO_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in errno.h */
/* ----------------------------------------- */
/* POSIX-specific  errnos */
#define OCS_EINTR     0x1801
#define OCS_EAGAIN    0x1802
#define OCS_EEXIST    0x180a
#define OCS_EINVAL    0x1803
#define OCS_EMSGSIZE  0x1804
#define OCS_ETIMEDOUT 0x1805
#define OCS_ESPIPE    0x1806
#define OCS_ENOTSUP   0x1807
#define OCS_ENOSYS    0x1808
#define OCS_EROFS     0x1809

/* ----------------------------------------- */
/* types normally defined in errno.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in errno.h */
/* ----------------------------------------- */

extern int OCS_errno;

#endif /* OCS_ERRNO_H */
