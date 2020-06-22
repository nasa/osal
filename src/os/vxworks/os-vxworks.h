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
 * File   : os-vxworks.h
 *
 * Author : Joe Hickey
 *
 * Purpose: This file contains definitions that are shared across the VxWorks
 *          OSAL implementation.  This file is private to the VxWorks port and it
 *          may contain VxWorks-specific definitions.
 */

/****************************************************************************************
                                    COMMON INCLUDE FILES
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>


#include "common_types.h"
#include "osapi.h"
#include <os-impl.h>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
****************************************************************************************/

typedef struct
{
    int fd;
    bool selectable;
} OS_VxWorks_filehandle_entry_t;

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

/*
 * The global file handle table.
 *
 * This table is shared across multiple units (files, sockets, etc) and they will share
 * the same file handle table from the basic file I/O.
 */
extern OS_VxWorks_filehandle_entry_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];



/****************************************************************************************
                       VXWORKS IMPLEMENTATION FUNCTION PROTOTYPES
****************************************************************************************/

int32 OS_VxWorks_TaskAPI_Impl_Init(void);
int32 OS_VxWorks_QueueAPI_Impl_Init(void);
int32 OS_VxWorks_BinSemAPI_Impl_Init(void);
int32 OS_VxWorks_CountSemAPI_Impl_Init(void);
int32 OS_VxWorks_MutexAPI_Impl_Init(void);
int32 OS_VxWorks_TimeBaseAPI_Impl_Init(void);
int32 OS_VxWorks_ModuleAPI_Impl_Init(void);
int32 OS_VxWorks_StreamAPI_Impl_Init(void);
int32 OS_VxWorks_DirAPI_Impl_Init(void);


