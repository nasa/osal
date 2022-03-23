/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for sys/ioctl.h
 */

#ifndef OCS_SYS_IOCTL_H
#define OCS_SYS_IOCTL_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in sys/ioctl.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/ioctl.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in sys/ioctl.h */
/* ----------------------------------------- */

extern int OCS_ioctl(int fd, unsigned long request, ...);

#endif /* OCS_SYS_IOCTL_H */
