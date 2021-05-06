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
 * General constants for OSAL that are shared across subsystems
 */

#ifndef OSAPI_CONSTANTS_H
#define OSAPI_CONSTANTS_H

#include "osconfig.h"
#include "common_types.h"

/*
** Defines for Queue Timeout parameters
*/
#define OS_PEND  (-1)
#define OS_CHECK (0)

/**
 * @brief Initializer for the osal_id_t type which will not match any valid value
 */
#define OS_OBJECT_ID_UNDEFINED ((osal_id_t) {0})

/**
 * @brief Constant that may be passed to OS_ForEachObject()/OS_ForEachObjectOfType() to match any
 * creator (i.e. get all objects)
 */
#define OS_OBJECT_CREATOR_ANY OS_OBJECT_ID_UNDEFINED

/**
 * @brief Maximum length of a local/native path name string
 *
 * This is a concatenation of the OSAL virtual path with the system
 * mount point or device name
 */
#define OS_MAX_LOCAL_PATH_LEN (OS_MAX_PATH_LEN + OS_FS_PHYS_NAME_LEN)

#endif /* OSAPI_CONSTANTS_H */
