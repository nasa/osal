/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-mutex
 */

#ifndef UT_ADAPTOR_MUTEX_H
#define UT_ADAPTOR_MUTEX_H

#include "common_types.h"
#include "OCS_taskLib.h"
#include "OCS_semLib.h"

extern void *const  UT_Ref_OS_impl_mutex_table;
extern size_t const UT_Ref_OS_impl_mutex_table_SIZE;

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

int32 UT_Call_OS_VxWorks_MutexAPI_Impl_Init(void);

#endif /* UT_ADAPTOR_MUTEX_H */
