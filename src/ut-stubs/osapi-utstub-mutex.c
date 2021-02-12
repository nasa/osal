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
 * \file     osapi-utstub-mutex.c
 * \author   joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-mutex.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

UT_DEFAULT_STUB(OS_MutexAPI_Init, (void))

/*****************************************************************************/
/**
** \brief OS_MutSemCreate stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemCreate.  The user can adjust the response by setting the
**        values in the MutSemCreateRtn structure prior to this function being
**        called.  If the value MutSemCreateRtn.count is greater than zero
**        then the counter is decremented; if it then equals zero the return
**        value is set to the user-defined value MutSemCreateRtn.value.
**        Alternately, the user can cause the function to return a failure
**        result, OS_ERROR, by setting the value of UT_OS_Fail to
**        OS_MUTCREATE_FAIL prior to this function being called.  OS_SUCCESS
**        is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag, OS_ERROR, or OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemCreate(osal_id_t *sem_id, const char *sem_name, uint32 options)
{
    UT_Stub_RegisterContext(UT_KEY(OS_MutSemCreate), sem_id);
    UT_Stub_RegisterContext(UT_KEY(OS_MutSemCreate), sem_name);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_MutSemCreate), options);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemCreate);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_MUTEX);
    }
    else
    {
        *sem_id = UT_STUB_FAKE_OBJECT_ID;
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_MutSemDelete stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemDelete.  The user can adjust the response by setting
**        the values in the MutSemDelRtn structure prior to this function
**        being called.  If the value MutSemDelRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value MutSemDelRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemDelete(osal_id_t sem_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_MutSemDelete), sem_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemDelete);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_MUTEX, sem_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_MutSemGive stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemGive.  The user can adjust the response by setting
**        the values in the MutSemGiveRtn structure prior to this function
**        being called.  If the value MutSemGiveRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value MutSemGiveRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemGive(osal_id_t sem_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_MutSemGive), sem_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemGive);

    return status;
}

/*****************************************************************************/
/**
** \brief OS_MutSemTake stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemTake.  The user can adjust the response by setting
**        the values in the MutSemTakeRtn structure prior to this function
**        being called.  If the value MutSemTakeRtn.count is greater than
**        zero then the counter is decremented; if it then equals zero the
**        return value is set to the user-defined value MutSemTakeRtn.value.
**        OS_SUCCESS is returned otherwise.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns either a user-defined status flag or OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemTake(osal_id_t sem_id)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_MutSemTake), sem_id);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemTake);

    return status;
}

/*****************************************************************************
 *
 * Stub function for OS_MutSemGetIdByName()
 *
 *****************************************************************************/
int32 OS_MutSemGetIdByName(osal_id_t *sem_id, const char *sem_name)
{
    UT_Stub_RegisterContext(UT_KEY(OS_MutSemGetIdByName), sem_id);
    UT_Stub_RegisterContext(UT_KEY(OS_MutSemGetIdByName), sem_name);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemGetIdByName);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_MutSemGetIdByName), sem_id, sizeof(*sem_id)) < sizeof(*sem_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_MUTEX, sem_id);
    }

    return status;
}

/*****************************************************************************/
/**
** \brief OS_MutSemGetInfo stub function
**
** \par Description
**        This function is used to mimic the response of the OS API function
**        OS_MutSemGetInfo.  It sets the mutex semaphore structure variables
**        to fixed values and returns OS_SUCCESS.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns OS_SUCCESS.
**
******************************************************************************/
int32 OS_MutSemGetInfo(osal_id_t sem_id, OS_mut_sem_prop_t *mut_prop)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OS_MutSemGetInfo), sem_id);
    UT_Stub_RegisterContext(UT_KEY(OS_MutSemGetInfo), mut_prop);

    int32 status;

    status = UT_DEFAULT_IMPL(OS_MutSemGetInfo);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_MutSemGetInfo), mut_prop, sizeof(*mut_prop)) < sizeof(*mut_prop))
    {
        strncpy(mut_prop->name, "Name", sizeof(mut_prop->name) - 1);
        mut_prop->name[sizeof(mut_prop->name) - 1] = '\0';
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &mut_prop->creator);
    }

    return status;
}
