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
 * Declarations and prototypes for shell abstraction
 */

#ifndef OSAPI_SHELL_H
#define OSAPI_SHELL_H

#include "osconfig.h"
#include "common_types.h"

/** @defgroup OSAPIShell OSAL Shell APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Executes the command and sends output to a file
 *
 * Takes a shell command in and writes the output of that command to the specified file
 * The output file must be opened previously with write access (OS_WRITE_ONLY or OS_READ_WRITE).
 *
 * @param[in] Cmd     Command to pass to shell
 * @param[in] filedes File to send output to.
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERROR if the command was not executed properly
 * @retval #OS_ERR_INVALID_ID if the file descriptor passed in is invalid
 */
int32 OS_ShellOutputToFile(const char *Cmd, osal_id_t filedes);

/**@}*/

#endif /* OSAPI_SHELL_H */
