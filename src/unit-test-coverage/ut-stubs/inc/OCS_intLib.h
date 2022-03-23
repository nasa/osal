/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for intLib.h
 */

#ifndef OCS_INTLIB_H
#define OCS_INTLIB_H

#include "OCS_basetypes.h"
#include "OCS_vxWorks.h"

/* ----------------------------------------- */
/* constants normally defined in intLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in intLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in intLib.h */
/* ----------------------------------------- */

extern OCS_STATUS       OCS_intConnect(OCS_VOIDFUNCPTR *vector, OCS_VOIDFUNCPTR routine, int parameter);
extern int              OCS_intDisable(int level);
extern int              OCS_intEnable(int level);
extern int              OCS_intLock(void);
extern int              OCS_intUnlock(int lockKey);
extern OCS_VOIDFUNCPTR *OCS_INUM_TO_IVEC(unsigned int ui);

#endif /* OCS_INTLIB_H */
