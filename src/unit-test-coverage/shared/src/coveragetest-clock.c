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
 * \file     coveragetest-clock.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "os-shared-coveragetest.h"
#include "os-shared-clock.h"

void Test_OS_GetLocalTime(void)
{
    /*
     * Test Case For:
     * int32 OS_GetLocalTime(OS_time_t *time_struct)
     */
    OS_time_t time_struct;
    int32     expected = OS_SUCCESS;
    int32     actual   = OS_GetLocalTime(&time_struct);

    UtAssert_True(actual == expected, "OS_GetLocalTime() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_GetLocalTime(NULL);
    UtAssert_True(actual == expected, "OS_GetLocalTime() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_SetLocalTime(void)
{
    /*
     * Test Case For:
     * int32 OS_SetLocalTime(OS_time_t *time_struct)
     */
    OS_time_t time_struct;
    int32     expected = OS_SUCCESS;
    int32     actual   = OS_SetLocalTime(&time_struct);

    UtAssert_True(actual == expected, "OS_SetLocalTime() (%ld) == OS_SUCCESS", (long)actual);

    expected = OS_INVALID_POINTER;
    actual   = OS_SetLocalTime(NULL);
    UtAssert_True(actual == expected, "OS_SetLocalTime() (%ld) == OS_INVALID_POINTER", (long)actual);
}

void Test_OS_TimeAccessConversions(void)
{
    /*
     * Test cases for the various time access and conversion functions:
     *
     * int64 OS_TimeGetTotalSeconds(OS_time_t tm)
     * int64 OS_TimeGetTotalMilliseconds(OS_time_t tm)
     * int64 OS_TimeGetTotalMicroseconds(OS_time_t tm)
     * int64 OS_TimeGetTotalNanoseconds(OS_time_t tm)
     *
     * uint32 OS_TimeGetSubsecondsPart(OS_time_t tm)
     * uint32 OS_TimeGetMillisecondsPart(OS_time_t tm)
     * uint32 OS_TimeGetMicrosecondsPart(OS_time_t tm)
     * uint32 OS_TimeGetNanosecondsPart(OS_time_t tm)
     *
     * OS_time_t OS_TimeAssembleFromMilliseconds(int64 seconds, uint32 milliseconds)
     * OS_time_t OS_TimeAssembleFromMicroseconds(int64 seconds, uint32 microseconds)
     * OS_time_t OS_TimeAssembleFromNanoseconds(int64 seconds, uint32 nanoseconds)
     * OS_time_t OS_TimeAssembleFromSubseconds(int64 seconds, uint32 subseconds)
     *
     * OS_time_t OS_TimeAdd(OS_time_t time1, OS_time_t time2)
     * OS_time_t OS_TimeSubtract(OS_time_t time1, OS_time_t time2)
     */
    OS_time_t t1;
    OS_time_t t2;
    OS_time_t t3;
    OS_time_t t4;

    /* To base-2 32-bit fixed point: 0.234567890 s * 0x100000000 ~= 0x3c0ca428 */
    t1 = OS_TimeAssembleFromNanoseconds(1, 234567890);

    /* From base-2 32-bit fixed point: 0x87654321 / 0x100000000 ~= 0.528888888 s */
    t2 = OS_TimeAssembleFromSubseconds(2, 0x87654321);

    /* To base-2 32-bit fixed point: 0.045678 s * 0x100000000 ~= 0x0bb18dad */
    t3 = OS_TimeAssembleFromMicroseconds(0, 45678);

    /* To base-2 32-bit fixed point: 0.901 s * 0x100000000 ~= 0xe6a7ef9e */
    t4 = OS_TimeAssembleFromMilliseconds(1, 901);

    /* These functions only return the total (whole + fraction) in the requested units */
    UtAssert_UINT32_EQ(OS_TimeGetTotalSeconds(t1), 1);
    UtAssert_UINT32_EQ(OS_TimeGetTotalSeconds(t2), 2);
    UtAssert_UINT32_EQ(OS_TimeGetTotalSeconds(t3), 0);
    UtAssert_UINT32_EQ(OS_TimeGetTotalSeconds(t4), 1);

    UtAssert_UINT32_EQ(OS_TimeGetTotalMilliseconds(t1), 1234);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMilliseconds(t2), 2528);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMilliseconds(t3), 45);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMilliseconds(t4), 1901);

    UtAssert_UINT32_EQ(OS_TimeGetTotalMicroseconds(t1), 1234567);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMicroseconds(t2), 2528888);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMicroseconds(t3), 45678);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMicroseconds(t4), 1901000);

    /* Note: Nanoseconds/Subseconds may not be exact due to limitations of OS_time_t resolution */
    UtAssert_UINT32_EQ(OS_TimeGetTotalNanoseconds(t1), 1234567800);
    UtAssert_UINT32_EQ(OS_TimeGetTotalNanoseconds(t2), 2528888800);
    UtAssert_UINT32_EQ(OS_TimeGetTotalNanoseconds(t3), 45678000);
    UtAssert_UINT32_EQ(OS_TimeGetTotalNanoseconds(t4), 1901000000);

    /* These functions only return the fractional part, not the whole part */
    UtAssert_UINT32_EQ(OS_TimeGetSubsecondsPart(t1), 0x3c0ca2a6);
    UtAssert_UINT32_EQ(OS_TimeGetSubsecondsPart(t2), 0x876541a4);
    UtAssert_UINT32_EQ(OS_TimeGetSubsecondsPart(t3), 0x0bb18dad);
    UtAssert_UINT32_EQ(OS_TimeGetSubsecondsPart(t4), 0xe6a7ef9e);

    UtAssert_UINT32_EQ(OS_TimeGetMillisecondsPart(t1), 234);
    UtAssert_UINT32_EQ(OS_TimeGetMillisecondsPart(t2), 528);
    UtAssert_UINT32_EQ(OS_TimeGetMillisecondsPart(t3), 45);
    UtAssert_UINT32_EQ(OS_TimeGetMillisecondsPart(t4), 901);

    UtAssert_UINT32_EQ(OS_TimeGetMicrosecondsPart(t1), 234567);
    UtAssert_UINT32_EQ(OS_TimeGetMicrosecondsPart(t2), 528888);
    UtAssert_UINT32_EQ(OS_TimeGetMicrosecondsPart(t3), 45678);
    UtAssert_UINT32_EQ(OS_TimeGetMicrosecondsPart(t4), 901000);

    UtAssert_UINT32_EQ(OS_TimeGetNanosecondsPart(t1), 234567800);
    UtAssert_UINT32_EQ(OS_TimeGetNanosecondsPart(t2), 528888800);
    UtAssert_UINT32_EQ(OS_TimeGetNanosecondsPart(t3), 45678000);
    UtAssert_UINT32_EQ(OS_TimeGetNanosecondsPart(t4), 901000000);

    /* Simple Add/Subtract */
    t3 = OS_TimeAdd(t1, t2);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMilliseconds(t3), 3763);
    t4 = OS_TimeSubtract(t3, t2);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMilliseconds(t4), 1234);

    /* Add/Subtract that will require carry */
    t1 = OS_TimeAssembleFromNanoseconds(3, 777777777);
    t2 = OS_TimeAssembleFromNanoseconds(4, 888888888);

    t3 = OS_TimeAdd(t1, t2);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMilliseconds(t3), 8666);
    t4 = OS_TimeSubtract(t3, t2);
    UtAssert_UINT32_EQ(OS_TimeGetTotalMilliseconds(t4), 3777);
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
void Osapi_Test_Teardown(void) {}

/*
 * Register the test cases to execute with the unit test tool
 */
void UtTest_Setup(void)
{
    ADD_TEST(OS_GetLocalTime);
    ADD_TEST(OS_SetLocalTime);
    ADD_TEST(OS_TimeAccessConversions);
}
