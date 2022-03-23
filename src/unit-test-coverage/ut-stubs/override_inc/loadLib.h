/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for loadLib.h
 */

#ifndef OVERRIDE_LOADLIB_H
#define OVERRIDE_LOADLIB_H

#include "OCS_loadLib.h"
#include <vxWorks.h>
#include <moduleLib.h>

/* ----------------------------------------- */
/* mappings for declarations in loadLib.h */
/* ----------------------------------------- */

#define loadModule OCS_loadModule

#endif /* OVERRIDE_LOADLIB_H */
