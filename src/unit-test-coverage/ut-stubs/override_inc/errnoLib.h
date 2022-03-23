/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for errnoLib.h
 */

#ifndef OVERRIDE_ERRNOLIB_H
#define OVERRIDE_ERRNOLIB_H

#include "OCS_errnoLib.h"
#include <vxWorks.h>

/* -----------------------------------------  */
/* mappings for declarations in errnoLib.h */
/* -----------------------------------------  */
#define errnoGet OCS_errnoGet

#endif /* OVERRIDE_ERRNOLIB_H */
