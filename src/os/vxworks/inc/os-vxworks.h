/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_VXWORKS_H
#define OS_VXWORKS_H

/****************************************************************************************
                                    COMMON INCLUDE FILES
****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <semLib.h>
#include <errnoLib.h>

#include "os-shared-globaldefs.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
****************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

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

int OS_VxWorks_TaskEntry(int arg);
int OS_VxWorks_ConsoleTask_Entry(int arg);

uint32 OS_VxWorks_SigWait(osal_id_t timebase_id);
int    OS_VxWorks_TimeBaseTask(int arg);
void   OS_VxWorks_RegisterTimer(osal_id_t obj_id);
void   OS_VxWorks_UsecToTimespec(uint32 usecs, struct timespec *time_spec);

int32 OS_VxWorks_GenericSemTake(SEM_ID vxid, int sys_ticks);
int32 OS_VxWorks_GenericSemGive(SEM_ID vxid);

int32 OS_VxWorks_TableMutex_Init(osal_objtype_t idtype);

#endif /* OS_VXWORKS_H */
