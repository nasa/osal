/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-countsem.h"

#include "os-vxworks.h"
#include "os-impl-countsem.h"

void *const  UT_Ref_OS_impl_count_sem_table      = OS_impl_count_sem_table;
size_t const UT_Ref_OS_impl_count_sem_table_SIZE = sizeof(OS_impl_count_sem_table);

int32 UT_Call_OS_VxWorks_CountSemAPI_Impl_Init(void)
{
    return OS_VxWorks_CountSemAPI_Impl_Init();
}
