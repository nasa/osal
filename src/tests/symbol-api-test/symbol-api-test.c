/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
    int32   status;
    cpuaddr SymAddress = 0;
    uint32  size; 
    char    fileName[128]; 

    /* Make the file system */
    status = OS_mkfs(0, "/ramdev1", "RAM", 512, 10240);
    UtAssert_True(status == OS_SUCCESS, "status after mkfs = %d", (int)status);

    status = OS_mount("/ramdev1", "/ram");
    UtAssert_True(status == OS_SUCCESS, "status after mount = %d", (int)status);

    /*
    ** dump the symbol table with a small limit to cause it to fail
    */
    size = 10;
    UtPrintf("Dumping symbol table with a limit of %d bytes\n", size);
    snprintf(fileName, sizeof(fileName), "/ram/SymbolTable%d.dat", size);

    status = OS_SymbolTableDump(fileName, size);
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("OS_SymbolTableDump API not implemented");
    }
    else
    {
        UtAssert_INT32_EQ(status, OS_ERR_OUTPUT_TOO_LARGE);
    }

    /*
    ** dump the symbol table with a high limit to pass
    */
    size = 1024 * 1024 * 5;
    UtPrintf("Dumping symbol table with a limit of %d bytes\n", size);
    snprintf(fileName, sizeof(fileName), "/ram/SymbolTable5MiB.dat");

    status = OS_SymbolTableDump(fileName, size);
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("OS_SymbolTableDump API not implemented");
    }
    else
    {
        UtAssert_INT32_EQ(status, OS_SUCCESS);
        if (status == OS_ERR_OUTPUT_TOO_LARGE)
        {
            UtPrintf("Symbol Table bigger than %d bytes. Consider increasing dump size\n", size);
        }
        else if (status == OS_ERR_NAME_TOO_LONG)
        {
            UtPrintf("OS_SymbolTableDump name to long, consider increasing OSAL_CONFIG_MAX_SYM_LEN\n");
        }
    }

    /*
    ** Test the symbol lookup
    */
    status = OS_SymbolLookup(&SymAddress, "OS_Application_Startup");
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("OS_SymbolLookup API not implemented");
    }
    else
    {
        UtAssert_True(status == OS_SUCCESS, "OS_SymbolLookup(OS_Application_Startup) = %d, Addr = %lx", (int)status,
                      (unsigned long)SymAddress);
    }

    /*
    ** Test a symbol lookup that does not exist
    */
    status = OS_SymbolLookup(&SymAddress, "ShouldNotExist");
    if (status == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssert_NA("OS_SymbolLookup API not implemented");
    }
    else
    {
        UtAssert_True(status != OS_SUCCESS, "OS_SymbolLookup(ShouldNotExist) = %d, Addr = %lx", (int)status,
                      (unsigned long)SymAddress);
    }
}

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
