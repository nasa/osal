/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-idmap
 */

#ifndef UT_ADAPTOR_IDMAP_H
#define UT_ADAPTOR_IDMAP_H

#include "common_types.h"
#include "OCS_semLib.h"

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not part of the implementation API.
 *
 *****************************************************/
int32 UT_Call_OS_VxWorks_TableMutex_Init(osal_objtype_t idtype);
void  UT_IdMapTest_SetImplTableMutex(osal_objtype_t idtype, OCS_SEM_ID vxid);

#endif /* UT_ADAPTOR_IDMAP_H */
