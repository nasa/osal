/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for ioLib.h
 */

#ifndef OCS_IOLIB_H
#define OCS_IOLIB_H

#include "OCS_basetypes.h"
#include "OCS_vxWorks.h"

/* ----------------------------------------- */
/* constants normally defined in ioLib.h */
/* ----------------------------------------- */

#define OCS_FIOCHKDSK  0x1E01
#define OCS_FIOUNMOUNT 0x1E02
#define OCS_FIONBIO    0x1E03

/* ----------------------------------------- */
/* types normally defined in ioLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in ioLib.h */
/* ----------------------------------------- */

extern int OCS_ioctl(int fd, unsigned long request, ...);

#endif /* OCS_IOLIB_H */
