/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for memPartLib.h
 */

#ifndef OVERRIDE_MEMPARTLIB_H
#define OVERRIDE_MEMPARTLIB_H

#include "OCS_memPartLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in memPartLib.h */
/* ----------------------------------------- */
#define PART_ID        OCS_PART_ID
#define MEM_PART_STATS OCS_MEM_PART_STATS

#define memPartShow    OCS_memPartShow
#define memPartInfoGet OCS_memPartInfoGet
#define memSysPartId   OCS_memSysPartId

#endif /* OVERRIDE_MEMPARTLIB_H */
