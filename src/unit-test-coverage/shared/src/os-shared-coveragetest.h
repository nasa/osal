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
 * \ingroup shared
 *
 * Declarations and prototypes for os-shared-coveragetest
 */

#ifndef OS_SHARED_COVERAGETEST_H
#define OS_SHARED_COVERAGETEST_H

#include "utassert.h"
#include "uttest.h"
#include "utstubs.h"
#include "osapi.h"

#include "os-shared-idmap.h"

/*
 * Macro to call a function and check its int32 return code
 */
#define OSAPI_TEST_FUNCTION_RC(func, exp)                                                             \
    {                                                                                                 \
        int32 rcexp = exp;                                                                            \
        int32 rcact = func;                                                                           \
        UtAssert_True(rcact == rcexp, "%s (%ld) == %s (%ld)", #func, (long)rcact, #exp, (long)rcexp); \
    }

/*
 * A union type allowing the osal_id_t to be manipulated as a uint32.
 * Normally application code would NOT do this, but coverage test can
 * because it has inside knowledge of the ID value structure.
 */
typedef union
{
    osal_id_t id;
    uint32    val;
} UT_idbuf_t;

#define OSAPI_TEST_OBJID(act, op, exp)                                                              \
    {                                                                                               \
        UT_idbuf_t idexp = {.id = exp};                                                             \
        UT_idbuf_t idact = {.id = act};                                                             \
        UtAssert_True(memcmp(&idexp, &idact, sizeof(osal_id_t)) op 0, "%s (%lu) %s %s (%lu)", #act, \
                      (unsigned long)idact.val, #op, #exp, (unsigned long)idexp.val);               \
    }

/*
 * Macro to add a test case to the list of tests to execute
 */
#define ADD_TEST(test) UtTest_Add((Test_##test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

#define UT_OBJID_1     ((osal_id_t) {1})
#define UT_OBJID_2     ((osal_id_t) {2})
#define UT_OBJID_OTHER ((osal_id_t) {0x12345})
#define UT_OBJID_MAX   ((osal_id_t) {0xFFFFFFFF})

#define UT_INDEX_0 OSAL_INDEX_C(0)
#define UT_INDEX_1 OSAL_INDEX_C(1)
#define UT_INDEX_2 OSAL_INDEX_C(2)

/*
 * Set up a coverage test iterator of the given type.
 *
 * The OS_ObjectIdIteratorGetNext() stub routine will be configured
 * to return the given range of IDs.
 */
void OS_UT_SetupIterator(osal_objtype_t obj_type, osal_index_t first_idx, osal_index_t num_entries);

/*
 * Set up the UT stubs for the target entry of the next test case.
 *
 * This configures the OS_ObjectIdGetById() stub to return a token
 * that refers to the given entry index.
 */
void OS_UT_SetupTestTargetIndex(osal_objtype_t obj_type, osal_index_t test_idx);

/*
 * Set up the UT stubs for a "get info" test.
 *
 * This sets up a single entry in the global table with the given name and
 * ID value.  It also configures the OS_ObjectIdGetById() stub to return a
 * token that refers to the same table entry.
 */
void OS_UT_SetupBasicInfoTest(osal_objtype_t obj_type, osal_index_t test_idx, const char *name, osal_id_t creator);

/*
 * Setup function prior to every test
 */
void Osapi_Test_Setup(void);

/*
 * Teardown function after every test
 */
void Osapi_Test_Teardown(void);

#endif /* OS_SHARED_COVERAGETEST_H */
