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
 * \file     os-shared-coverage-support.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

void OS_UT_SetupIterator(osal_objtype_t obj_type, osal_index_t first_idx, osal_index_t num_entries)
{
    OS_object_token_t tokenlist[num_entries];
    osal_index_t      idx = OSAL_INDEX_C(0);

    while (idx < num_entries)
    {
        tokenlist[idx].lock_mode = OS_LOCK_MODE_NONE;
        tokenlist[idx].obj_type  = obj_type;
        tokenlist[idx].obj_idx   = first_idx + idx;
        tokenlist[idx].obj_id    = OS_ObjectIdFromInteger((obj_type << OS_OBJECT_TYPE_SHIFT) | tokenlist[idx].obj_idx);
        ++idx;
    }

    UT_SetDataBuffer(UT_KEY(OS_ObjectIdIteratorGetNext), tokenlist, sizeof(OS_object_token_t) * num_entries, true);
}

void OS_UT_SetupTestTargetIndex(osal_objtype_t obj_type, osal_index_t test_idx)
{
    OS_object_token_t token;

    token.lock_mode = OS_LOCK_MODE_NONE;
    token.obj_type  = obj_type;
    token.obj_idx   = test_idx;
    token.obj_id    = OS_ObjectIdFromInteger((obj_type << OS_OBJECT_TYPE_SHIFT) | test_idx);

    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &token, sizeof(OS_object_token_t), true);
    UT_SetDeferredRetcode(UT_KEY(OS_ObjectIdGetById), 1, OS_SUCCESS);
}

void OS_UT_SetupBasicInfoTest(osal_objtype_t obj_type, osal_index_t test_idx, const char *name, osal_id_t creator)
{
    OS_common_record_t *rptr;

    switch (obj_type)
    {
        case OS_OBJECT_TYPE_OS_TASK:
            rptr = OS_global_task_table;
            break;
        case OS_OBJECT_TYPE_OS_QUEUE:
            rptr = OS_global_queue_table;
            break;
        case OS_OBJECT_TYPE_OS_BINSEM:
            rptr = OS_global_bin_sem_table;
            break;
        case OS_OBJECT_TYPE_OS_COUNTSEM:
            rptr = OS_global_count_sem_table;
            break;
        case OS_OBJECT_TYPE_OS_MUTEX:
            rptr = OS_global_mutex_table;
            break;
        case OS_OBJECT_TYPE_OS_CONSOLE:
            rptr = OS_global_console_table;
            break;
        case OS_OBJECT_TYPE_OS_MODULE:
            rptr = OS_global_module_table;
            break;
        case OS_OBJECT_TYPE_OS_FILESYS:
            rptr = OS_global_filesys_table;
            break;
        case OS_OBJECT_TYPE_OS_TIMEBASE:
            rptr = OS_global_timebase_table;
            break;
        case OS_OBJECT_TYPE_OS_TIMECB:
            rptr = OS_global_timecb_table;
            break;
        case OS_OBJECT_TYPE_OS_STREAM:
            rptr = OS_global_stream_table;
            break;
        case OS_OBJECT_TYPE_OS_DIR:
            rptr = OS_global_dir_table;
            break;
        default:
            rptr = NULL;
            break;
    }

    rptr += test_idx;
    memset(rptr, 0, sizeof(*rptr));
    rptr->creator    = UT_OBJID_OTHER;
    rptr->name_entry = "ABC";

    OS_UT_SetupTestTargetIndex(obj_type, test_idx);
}
