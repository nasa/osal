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
 * \file
 *
 * Owner: Alan Cudmore
 * Date:  April 2013
 */

#ifndef UT_OSCORE_TASK_TEST_H
#define UT_OSCORE_TASK_TEST_H

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_support.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_TASK_LIST_LEN (OS_MAX_TASKS + 10)

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
** Function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_task_create_test(void);
void UT_os_task_delete_test(void);
void UT_os_task_install_delete_handler_test(void);
void UT_os_task_exit_test(void);
void UT_os_task_set_priority_test(void);
void UT_os_task_register_test(void);
void UT_os_task_get_id(void);
void UT_os_task_get_id_by_name_test(void);
void UT_os_task_get_info_test(void);
void UT_os_task_delay_test(void);
void UT_os_task_get_id_test(void);
void UT_os_task_getid_by_sysdata_test(void);

/*--------------------------------------------------------------------------------*/

#endif /* UT_OSCORE_TASK_TEST_H */
