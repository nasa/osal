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

/*
 * File: osapi_adaptor.h
 *
 * Purpose:
 *   Provide access to the unit under test static data and functions
 *
 * History:
 *   07/10/2015  A. Asp, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSAPI_ADAPTOR_H_
#define _OSAPI_ADAPTOR_H_

#include "vxworks6-coverage-stubs.h"
#include "osconfig.h"


typedef struct
{
    int32    free;
    int32    id;
    char     name [OS_MAX_API_NAME];
    int32    creator;
    uint32   stack_size;
    uint32   priority;
    void    *delete_hook_pointer;

} OsApi_Adaptor_OS_task_record_t;

/* queues */
typedef struct
{
    int32    free;
    VCS_SEM_ID id;                       /* a pointer to the id */
    uint32   max_size;
    char     name [OS_MAX_API_NAME];
    int32    creator;
} OsApi_Adaptor_OS_queue_record_t;

/* Binary Semaphores */
typedef struct
{
    int32  free;
    VCS_SEM_ID id;                       /* a pointer to the id */
    char   name [OS_MAX_API_NAME];
    int32  creator;
} OsApi_Adaptor_OS_bin_sem_record_t;

/* Counting Semaphores */
typedef struct
{
    int32  free;
    VCS_SEM_ID id;                       /* a pointer to the id */
    char   name [OS_MAX_API_NAME];
    int32  creator;
} OsApi_Adaptor_OS_count_sem_record_t;


/* Mutexes */
typedef struct
{
    int32  free;
    VCS_SEM_ID id;
    char   name [OS_MAX_API_NAME];
    int32  creator;
} OsApi_Adaptor_OS_mut_sem_record_t;


void OsApi_Adaptor_Reset_Tables(void);

VCS_SEM_ID OsApi_Adaptor_Get_Os_Task_Table_Sem(void);
VCS_SEM_ID OsApi_Adaptor_Get_Os_Queue_Table_Sem(void);
VCS_SEM_ID OsApi_Adaptor_Get_Os_Bin_Sem_Table_Sem(void);
VCS_SEM_ID OsApi_Adaptor_Get_Os_Count_Sem_Table_Sem(void);
VCS_SEM_ID OsApi_Adaptor_Get_Os_Mut_Sem_Table_Sem(void);

void OsApi_Adaptor_Set_Os_Task_Table_Sem(VCS_SEM_ID newVal);
void OsApi_Adaptor_Set_Os_Queue_Table_Sem(VCS_SEM_ID newVal);
void OsApi_Adaptor_Set_Os_Bin_Sem_Table_Sem(VCS_SEM_ID newVal);
void OsApi_Adaptor_Set_Os_Count_Sem_Table_Sem(VCS_SEM_ID newVal);
void OsApi_Adaptor_Set_Os_Mut_Sem_Table_Sem(VCS_SEM_ID newVal);


OsApi_Adaptor_OS_task_record_t *OsApi_Adaptor_getTaskTableEntry(uint32 idx);
OsApi_Adaptor_OS_queue_record_t *OsApi_Adaptor_getQueueTableEntry(uint32 idx);
OsApi_Adaptor_OS_bin_sem_record_t *OsApi_Adaptor_getBinSemTableEntry(uint32 idx);
OsApi_Adaptor_OS_count_sem_record_t *OsApi_Adaptor_getCountSemTableEntry(uint32 idx);
OsApi_Adaptor_OS_mut_sem_record_t *OsApi_Adaptor_getMutSemTableEntry(uint32 idx);

void OsApi_Adaptor_setTaskTableEntry(uint32 idx, int32 free, int32 id, const char *name,
                                     int32 creator, uint32 stack_size, uint32 priority,
                                     void *delete_hook_pointer);

void OsApi_Adaptor_setQueueTableEntry(uint32 idx, int32 free, VCS_MSG_Q_ID id, uint32 size,
                                      const char *name, int32 creator);

void OsApi_Adaptor_setBinSemTableEntry(uint32 idx, int32 free, VCS_SEM_ID id,
                                       const char *name, int32 creator);

void OsApi_Adaptor_setCountSemTableEntry(uint32 idx, int32 free, VCS_SEM_ID id,
                                         const char *name, int32 creator);

void OsApi_Adaptor_setMutSemTableEntry(uint32 idx, int32 free, VCS_SEM_ID id,
                                       const char *name, int32 creator);

uint32 OsApi_Adaptor_getOsTaskKey(void);
void   OsApi_Adaptor_setOsTaskKey(uint32 key);
uint32 OsApi_Adaptor_getOsPrintfEnabled(void);
void   OsApi_Adaptor_setOsPrintfEnabled(uint32 val);

#endif
