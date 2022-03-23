/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-loader.h"

#include "os-vxworks.h"
#include "os-impl-loader.h"

void *const  UT_Ref_OS_impl_module_table      = OS_impl_module_table;
size_t const UT_Ref_OS_impl_module_table_SIZE = sizeof(OS_impl_module_table);

int32 UT_Call_OS_VxWorks_ModuleAPI_Impl_Init(void)
{
    return OS_VxWorks_ModuleAPI_Impl_Init();
}
