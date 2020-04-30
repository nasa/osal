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
 * \file     coveragetest-countsem.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-countsem.h"

#include <OCS_string.h>

/*
**********************************************************************************
**          PUBLIC API FUNCTIONS
**********************************************************************************
*/

void Test_OS_CountSemAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_CountSemAPI_Init();

    UtAssert_True(actual == expected, "OS_CountSemAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_CountSemCreate(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemCreate (uint32 *sem_id, const char *sem_name,
     *          uint32 sem_initial_value, uint32 options)
     */
    int32 expected = OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;
    int32 actual = OS_CountSemCreate(&objid, "UT", 0,0);

    UtAssert_True(actual == expected, "OS_CountSemCreate() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "objid (%lu) != 0", (unsigned long)objid);

    OSAPI_TEST_FUNCTION_RC(OS_CountSemCreate(NULL, NULL, 0, 0), OS_INVALID_POINTER);
    UT_SetForceFail(UT_KEY(OCS_strlen), 10 + OS_MAX_API_NAME);
    OSAPI_TEST_FUNCTION_RC(OS_CountSemCreate(&objid, "UT", 0, 0), OS_ERR_NAME_TOO_LONG);
}

void Test_OS_CountSemDelete(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemDelete (uint32 sem_id)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_CountSemDelete(1);

    UtAssert_True(actual == expected, "OS_CountSemDelete() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_CountSemGive(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGive ( uint32 sem_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_CountSemGive(1);

    UtAssert_True(actual == expected, "OS_CountSemGive() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_CountSemTake(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTake ( uint32 sem_id )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_CountSemTake(1);

    UtAssert_True(actual == expected, "OS_CountSemTake() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_CountSemTimedWait(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemTimedWait ( uint32 sem_id, uint32 msecs )
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_CountSemTimedWait(1,1);

    UtAssert_True(actual == expected, "OS_CountSemTimedWait() (%ld) == OS_SUCCESS", (long)actual);
}


void Test_OS_CountSemGetIdByName(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGetIdByName (uint32 *sem_id, const char *sem_name)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    uint32 objid = 0xFFFFFFFF;

    UT_SetForceFail(UT_KEY(OS_ObjectIdFindByName), OS_SUCCESS);
    actual = OS_CountSemGetIdByName(&objid, "UT");
    UtAssert_True(actual == expected, "OS_CountSemGetIdByName() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(objid != 0, "OS_CountSemGetIdByName() objid (%lu) != 0", (unsigned long)objid);
    UT_ClearForceFail(UT_KEY(OS_ObjectIdFindByName));


    expected = OS_ERR_NAME_NOT_FOUND;
    actual = OS_CountSemGetIdByName(&objid, "NF");
    UtAssert_True(actual == expected, "OS_CountSemGetIdByName() (%ld) == %ld",
            (long)actual, (long)expected);

    OSAPI_TEST_FUNCTION_RC(OS_CountSemGetIdByName(NULL, NULL), OS_INVALID_POINTER);

}

void Test_OS_CountSemGetInfo(void)
{
    /*
     * Test Case For:
     * int32 OS_CountSemGetInfo (uint32 sem_id, OS_count_sem_prop_t *count_prop)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;
    OS_count_sem_prop_t prop;
    uint32 local_index = 1;
    OS_common_record_t utrec;
    OS_common_record_t *rptr = &utrec;

    memset(&utrec, 0, sizeof(utrec));
    utrec.creator = 111;
    utrec.name_entry = "ABC";
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &local_index, sizeof(local_index), false);
    UT_SetDataBuffer(UT_KEY(OS_ObjectIdGetById), &rptr, sizeof(rptr), false);
    actual = OS_CountSemGetInfo(1, &prop);

    UtAssert_True(actual == expected, "OS_CountSemGetInfo() (%ld) == OS_SUCCESS", (long)actual);
    UtAssert_True(prop.creator == 111, "prop.creator (%lu) == 111",
            (unsigned long)prop.creator);
    UtAssert_True(strcmp(prop.name, "ABC") == 0, "prop.name (%s) == ABC",
            prop.name);

    OSAPI_TEST_FUNCTION_RC(OS_CountSemGetInfo(0, NULL), OS_INVALID_POINTER);
}



/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void)
{
    UT_ResetState(0);
}

/*
 * Osapi_Test_Teardown
 *
 * Purpose:
 *   Called by the unit test tool to tear down the app after each test
 */
void Osapi_Test_Teardown(void)
{

}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_CountSemAPI_Init);
    ADD_TEST(OS_CountSemCreate);
    ADD_TEST(OS_CountSemDelete);
    ADD_TEST(OS_CountSemGive);
    ADD_TEST(OS_CountSemTake);
    ADD_TEST(OS_CountSemTimedWait);
    ADD_TEST(OS_CountSemGetIdByName);
    ADD_TEST(OS_CountSemGetInfo);
}





