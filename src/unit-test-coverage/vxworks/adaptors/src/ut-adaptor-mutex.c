/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-mutex.h"

#include "os-vxworks.h"
#include "os-impl-mutex.h"

void *const  UT_Ref_OS_impl_mutex_table      = OS_impl_mutex_table;
size_t const UT_Ref_OS_impl_mutex_table_SIZE = sizeof(OS_impl_mutex_table);

int32 UT_Call_OS_VxWorks_MutexAPI_Impl_Init(void)
{
    return OS_VxWorks_MutexAPI_Impl_Init();
}
