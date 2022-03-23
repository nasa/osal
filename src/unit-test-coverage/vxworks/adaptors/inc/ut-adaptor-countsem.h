/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-countsem
 */

#ifndef UT_ADAPTOR_COUNTSEM_H
#define UT_ADAPTOR_COUNTSEM_H

#include "common_types.h"
#include "OCS_taskLib.h"
#include "OCS_semLib.h"

extern void *const  UT_Ref_OS_impl_count_sem_table;
extern size_t const UT_Ref_OS_impl_count_sem_table_SIZE;

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

int32 UT_Call_OS_VxWorks_CountSemAPI_Impl_Init(void);

#endif /* UT_ADAPTOR_COUNTSEM_H */
