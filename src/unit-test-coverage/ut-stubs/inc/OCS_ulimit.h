/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for ulimit.h
 */

#ifndef OCS_ULIMIT_H
#define OCS_ULIMIT_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in ulimit.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in ulimit.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in ulimit.h */
/* ----------------------------------------- */
long OCS_ulimit(int cmd, long newlimit);

#endif /* OCS_ULIMIT_H */
