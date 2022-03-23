/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for ramDrv.h
 */

#ifndef OVERRIDE_RAMDRV_H
#define OVERRIDE_RAMDRV_H

#include "OCS_ramDrv.h"
#include <vxWorks.h>
#include <blkIo.h>

/* ----------------------------------------- */
/* mappings for declarations in ramDrv.h */
/* ----------------------------------------- */

#define ramDevCreate OCS_ramDevCreate

#endif /* OVERRIDE_RAMDRV_H */
