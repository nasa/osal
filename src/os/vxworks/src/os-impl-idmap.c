/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-impl-idmap.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"
#include "os-shared-idmap.h"

#include <errnoLib.h>
#include <objLib.h>
#include <semLib.h>
#include <sysLib.h>

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

VX_MUTEX_SEMAPHORE(OS_task_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_queue_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_bin_sem_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_mutex_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_count_sem_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_stream_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_dir_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_timebase_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_module_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_filesys_table_mut_mem);
VX_MUTEX_SEMAPHORE(OS_console_mut_mem);

VxWorks_GlobalMutex_t VX_MUTEX_TABLE[] =
{
        [OS_OBJECT_TYPE_UNDEFINED] = { NULL },
        [OS_OBJECT_TYPE_OS_TASK] = { .mem = OS_task_table_mut_mem },
        [OS_OBJECT_TYPE_OS_QUEUE] = { .mem = OS_queue_table_mut_mem },
        [OS_OBJECT_TYPE_OS_COUNTSEM] = { .mem = OS_count_sem_table_mut_mem },
        [OS_OBJECT_TYPE_OS_BINSEM] = { .mem = OS_bin_sem_table_mut_mem },
        [OS_OBJECT_TYPE_OS_MUTEX] = { .mem = OS_mutex_table_mut_mem },
        [OS_OBJECT_TYPE_OS_STREAM] = { .mem = OS_stream_table_mut_mem },
        [OS_OBJECT_TYPE_OS_DIR] = { .mem = OS_dir_table_mut_mem },
        [OS_OBJECT_TYPE_OS_TIMEBASE] = { .mem = OS_timebase_table_mut_mem },
        [OS_OBJECT_TYPE_OS_MODULE] = { .mem = OS_module_table_mut_mem },
        [OS_OBJECT_TYPE_OS_FILESYS] = { .mem = OS_filesys_table_mut_mem },
        [OS_OBJECT_TYPE_OS_CONSOLE] = { .mem = OS_console_mut_mem },
};

enum
{
    VX_MUTEX_TABLE_SIZE = (sizeof(VX_MUTEX_TABLE) / sizeof(VX_MUTEX_TABLE[0]))
};

/*----------------------------------------------------------------
 *
 * Function: OS_Lock_Global_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_Lock_Global_Impl(uint32 idtype)
{
    VxWorks_GlobalMutex_t *mut;

    if (idtype >= VX_MUTEX_TABLE_SIZE)
    {
        return OS_ERROR;
    }

    mut = &VX_MUTEX_TABLE[idtype];
    if (mut->vxid == (SEM_ID)0)
    {
        return OS_ERROR;
    }

    if (semTake(mut->vxid, WAIT_FOREVER) != OK)
    {
        OS_DEBUG("semTake() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    return OS_SUCCESS;
} /* end OS_Lock_Global_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_Unlock_Global_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_Unlock_Global_Impl(uint32 idtype)
{
    VxWorks_GlobalMutex_t *mut;

    if (idtype >= VX_MUTEX_TABLE_SIZE)
    {
        return OS_ERROR;
    }

    mut = &VX_MUTEX_TABLE[idtype];
    if (mut->vxid == (SEM_ID)0)
    {
        return OS_ERROR;
    }

    if (semGive(mut->vxid) != OK)
    {
        OS_DEBUG("semGive() - vxWorks errno %d\n",errno);
        return OS_ERROR;
    }

    return OS_SUCCESS;
} /* end OS_Unlock_Global_Impl */



/****************************************************************************************
                                INITIALIZATION FUNCTION
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_TableMutex_Init
 *
 *  Purpose: Initialize the tables that the OS API uses to keep track of information
 *           about objects
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_TableMutex_Init(uint32 idtype)
{
    int32    return_code = OS_SUCCESS;
    SEM_ID   semid;

    /* Initialize the table mutex for the given idtype */
    if (idtype < VX_MUTEX_TABLE_SIZE && VX_MUTEX_TABLE[idtype].mem != NULL)
    {
        semid = semMInitialize (VX_MUTEX_TABLE[idtype].mem, SEM_Q_PRIORITY | SEM_INVERSION_SAFE);

        if ( semid == (SEM_ID)0 )
        {
            OS_DEBUG("Error: semMInitialize() failed - vxWorks errno %d\n",errno);
            return_code = OS_ERROR;
        }
        else
        {
            VX_MUTEX_TABLE[idtype].vxid = semid;
        }
    }

    return(return_code);
} /* end OS_VxWorks_TableMutex_Init */

