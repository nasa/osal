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

/* OSAL coverage stub replacement for file mode bits
 * this file is shared by several UT replacement headers */
#ifndef _OSAL_UT_OSLOADER_H_
#define _OSAL_UT_OSLOADER_H_

#include <common_types.h>
#include <overrides/symLib.h>

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

/*
 * Prototype for table init function (needs to be called from UT)
 */
int32 OS_VxWorks_ModuleAPI_Impl_Init(void);

/*
 * Invokes OS_SymTableIterator_Impl() with the given arguments.
 * This is normally a static function but exposed via a non-static wrapper for UT purposes.
 */
int32 Osapi_Internal_CallIteratorFunc(char *name, void* val, uint32 TestSize, uint32 SizeLimit);

#endif /* _OSAL_UT_OSLOADER_H_ */

