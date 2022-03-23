/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-common
 */

#ifndef UT_ADAPTOR_COMMON_H
#define UT_ADAPTOR_COMMON_H

#include "common_types.h"
#include "utstubs.h"
#include "OCS_semLib.h"

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not part of the implementation API.
 *
 *****************************************************/

void UT_CommonTest_SetImplTableMutex(uint32 idtype, OCS_SEM_ID vxid);

extern int32 OS_VxWorks_GenericSemTake(OCS_SEM_ID vxid, int sys_ticks);
extern int32 OS_VxWorks_GenericSemGive(OCS_SEM_ID vxid);

/*
 * This also needs to expose the keys for the stubs to
 * helper functions that the test case needs to configure.
 *
 * This is because the test case cannot directly include
 * the internal header file which provides this API.
 */
extern const UT_EntryKey_t UT_StubKey_OS_VxWorks_TableMutex_Init;

#endif /* UT_ADAPTOR_COMMON_H */
