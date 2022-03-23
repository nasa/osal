/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for loadLib.h
 */

#ifndef OCS_LOADLIB_H
#define OCS_LOADLIB_H

#include "OCS_basetypes.h"
#include "OCS_moduleLib.h"
#include "OCS_vxWorks.h"

/* ----------------------------------------- */
/* constants normally defined in loadLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in loadLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in loadLib.h */
/* ----------------------------------------- */

extern OCS_MODULE_ID OCS_loadModule(int fd, unsigned int symFlag);

#endif /* OCS_LOADLIB_H */
