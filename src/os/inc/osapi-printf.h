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
 * Declarations and prototypes for printf/console output
 */

#ifndef OSAPI_PRINTF_H
#define OSAPI_PRINTF_H

#include "osconfig.h"
#include "common_types.h"

/** @defgroup OSAPIPrintf OSAL Printf APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Abstraction for the system printf() call
 *
 * This function abstracts out the printf type statements. This is
 * useful for using OS- specific thats that will allow non-polled
 * print statements for the real time systems.
 *
 * Operates in a manner similar to the printf() call defined by the standard C
 * library and takes all the parameters and formatting options of printf.
 * This abstraction may implement additional buffering, if necessary,
 * to improve the real-time performance of the call.
 *
 * Strings (including terminator) longer than #OS_BUFFER_SIZE will be truncated.
 *
 * The output of this routine also may be dynamically enabled or disabled by
 * the OS_printf_enable() and OS_printf_disable() calls, respectively.
 *
 * @param[in] string Format string, followed by additional arguments
 */
void OS_printf(const char *string, ...) OS_PRINTF(1, 2);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief This function disables the output from OS_printf.
 */
void OS_printf_disable(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief This function enables the output from OS_printf.
 *
 */
void OS_printf_enable(void);
/**@}*/

#endif /* OSAPI_PRINTF_H */
