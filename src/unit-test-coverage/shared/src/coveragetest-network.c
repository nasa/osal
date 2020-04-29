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
 * \file     coveragetest-network.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-network.h"


void Test_OS_NetworkAPI_Init(void)
{
    /*
     * Test Case For:
     * int32 OS_NetworkAPI_Init(void)
     */
    int32 expected = OS_SUCCESS;
    int32 actual = OS_NetworkAPI_Init();

    UtAssert_True(actual == expected, "OS_NetworkAPI_Init() (%ld) == OS_SUCCESS", (long)actual);
}

void Test_OS_NetworkGetHostName (void)
{
    /*
     * Test Case For:
     * int32 OS_NetworkGetHostName(char *host_name, uint32 name_len)
     */
    char Buffer[32];
    int32 expected = OS_SUCCESS;
    int32 actual = ~OS_SUCCESS;

    actual = OS_NetworkGetHostName(Buffer, sizeof(Buffer));
    UtAssert_True(actual == expected, "OS_NetworkGetHostName() (%ld) == OS_SUCCESS", (long)actual);

    UT_SetForceFail(UT_KEY(OS_NetworkGetHostName_Impl), -4444);
    expected = -4444;
    actual = OS_NetworkGetHostName(Buffer, sizeof(Buffer));
    UtAssert_True(actual == expected, "OS_NetworkGetHostName(impl error) (%ld) == -4444", (long)actual);

    expected = OS_INVALID_POINTER;
    actual = OS_NetworkGetHostName(NULL, sizeof(Buffer));
    UtAssert_True(actual == expected, "OS_NetworkGetHostName(Ptr=NULL) (%ld) == OS_INVALID_POINTER", (long)actual);

    expected = OS_ERROR;
    actual = OS_NetworkGetHostName(Buffer, 0);
    UtAssert_True(actual == expected, "OS_NetworkGetHostName(Size=0) (%ld) == OS_ERROR", (long)actual);
}

void Test_OS_NetworkGetID (void)
{
    /*
     * Test Case For:
     * int32 OS_NetworkGetID(void)
     */
    int32 expected;
    int32 actual;

    expected = 42;
    actual = OS_NetworkGetID();
    UtAssert_True(actual == expected, "OS_NetworkGetID(nominal) (%ld) == 42", (long)actual);

    UT_SetForceFail(UT_KEY(OS_NetworkGetID_Impl), -5555);
    expected = -1;
    actual = OS_NetworkGetID();
    UtAssert_True(actual == expected, "OS_NetworkGetID(error) (%ld) == -1", (long)actual);
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
    ADD_TEST(OS_NetworkAPI_Init);
    ADD_TEST(OS_NetworkGetID);
    ADD_TEST(OS_NetworkGetHostName);
}





