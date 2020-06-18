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

/*
 * Filename: osapi_testcase.h
 *
 * Purpose: This file declares common variables and functions used among the
 *          unit test case source files
 *
 * Notes:
 *
 * Modification History:
 *   06/25/2015, A. Asp, Odyssey Space Research, LLC
 *    * Created
 *
 */


/*
 * Includes
 */
#include "osapi.h"              /* cfe.h not available from within osal. */

#include "utassert.h"
#include "uttest.h"
#include "utlist.h"

#include "osapi_stubs.h"
#include "osapi_adaptor.h"


#define OS_INCLUDE_MODULE_LOADER



/* Variables defined in osapi.c */
extern const uint32 MAX_PRIORITY;
extern const uint32 UNINITIALIZED;


/* Utility functions */
void deleteFnc(void);
void setTaskTableEntry(uint32 idx, int free, int id, char *name, int creator,
                       uint32 stack_size, uint32 priority, void *delete_hook_pointer);



/* -------------------------------------------------------------------------- */
#define ADD_TEST(test,setup,teardown) UtTest_Add((test), (setup), (teardown), #test)

void Osapi_Setup(void);
void Osapi_TearDown(void);
void Osapi_AddTestCase(void);


