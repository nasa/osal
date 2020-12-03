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
 * @file osapi-clock.h
 */

#ifndef OSAPI_CLOCK_H
#define OSAPI_CLOCK_H

#include "osconfig.h"
#include "common_types.h"

/** @brief OSAL time */
typedef struct
{
    uint32 seconds;
    uint32 microsecs;
} OS_time_t;


/** @defgroup OSAPIClock OSAL Real Time Clock APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get the local time
 *
 * This function gets the local time from the underlying OS.
 *
 * @note Mission time management typically uses the cFE Time Service
 *
 * @param[out]  time_struct An OS_time_t that will be set to the current time
 *
 * @return Get local time status, see @ref OSReturnCodes
 */
int32 OS_GetLocalTime(OS_time_t *time_struct);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set the local time
 *
 * This function sets the local time on the underlying OS.
 *
 * @note Mission time management typically uses the cFE Time Services
 *
 * @param[in]  time_struct An OS_time_t containing the current time
 *
 * @return Set local time status, see @ref OSReturnCodes
 */
int32 OS_SetLocalTime(OS_time_t *time_struct);
/**@}*/

#endif
