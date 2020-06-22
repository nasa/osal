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
 * \file   os-rtems.h
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains definitions that are shared across the RTEMS
 *          OSAL implementation.  This file is private to the RTEMS port and it
 *          may contain RTEMS-specific definitions.
 *
 */

/****************************************************************************************
                                    COMMON INCLUDE FILES
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <rtems.h>
#include <rtems/malloc.h>
#include <rtems/rtems/intr.h>

#include "common_types.h"
#include "osapi.h"
#include "os-impl.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
 ***************************************************************************************/

typedef struct
{
   uint32        ClockAccuracyNsec;
   rtems_id      IdleTaskId;
} RTEMS_GlobalVars_t;

typedef struct
{
    int fd;
    bool selectable;
} OS_Rtems_filehandle_entry_t;


/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

extern RTEMS_GlobalVars_t RTEMS_GlobalVars;

/*
 * The global file handle table.
 *
 * This table is shared across multiple units (files, sockets, etc) and they will share
 * the same file handle table from the basic file I/O.
 */
extern OS_Rtems_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];


/****************************************************************************************
                       RTEMS IMPLEMENTATION FUNCTION PROTOTYPES
 ***************************************************************************************/

int32 OS_Rtems_TaskAPI_Impl_Init(void);
int32 OS_Rtems_QueueAPI_Impl_Init(void);
int32 OS_Rtems_BinSemAPI_Impl_Init(void);
int32 OS_Rtems_CountSemAPI_Impl_Init(void);
int32 OS_Rtems_MutexAPI_Impl_Init(void);
int32 OS_Rtems_TimeBaseAPI_Impl_Init(void);
int32 OS_Rtems_ModuleAPI_Impl_Init(void);
int32 OS_Rtems_StreamAPI_Impl_Init(void);
int32 OS_Rtems_DirAPI_Impl_Init(void);
int32 OS_Rtems_FileSysAPI_Impl_Init(void);



