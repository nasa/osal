/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for hostLib.h
 */

#ifndef OVERRIDE_HOSTLIB_H
#define OVERRIDE_HOSTLIB_H

#include "OCS_hostLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in hostLib.h */
/* ----------------------------------------- */
#define hostGetByName OCS_hostGetByName

#endif /* OVERRIDE_HOSTLIB_H */
