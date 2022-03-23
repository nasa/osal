/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for intLib.h
 */

#ifndef OVERRIDE_INTLIB_H
#define OVERRIDE_INTLIB_H

#include "OCS_intLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in intLib.h */
/* ----------------------------------------- */

#define intConnect   OCS_intConnect
#define intDisable   OCS_intDisable
#define intEnable    OCS_intEnable
#define intLock      OCS_intLock
#define intUnlock    OCS_intUnlock
#define INUM_TO_IVEC OCS_INUM_TO_IVEC

#endif /* OVERRIDE_INTLIB_H */
