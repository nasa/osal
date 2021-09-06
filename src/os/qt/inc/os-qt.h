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
 * \file     os-qt.h
 * \ingroup  qt
 * \author   joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains definitions that are shared across the QT
 *          OSAL implementation.  This file is private to the QT port and it
 *          may contain QT-specific definitions.
 */

#ifndef INCLUDE_OS_QT_H_
#define INCLUDE_OS_QT_H_

#include <QtCore>
/****************************************************************************************
                                    COMMON INCLUDE FILES
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
// #include <pthread.h>
// #include <mqueue.h>
// #include <fcntl.h>
// #include <semaphore.h>
// #include <sys/types.h>
// #include <sys/signal.h>

/*
 * Use the global definitions from the shared layer
 */
#include <os-shared-globaldefs.h>

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
 ***************************************************************************************/

typedef void *(*PthreadFuncPtr_t)(void *arg);

typedef struct
{
    int PriorityMax;
    int PriorityMin;
} QT_PriorityLimits_t;

typedef struct
{
    bool                   EnableTaskPriorities;
    osal_blockcount_t      TruncateQueueDepth;
    uint32                 ClockAccuracyNsec;
    // QThread *              ThreadKey;
    // pthread_key_t          ThreadKey;
    sigset_t               MaximumSigMask;
    sigset_t               NormalSigMask;
    size_t                 PageSize;
    QT_PriorityLimits_t PriLimits;
    int                    SelectedRtScheduler;
} QT_GlobalVars_t;

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern QT_GlobalVars_t QT_GlobalVars;

/****************************************************************************************
                       QT IMPLEMENTATION FUNCTION PROTOTYPES
 ***************************************************************************************/

int32 OS_QT_TaskAPI_Impl_Init(void);
int32 OS_QT_QueueAPI_Impl_Init(void);
int32 OS_QT_BinSemAPI_Impl_Init(void);
int32 OS_QT_CountSemAPI_Impl_Init(void);
int32 OS_QT_MutexAPI_Impl_Init(void);
int32 OS_QT_ModuleAPI_Impl_Init(void);
int32 OS_QT_TimeBaseAPI_Impl_Init(void);
int32 OS_QT_StreamAPI_Impl_Init(void);
int32 OS_QT_DirAPI_Impl_Init(void);
int32 OS_QT_FileSysAPI_Impl_Init(void);

int32 OS_QT_TableMutex_Init(osal_objtype_t idtype);


#include <QThread>



class OSALThread : public QThread{
    public:
    void * data;
    PthreadFuncPtr_t entry;
    void run() override {
       entry(this->data);
    };
};

/*tasks */
typedef struct
{
    char name[OS_MAX_API_NAME];
    OSALThread * thread;
} OS_impl_task_internal_record_t;

int32 OS_QT_InternalTaskCreate_Impl(OS_impl_task_internal_record_t *ost, osal_priority_t priority, size_t stacksz,
                                       PthreadFuncPtr_t entry, void *entry_arg);

void  OS_QT_CompAbsDelayTime(uint32 msecs, struct timespec *tm);

#endif /* INCLUDE_OS_QT_H_ */
