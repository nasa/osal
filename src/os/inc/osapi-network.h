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
 * Declarations and prototypes for network subsystem
 */

#ifndef OSAPI_NETWORK_H
#define OSAPI_NETWORK_H

#include "osconfig.h"
#include "common_types.h"

/**
 * @defgroup OSALAPINetwork Network ID APIs
 *
 * Provides some basic methods to query a network host name and ID
 *
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief  Gets the network ID of the local machine
 *
 * The ID is an implementation-defined value and may not be consistent
 * in meaning across different platform types.
 *
 * @note This API may be removed in a future version of OSAL due to
 *       inconsistencies between platforms.
 *
 * @return The ID or fixed value of -1 if the host id could not be found.
 *         Note it is not possible to differentiate between error codes and valid
 *         network IDs here. It is assumed, however, that -1 is never a valid ID.
 */
int32 OS_NetworkGetID(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Gets the local machine network host name
 *
 * If configured in the underlying network stack,
 * this function retrieves the local hostname of the system.
 *
 * @param[out]  host_name    Buffer to hold name information
 * @param[in]   name_len     Maximum length of host name buffer
 *
 * @return Execution status, see @ref OSReturnCodes
 */
int32 OS_NetworkGetHostName(char *host_name, size_t name_len);

/**@}*/

#endif /* OSAPI_NETWORK_H */
