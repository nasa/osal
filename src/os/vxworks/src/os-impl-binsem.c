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
 * \file     os-impl-binsem.c
 * \ingroup  vxworks
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include "os-impl-binsem.h"
#include "os-shared-binsem.h"
#include "os-shared-timebase.h"
#include "os-shared-idmap.h"

/****************************************************************************************
                                     DEFINES
****************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
****************************************************************************************/

/* Tables where the OS object information is stored */
OS_impl_binsem_internal_record_t OS_impl_bin_sem_table[OS_MAX_BIN_SEMAPHORES];

/****************************************************************************************
                             BINARY SEMAPHORE API
****************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_VxWorks_BinSemAPI_Impl_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_VxWorks_BinSemAPI_Impl_Init(void)
{
    memset(OS_impl_bin_sem_table, 0, sizeof(OS_impl_bin_sem_table));
    return (OS_SUCCESS);
} /* end OS_VxWorks_BinSemAPI_Impl_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_BinSemCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options)
{
    SEM_ID                            tmp_sem_id;
    OS_impl_binsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);

    /* Initialize VxWorks Semaphore.
     * The memory for this sem is statically allocated. */
    tmp_sem_id = semBInitialize(impl->bmem, SEM_Q_PRIORITY, sem_initial_value);

    /* check if semBInitialize failed */
    if (tmp_sem_id == (SEM_ID)0)
    {
        OS_DEBUG("semBInitialize() - vxWorks errno %d\n", errno);
        return OS_SEM_FAILURE;
    }

    impl->vxid = tmp_sem_id;
    return OS_SUCCESS;

} /* end OS_BinSemCreate_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_BinSemDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_binsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);

    /*
     * As the memory for the sem is statically allocated, delete is a no-op.
     */
    impl->vxid = 0;
    return OS_SUCCESS;

} /* end OS_BinSemDelete_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_BinSemGive_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemGive_Impl(const OS_object_token_t *token)
{
    OS_impl_binsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);

    /* Use common routine */
    return OS_VxWorks_GenericSemGive(impl->vxid);
} /* end OS_BinSemGive_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_BinSemFlush_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemFlush_Impl(const OS_object_token_t *token)
{
    OS_impl_binsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);

    /* Flush VxWorks Semaphore */
    if (semFlush(impl->vxid) != OK)
    {
        OS_DEBUG("semFlush() - vxWorks errno %d\n", errno);
        return OS_SEM_FAILURE;
    }

    return OS_SUCCESS;
} /* end OS_BinSemFlush_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_BinSemTake_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemTake_Impl(const OS_object_token_t *token)
{
    OS_impl_binsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);

    /* Use common routine */
    return OS_VxWorks_GenericSemTake(impl->vxid, WAIT_FOREVER);

} /* end OS_BinSemTake_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_BinSemTimedWait_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs)
{
    int                               ticks;
    int32                             status;
    OS_impl_binsem_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_bin_sem_table, *token);

    status = OS_Milli2Ticks(msecs, &ticks);

    if (status == OS_SUCCESS)
    {
        status = OS_VxWorks_GenericSemTake(impl->vxid, ticks);
    }

    return status;
} /* end OS_BinSemTimedWait_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_BinSemGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_BinSemGetInfo_Impl(const OS_object_token_t *token, OS_bin_sem_prop_t *bin_prop)
{
    /* VxWorks has no API for obtaining the current value of a semaphore */
    return OS_SUCCESS;
} /* end OS_BinSemGetInfo_Impl */
