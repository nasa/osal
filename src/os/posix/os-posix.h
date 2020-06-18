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
 * \file   os-posix.h
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains definitions that are shared across the POSIX
 *          OSAL implementation.  This file is private to the POSIX port and it
 *          may contain POSIX-specific definitions.
 *
 */

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
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/signal.h>

#include "common_types.h"
#include "osapi.h"
#include "os-impl.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
 ***************************************************************************************/

typedef void * (*PthreadFuncPtr_t)(void *arg);

typedef struct
{
    int           PriorityMax;
    int           PriorityMin;
} POSIX_PriorityLimits_t;

typedef struct
{
   bool          EnableTaskPriorities;
   uint32        TruncateQueueDepth;
   uint32        ClockAccuracyNsec;
   pthread_key_t ThreadKey;
   sigset_t      MaximumSigMask;
   sigset_t      NormalSigMask;
   POSIX_PriorityLimits_t PriLimits;
   int           SelectedRtScheduler;
} POSIX_GlobalVars_t;

typedef struct
{
    int fd;
    bool selectable;
} OS_Posix_filehandle_entry_t;


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern POSIX_GlobalVars_t POSIX_GlobalVars;

/*
 * The global file handle table.
 *
 * This table is shared across multiple units (files, sockets, etc) and they will share
 * the same file handle table from the basic file I/O.
 */
extern OS_Posix_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];



/****************************************************************************************
                       POSIX IMPLEMENTATION FUNCTION PROTOTYPES
 ***************************************************************************************/

int32 OS_Posix_TaskAPI_Impl_Init(void);
int32 OS_Posix_QueueAPI_Impl_Init(void);
int32 OS_Posix_BinSemAPI_Impl_Init(void);
int32 OS_Posix_CountSemAPI_Impl_Init(void);
int32 OS_Posix_MutexAPI_Impl_Init(void);
int32 OS_Posix_ModuleAPI_Impl_Init(void);
int32 OS_Posix_TimeBaseAPI_Impl_Init(void);
int32 OS_Posix_StreamAPI_Impl_Init(void);
int32 OS_Posix_DirAPI_Impl_Init(void);
int32 OS_Posix_FileSysAPI_Impl_Init(void);

int32 OS_Posix_InternalTaskCreate_Impl (pthread_t *thr, uint32 priority, size_t stacksz, PthreadFuncPtr_t Entry, void *entry_arg);



