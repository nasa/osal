/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for drv/hdisk/ataDrv.h
 */

#ifndef OVERRIDE_DRV_HDISK_ATADRV_H
#define OVERRIDE_DRV_HDISK_ATADRV_H

#include "OCS_drv_hdisk_ataDrv.h"
#include <vxWorks.h>
#include <blkIo.h>

/* ----------------------------------------- */
/* mappings for declarations in drv/hdisk/ataDrv.h */
/* ----------------------------------------- */
#define ataDevCreate OCS_ataDevCreate

#endif /* OVERRIDE_DRV_HDISK_ATADRV_H */
