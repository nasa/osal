/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for drv/hdisk/ataDrv.h
 */

#ifndef OCS_DRV_HDISK_ATADRV_H
#define OCS_DRV_HDISK_ATADRV_H

#include "OCS_basetypes.h"
#include "OCS_vxWorks.h"
#include "OCS_blkIo.h"

/* ----------------------------------------- */
/* constants normally defined in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */
extern OCS_BLK_DEV *OCS_ataDevCreate(int ctrl, int drive, unsigned int nBlocks, unsigned int blkOffset);

#endif /* OCS_DRV_HDISK_ATADRV_H */
