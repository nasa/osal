/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for blkIo.h
 */

#ifndef OVERRIDE_BLKIO_H
#define OVERRIDE_BLKIO_H

#include "OCS_blkIo.h"
#include <vxWorks.h>

/* ---------------------------------------*/
/* mappings for declarations in blkIo.h */
/* ---------------------------------------*/
#define NULLDEV    OCS_NULLDEV
#define BLK_DEV    OCS_BLK_DEV
#define BLK_DEV_ID OCS_BLK_DEV_ID

#endif /* OVERRIDE_BLKIO_H */
