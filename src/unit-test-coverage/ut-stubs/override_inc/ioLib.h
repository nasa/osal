/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for ioLib.h
 */

#ifndef OVERRIDE_IOLIB_H
#define OVERRIDE_IOLIB_H

#include "OCS_ioLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in ioLib.h */
/* ----------------------------------------- */

#define FIOCHKDSK  OCS_FIOCHKDSK
#define FIOUNMOUNT OCS_FIOUNMOUNT
#define FIONBIO    OCS_FIONBIO
#define ioctl      OCS_ioctl

#endif /* OVERRIDE_IOLIB_H */
