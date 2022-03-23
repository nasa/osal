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
 *
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-module.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*****************************************************************************/
/**
** \brief dummy_function stub function
**
** \par Description
**        This function is used by the OS API function, OS_SymbolLookup, which
**        requires a valid function for which to report the address.
**
** \par Assumptions, External Events, and Notes:
**        None
**
** \returns
**        Returns a user-defined status value.
**
******************************************************************************/
int32 dummy_function(void)
{
    int32 status;

    status = UT_DEFAULT_IMPL(dummy_function);

    return status;
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ModuleLoad' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ModuleLoad(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *module_id = UT_Hook_GetArgValueByName(Context, "module_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *module_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_MODULE);
    }
    else
    {
        *module_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ModuleUnload' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ModuleUnload(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t module_id = UT_Hook_GetArgValueByName(Context, "module_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_MODULE, module_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ModuleInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ModuleInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_module_prop_t *module_info = UT_Hook_GetArgValueByName(Context, "module_info", OS_module_prop_t *);
    int32             status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_ModuleInfo), module_info, sizeof(*module_info)) < sizeof(*module_info))
    {
        memset(module_info, 0, sizeof(*module_info));
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_SymbolLookup' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_SymbolLookup(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    cpuaddr *symbol_address = UT_Hook_GetArgValueByName(Context, "symbol_address", cpuaddr *);
    int32    status;

    /*
     * Register the context so a hook can do something with the parameters
     */

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status != OS_SUCCESS)
    {
        *symbol_address = 0xDEADBEEFU;
    }
    else if (UT_Stub_CopyToLocal(UT_KEY(OS_SymbolLookup), symbol_address, sizeof(*symbol_address)) <
             sizeof(*symbol_address))
    {
        /* return the dummy function when test didn't register anything else */
        *symbol_address = (cpuaddr)&dummy_function;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_ModuleSymbolLookup' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_ModuleSymbolLookup(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    cpuaddr *symbol_address = UT_Hook_GetArgValueByName(Context, "symbol_address", cpuaddr *);
    int32    status;

    /*
     * Register the context so a hook can do something with the parameters
     */

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status != OS_SUCCESS)
    {
        *symbol_address = 0xDEADBEEFU;
    }
    else if (UT_Stub_CopyToLocal(UT_KEY(OS_ModuleSymbolLookup), symbol_address, sizeof(*symbol_address)) <
             sizeof(*symbol_address))
    {
        /* return the dummy function when test didn't register anything else */
        *symbol_address = (cpuaddr)&dummy_function;
    }
}
