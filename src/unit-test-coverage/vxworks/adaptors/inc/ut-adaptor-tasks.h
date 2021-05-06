/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-tasks
 */

#ifndef UT_ADAPTOR_TASKS_H
#define UT_ADAPTOR_TASKS_H

#include "common_types.h"
#include "OCS_taskLib.h"

extern void *const  UT_Ref_OS_impl_task_table;
extern size_t const UT_Ref_OS_impl_task_table_SIZE;

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

int32         UT_Call_OS_VxWorks_TaskAPI_Impl_Init(void);
void          UT_TaskTest_SetImplTaskId(osal_index_t local_id, OCS_TASK_ID TaskId);
int           UT_TaskTest_CallEntryPoint(osal_id_t arg);
OCS_WIND_TCB *UT_TaskTest_GetTaskTcb(osal_index_t local_id);

#endif /* UT_ADAPTOR_TASKS_H */
