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

/*
 * File: osapi-os-net.h
 *
 * Author:  Alan Cudmore Code 582
 *
 * Purpose: Contains functions prototype definitions and variables declarations
 *          for the OS Abstraction Layer, Network Module
 */

#ifndef _osapi_network_
#define _osapi_network_

#ifdef OSAL_OMIT_DEPRECATED
#error This header header is deprecated
#endif

/*
 * BACKWARD COMPATIBILITY HEADER
 *
 * OSAPI headers have beens split into subsystem-based components.
 *
 * This header is now just a wrapper that includes the same general
 * set of components that this file traditionally supplied.
 */
#include "osapi-sockets.h"
#include "osapi-network.h"

#endif
