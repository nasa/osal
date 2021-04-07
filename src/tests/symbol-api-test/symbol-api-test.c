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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_types.h"
#include "osapi.h"
#include "utassert.h"
#include "uttest.h"
#include "utbsp.h"

/* *************************************** MAIN ************************************** */

void TestSymbolApi(void)
{
#ifdef OS_INCLUDE_MODULE_LOADER
    int32   status;
    cpuaddr SymAddress;

    /* Make the file system */
    status = OS_mkfs(0, "/ramdev0", "RAM", 512, 2048);
    UtAssert_True(status == OS_SUCCESS, "status after mkfs = %d", (int)status);

    status = OS_mount("/ramdev0", "/ram");
    UtAssert_True(status == OS_SUCCESS, "status after mount = %d", (int)status);

    /*
    ** dump the symbol table with a 32768 byte limit
    */
    UtPrintf("Dumping symbol table with a limit of 32768 bytes\n");
    status = OS_SymbolTableDump("/ram/SymbolTable32k.dat", 32768);
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("Module API not implemented");
    }
    else
    {
        UtAssert_True(status == OS_SUCCESS, "status after 32k OS_SymbolTableDump = %d", (int)status);
    }

    /*
    ** dump the symbol table with a 128k byte limit
    */
    UtPrintf("Dumping symbol table with a limit of 131072 bytes\n");
    status = OS_SymbolTableDump("/ram/SymbolTable128k.dat", 131072);
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("Module API not implemented");
    }
    else
    {
        UtAssert_True(status == OS_SUCCESS, "status after 128k OS_SymbolTableDump = %d", (int)status);
    }

    /*
    ** dump the symbol table with a 512k byte limit
    */
    UtPrintf("Dumping symbol table with a limit of 524288 bytes\n");
    status = OS_SymbolTableDump("/ram/SymbolTable512k.dat", 524288);
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("Module API not implemented");
    }
    else
    {
        UtAssert_True(status == OS_SUCCESS, "status after 512k OS_SymbolTableDump = %d", (int)status);
    }

    /*
    ** Test the symbol lookup
    */
    status = OS_SymbolLookup(&SymAddress, "OS_Application_Startup");
    UtAssert_True(status == OS_SUCCESS, "OS_SymbolLookup(OS_Application_Startup) = %d, Addr = %lx", (int)status,
                  (unsigned long)SymAddress);

    /*
    ** Test a symbol lookup that does not exist
    */
    status = OS_SymbolLookup(&SymAddress, "ShouldNotExist");
    UtAssert_True(status != OS_SUCCESS, "OS_SymbolLookup(ShouldNotExist) = %d, Addr = %lx", (int)status,
                  (unsigned long)SymAddress);

#else
    /* If the module loader is not present, generate an N/A test case just to indicate that the test ran */
    UtAssert_True(1, "Module loader not present");
#endif
} /* end TestSymbolApi */

void UtTest_Setup(void)
{
    if (OS_API_Init() != OS_SUCCESS)
    {
        UtAssert_Abort("OS_API_Init() failed");
    }

    /* the test should call OS_API_Teardown() before exiting */
    UtTest_AddTeardown(OS_API_Teardown, "Cleanup");

    /*
     * Register the test setup and check routines in UT assert
     */
    UtTest_Add(TestSymbolApi, NULL, NULL, "SymbolApiTest");
}
