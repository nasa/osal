/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-binsem
 */

#ifndef UT_ADAPTOR_BINSEM_H
#define UT_ADAPTOR_BINSEM_H

#include "common_types.h"
#include "utstubs.h"
#include "OCS_taskLib.h"
#include "OCS_semLib.h"

extern void *const  UT_Ref_OS_impl_bin_sem_table;
extern size_t const UT_Ref_OS_impl_bin_sem_table_SIZE;

/*
 * This also needs to expose the keys for the stubs to
 * helper functions that the test case needs to configure.
 *
 * This is because the test case cannot directly include
 * the internal header file which provides this API.
 */
extern const UT_EntryKey_t UT_StubKey_GenericSemTake;
extern const UT_EntryKey_t UT_StubKey_GenericSemGive;

extern int32 UT_Call_OS_VxWorks_BinSemAPI_Impl_Init(void);

#endif /* UT_ADAPTOR_BINSEM_H */
