/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for xbdBlkDev.h
 */

#ifndef OVERRIDE_XBDBLKDEV_H
#define OVERRIDE_XBDBLKDEV_H

#include "OCS_xbdBlkDev.h"
#include <vxWorks.h>
#include <blkIo.h>

/* ----------------------------------------- */
/* mappings for declarations in xbdBlkDev.h */
/* ----------------------------------------- */

#define NULLDEV             OCS_NULLDEV
#define device_t            OCS_device_t
#define xbdBlkDevCreateSync OCS_xbdBlkDevCreateSync
#define xbdBlkDevDelete     OCS_xbdBlkDevDelete

#endif /* OVERRIDE_XBDBLKDEV_H */
