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
 * File:  bsp_loader.c
 *
 * Purpose:
 *   Include the Static loader. This is not part of the OSAL, so it cannot be 
 *   included in the source. This file allows the static loader to be included
 *   if it's available. If not, simply undef the OS_STATIC_LOADER define
 *   in osconfig.h
 *
 * History:
 */
#include "osapi.h"

#ifdef OS_STATIC_LOADER
   #include "LzmaDec.c"
   #include "loadstaticloadfile.c"
#endif

