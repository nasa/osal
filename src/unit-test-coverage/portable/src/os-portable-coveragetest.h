/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 * \ingroup portable
 *
 * Declarations and prototypes for os-portable-coveragetest
 */

#ifndef OS_PORTABLE_COVERAGETEST_H
#define OS_PORTABLE_COVERAGETEST_H

/*
 * Includes
 */

#include "utassert.h"
#include "uttest.h"
#include "utstubs.h"

#include "os-shared-globaldefs.h"

#define OSAPI_TEST_FUNCTION_RC(func, args, exp)                                                                \
    {                                                                                                          \
        int32 rcexp = exp;                                                                                     \
        int32 rcact = func args;                                                                               \
        UtAssert_True(rcact == rcexp, "%s%s (%ld) == %s (%ld)", #func, #args, (long)rcact, #exp, (long)rcexp); \
    }

#define ADD_TEST(test) UtTest_Add((Test_##test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

/*
 * The default/primary table index used by most coverage tests.
 */
#define UT_INDEX_0 OSAL_INDEX_C(0)

/*
 * A secondary table index for coverage tests which require
 * more than one entry
 */
#define UT_INDEX_1 OSAL_INDEX_C(1)

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void);
void Osapi_Test_Teardown(void);

#endif /* OS_PORTABLE_COVERAGETEST_H */
