/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-queues.h"

#include "os-vxworks.h"
#include "os-impl-queues.h"

void *const  UT_Ref_OS_impl_queue_table      = OS_impl_queue_table;
size_t const UT_Ref_OS_impl_queue_table_SIZE = sizeof(OS_impl_queue_table);

int32 UT_Call_OS_VxWorks_QueueAPI_Impl_Init(void)
{
    return OS_VxWorks_QueueAPI_Impl_Init();
}
