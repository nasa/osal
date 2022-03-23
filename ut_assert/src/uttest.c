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

/*
 * File: uttest.c
 *
 * Purpose: This file contains functions to implement a standard way to execute unit tests.
 */

/*
 * Includes
 */

#include "utglobal.h"

/*
 * Global state instance
 */
UtAssert_Global_t UtAssert_Global;

enum
{
    UTASSERT_GROUP_DEFAULT = 0,
    UTASSERT_GROUP_SETUP,
    UTASSERT_GROUP_TEST,
    UTASSERT_GROUP_TEARDOWN,
    UTASSERT_GROUP_MAX
};

/*
 * Function Definitions
 */

void UtTest_AddCommon(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *TestName,
                      uint32 EntryType)
{
    UtTestDataBaseEntry_t UtTestDataBaseEntry;

    memset(&UtTestDataBaseEntry, 0, sizeof(UtTestDataBaseEntry));
    UtTestDataBaseEntry.Test     = Test;
    UtTestDataBaseEntry.Setup    = Setup;
    UtTestDataBaseEntry.Teardown = Teardown;
    if (TestName != NULL)
    {
        strncpy(UtTestDataBaseEntry.TestName, TestName, sizeof(UtTestDataBaseEntry.TestName) - 1);
    }

    UT_BSP_Lock();
    UtList_Add(UtAssert_Global.DataBasePtr, &UtTestDataBaseEntry, sizeof(UtTestDataBaseEntry_t), EntryType);
    UT_BSP_Unlock();
}

void UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *SequenceName)
{
    UtTest_AddCommon(Test, Setup, Teardown, SequenceName, UTASSERT_GROUP_TEST);
}

void UtTest_AddSetup(void (*Setup)(void), const char *SequenceName)
{
    UtTest_AddCommon(NULL, Setup, NULL, SequenceName, UTASSERT_GROUP_SETUP);
}

void UtTest_AddTeardown(void (*Teardown)(void), const char *SequenceName)
{
    UtTest_AddCommon(NULL, NULL, Teardown, SequenceName, UTASSERT_GROUP_TEARDOWN);
}

void UtTest_AddSubTest(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *GroupName,
                       const char *TestName)
{
    char        CompleteTestName[128];
    const char *GroupPtr;
    const char *TestPtr;

    /* Remove any common prefix between the two names.
     * They are often function names that all start with "Test_XXX"
     * and this repetitive information just becomes clutter.
     */
    GroupPtr = GroupName;
    TestPtr  = TestName;
    while (*GroupPtr != 0 && *GroupPtr == *TestPtr)
    {
        ++GroupPtr;
        ++TestPtr;
    }

    /*
     * Only break at an underscore(_) to avoid weird effects
     */
    while (TestPtr > TestName && *TestPtr != '_')
    {
        --TestPtr;
    }
    if (*TestPtr == '_')
    {
        ++TestPtr;
    }

    /*
     * Remove a remaining "Test_" prefix on the group name.
     * Again just to remove common repetitive content
     */
    GroupPtr = GroupName;
    if (strncmp(GroupPtr, "Test_", 5) == 0)
    {
        GroupPtr += 5;
    }

    (void)snprintf(CompleteTestName, sizeof(CompleteTestName), "%s.%s", GroupPtr, TestPtr);

    UtTest_AddCommon(Test, Setup, Teardown, CompleteTestName, UTASSERT_GROUP_TEST);
}

void UtTest_Run(void)
{
    UtListNode_t *         UtListMain;
    UtListNode_t *         UtListNode;
    UtTestDataBaseEntry_t *UtTestDataBaseEntry;

    UT_BSP_Lock();

    /*
     * The overall test sequence goes SETUP->TEST->TEARDOWN
     *
     * Combine all registered test groups into a merged group for execution.
     *
     * This could also (theoretically) randomize the order of the "TEST" group
     * while assembling this list, if there was a portable source of entropy.
     */
    UtListMain = UtList_GetHead(UtAssert_Global.DataBasePtr, UTASSERT_GROUP_DEFAULT);
    UtList_Merge(UtListMain, UtList_GetHead(UtAssert_Global.DataBasePtr, UTASSERT_GROUP_SETUP));
    UtList_Merge(UtListMain, UtList_GetHead(UtAssert_Global.DataBasePtr, UTASSERT_GROUP_TEST));
    UtList_Merge(UtListMain, UtList_GetHead(UtAssert_Global.DataBasePtr, UTASSERT_GROUP_TEARDOWN));

    UT_BSP_Unlock();

    /*
     * Run through the merged list in order
     */
    for (UtListNode = UtList_GetNext(UtListMain); !UtList_IsEnd(UtListMain, UtListNode);
         UtListNode = UtList_GetNext(UtListNode))
    {
        UtTestDataBaseEntry = UtList_GetObject(UtListNode);

        if (UtTestDataBaseEntry != NULL)
        {
            UtAssert_BeginTest(UtTestDataBaseEntry->TestName);

            UtAssert_SetContext(UTASSERT_CASETYPE_TSF);
            if (UtTestDataBaseEntry->Setup)
            {
                UtTestDataBaseEntry->Setup();
            }
            UtAssert_SetContext(UTASSERT_CASETYPE_FAILURE);
            if (UtTestDataBaseEntry->Test)
            {
                UtTestDataBaseEntry->Test();
                UtAssert_Global.ExecutedCount++;
            }
            UtAssert_SetContext(UTASSERT_CASETYPE_TTF);
            if (UtTestDataBaseEntry->Teardown)
            {
                UtTestDataBaseEntry->Teardown();
            }

            UtAssert_EndTest();
        }
    }

    UT_BSP_Lock();
    UtList_Destroy(UtAssert_Global.DataBasePtr);
    UT_BSP_Unlock();

    UT_BSP_EndTest(UtAssert_GetCounters());
}

void UtTest_EarlyInit(void)
{
    /*
     * Reset the test global variables, just in case.
     */
    memset(&UtAssert_Global, 0, sizeof(UtAssert_Global));
    UtAssert_Global.DataBasePtr = UtList_Create(UTASSERT_GROUP_MAX);
}
