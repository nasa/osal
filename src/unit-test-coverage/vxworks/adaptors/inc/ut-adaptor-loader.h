/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-loader
 */

#ifndef UT_ADAPTOR_LOADER_H
#define UT_ADAPTOR_LOADER_H

#include "common_types.h"
#include "utstubs.h"
#include "OCS_taskLib.h"
#include "OCS_semLib.h"

extern void *const  UT_Ref_OS_impl_module_table;
extern size_t const UT_Ref_OS_impl_module_table_SIZE;

extern int32 UT_Call_OS_VxWorks_ModuleAPI_Impl_Init(void);

#endif /* UT_ADAPTOR_LOADER_H */
