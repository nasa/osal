/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for unldLib.h
 */

#ifndef OVERRIDE_UNLDLIB_H
#define OVERRIDE_UNLDLIB_H

#include "OCS_unldLib.h"
#include <vxWorks.h>
#include <moduleLib.h>

/* ----------------------------------------- */
/* mappings for declarations in unldLib.h */
/* ----------------------------------------- */
#define unldByModuleId OCS_unldByModuleId

#endif /* OVERRIDE_UNLDLIB_H */
