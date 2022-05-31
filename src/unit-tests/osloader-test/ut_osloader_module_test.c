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

/*================================================================================*
** File:  ut_osloader_module_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_osloader_module_test.h"
#include "ut_osloader_test_platforms.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: OS_ModuleLoad
** Purpose: Loads the new ELF object module into the RTOS
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NO_FREE_IDS if the module table is full
**          OS_ERR_NAME_TAKEN if the module name has already been used
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_module_load_test()
{
    int       i;
    osal_id_t module_id  = OS_OBJECT_ID_UNDEFINED;
    osal_id_t module_id2 = OS_OBJECT_ID_UNDEFINED;
    char      module_name[UT_OS_NAME_BUFF_SIZE];
    char      module_file_name[UT_OS_PATH_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* API Not implemented */

    if (!UT_IMPL(OS_ModuleLoad(0, "TestModule", UT_OS_GENERIC_MODULE_NAME1, OS_MODULE_FLAG_LOCAL_SYMBOLS)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg-1 */

    UT_RETVAL(OS_ModuleLoad(0, "TestModule", UT_OS_GENERIC_MODULE_NAME1, OS_MODULE_FLAG_LOCAL_SYMBOLS),
              OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Null-pointer-arg-2 */

    UT_RETVAL(OS_ModuleLoad(&module_id, 0, UT_OS_GENERIC_MODULE_NAME1, OS_MODULE_FLAG_LOCAL_SYMBOLS),
              OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #3 Null-pointer-arg-3 */

    UT_RETVAL(OS_ModuleLoad(&module_id, "TestModule", 0, OS_MODULE_FLAG_LOCAL_SYMBOLS), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* Name too long */

    memset(module_name, 'x', sizeof(module_name) - 1);
    module_name[sizeof(module_name) - 1] = 0;
    UT_RETVAL(OS_ModuleLoad(&module_id, module_name, UT_OS_GENERIC_MODULE_NAME1, OS_MODULE_FLAG_LOCAL_SYMBOLS),
              OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* Path invalid */

    memset(module_file_name, 'x', OS_MAX_PATH_LEN - 1);
    module_file_name[OS_MAX_PATH_LEN - 1] = 0;
    UT_RETVAL(OS_ModuleLoad(&module_id, "TestModule", module_file_name, OS_MODULE_FLAG_LOCAL_SYMBOLS),
              OS_FS_ERR_PATH_INVALID);

    /*-----------------------------------------------------*/
    /* #4 No-free-IDs */

    for (i = 0; i <= OS_MAX_MODULES; i++)
    {
        snprintf(module_name, sizeof(module_name), UT_OS_GENERIC_MODULE_NAME_TEMPLATE, i);
        snprintf(module_file_name, sizeof(module_file_name), UT_OS_GENERIC_MODULE_FILE_TEMPLATE, i);

        if (i == OS_MAX_MODULES)
        {
            UT_RETVAL(OS_ModuleLoad(&module_id, module_name, module_file_name, OS_MODULE_FLAG_LOCAL_SYMBOLS),
                      OS_ERR_NO_FREE_IDS);
        }
        else if (!UT_SETUP(OS_ModuleLoad(&module_id, module_name, module_file_name, OS_MODULE_FLAG_LOCAL_SYMBOLS)))
        {
            break;
        }
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    /* #5 Duplicate-name */

    /* Setup */
    if (UT_SETUP(OS_ModuleLoad(&module_id2, "DUPLICATE", UT_OS_GENERIC_MODULE_NAME2, OS_MODULE_FLAG_LOCAL_SYMBOLS)))
    {
        UT_RETVAL(OS_ModuleLoad(&module_id, "DUPLICATE", UT_OS_GENERIC_MODULE_NAME2, OS_MODULE_FLAG_LOCAL_SYMBOLS),
                  OS_ERR_NAME_TAKEN);

        /* Reset test environment */
        UT_TEARDOWN(OS_ModuleUnload(module_id2));
    }

    /*-----------------------------------------------------*/
    /* #6 OS-call-failure */

    /*-----------------------------------------------------*/
    /* #7 Nominal */

    UT_NOMINAL(OS_ModuleLoad(&module_id, "Good", UT_OS_GENERIC_MODULE_NAME2, OS_MODULE_FLAG_LOCAL_SYMBOLS));

    /* Reset test environment */
    UT_TEARDOWN(OS_ModuleUnload(module_id));
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_ModuleUnload
** Purpose: Unloads the module from the running RTOS
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid module id
**          OS_ERROR if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_module_unload_test()
{
    osal_id_t module_id = OS_OBJECT_ID_UNDEFINED;

    /*-----------------------------------------------------*/
    /* API Not implemented */

    if (!UT_IMPL(OS_ModuleUnload(OS_OBJECT_ID_UNDEFINED)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #1 Invalid-ID-arg */

    UT_RETVAL(OS_ModuleUnload(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_ModuleUnload(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 OS-call-failure */

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    /* Setup */
    if (UT_SETUP(OS_ModuleLoad(&module_id, "Good", UT_OS_GENERIC_MODULE_NAME2, OS_MODULE_FLAG_LOCAL_SYMBOLS)))
    {
        UT_NOMINAL(OS_ModuleUnload(module_id));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_ModuleInfo
** Purpose: Returns module information about a given module id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid module id
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_module_info_test()
{
    osal_id_t        module_id = OS_OBJECT_ID_UNDEFINED;
    OS_module_prop_t module_info;

    /*-----------------------------------------------------*/
    /* API Not implemented */

    if (!UT_IMPL(OS_ModuleInfo(OS_OBJECT_ID_UNDEFINED, &module_info)))
    {
        return;
    }

    /*-----------------------------------------------------*/
    /* #2 Invalid-ID-arg */

    UT_RETVAL(OS_ModuleInfo(UT_OBJID_INCORRECT, &module_info), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_ModuleInfo(OS_OBJECT_ID_UNDEFINED, &module_info), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    /* Setup */
    if (UT_SETUP(OS_ModuleLoad(&module_id, "Good", UT_OS_GENERIC_MODULE_NAME2, OS_MODULE_FLAG_LOCAL_SYMBOLS)))
    {
        /* Invalid-pointer-arg */
        UT_RETVAL(OS_ModuleInfo(module_id, NULL), OS_INVALID_POINTER);

        /* Nominal */
        UT_NOMINAL(OS_ModuleInfo(module_id, &module_info));

        UT_TEARDOWN(OS_ModuleUnload(module_id));
    }
}

/*================================================================================*
** End of File: ut_osloader_module_test.c
**================================================================================*/
