/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-tasks.h"

#include "os-vxworks.h"
#include "os-impl-tasks.h"
#include "osapi-idmap.h"

void *const  UT_Ref_OS_impl_task_table      = OS_impl_task_table;
size_t const UT_Ref_OS_impl_task_table_SIZE = sizeof(OS_impl_task_table);

int32 UT_Call_OS_VxWorks_TaskAPI_Impl_Init(void)
{
    return OS_VxWorks_TaskAPI_Impl_Init();
}

void UT_TaskTest_SetImplTaskId(osal_index_t local_id, OCS_TASK_ID TaskId)
{
    OS_impl_task_table[local_id].vxid = TaskId;
}

/*
 * Because the task entry point is declared "static",
 * in order for the UT to invoke it there must be a non-static
 * way to get access to it.
 */
int UT_TaskTest_CallEntryPoint(osal_id_t arg)
{
    return OS_VxWorks_TaskEntry(OS_ObjectIdToInteger(arg));
}

OCS_WIND_TCB *UT_TaskTest_GetTaskTcb(osal_index_t local_id)
{
    return &OS_impl_task_table[local_id].tcb;
}
