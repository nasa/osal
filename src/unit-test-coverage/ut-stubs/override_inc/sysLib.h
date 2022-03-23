/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for sysLib.h
 */

#ifndef OVERRIDE_SYSLIB_H
#define OVERRIDE_SYSLIB_H

#include "OCS_sysLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in sysLib.h */
/* ----------------------------------------- */
#define sysClkRateGet OCS_sysClkRateGet

#endif /* OVERRIDE_SYSLIB_H */
