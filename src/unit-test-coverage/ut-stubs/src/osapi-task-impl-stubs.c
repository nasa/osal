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
 * \file     osapi-task-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-task.h"
#include "osapi-idmap.h"

/*
** Task API
*/
UT_DEFAULT_STUB(OS_TaskMatch_Impl, (const OS_object_token_t *token))
UT_DEFAULT_STUB(OS_TaskDetach_Impl, (const OS_object_token_t *token))
UT_DEFAULT_STUB(OS_TaskCreate_Impl, (const OS_object_token_t *token, uint32 flags))
UT_DEFAULT_STUB(OS_TaskDelete_Impl, (const OS_object_token_t *token))
void OS_TaskExit_Impl(void)
{
    UT_DEFAULT_IMPL(OS_TaskExit_Impl);
}

UT_DEFAULT_STUB(OS_TaskDelay_Impl, (uint32 millisecond))
UT_DEFAULT_STUB(OS_TaskSetPriority_Impl, (const OS_object_token_t *token, osal_priority_t new_priority))
osal_id_t OS_TaskGetId_Impl(void)
{
    int32     status;
    osal_id_t id;

    status = UT_DEFAULT_IMPL_RC(OS_TaskGetId_Impl, 1);

    /* convert the int32 status value to an osal_id_t -
     * (this assumes the types are compatible) */
    id = OS_ObjectIdFromInteger(status);

    return id;
}
UT_DEFAULT_STUB(OS_TaskGetInfo_Impl, (const OS_object_token_t *token, OS_task_prop_t *task_prop))
UT_DEFAULT_STUB(OS_TaskRegister_Impl, (osal_id_t global_task_id))

bool OS_TaskIdMatchSystemData_Impl(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    return UT_DEFAULT_IMPL(OS_TaskIdMatchSystemData_Impl);
}

UT_DEFAULT_STUB(OS_TaskValidateSystemData_Impl, (const void *sysdata, size_t sysdata_size))
