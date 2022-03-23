/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for moduleLib.h
 */

#ifndef OVERRIDE_MODULELIB_H
#define OVERRIDE_MODULELIB_H

#include "OCS_moduleLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in moduleLib.h */
/* ----------------------------------------- */

#define MODULE_ID   OCS_MODULE_ID
#define MODULE_INFO OCS_MODULE_INFO

#define moduleInfoGet OCS_moduleInfoGet

#endif /* OVERRIDE_MODULELIB_H */
