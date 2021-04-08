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
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-portable-posix-io
 */

#ifndef UT_ADAPTOR_PORTABLE_POSIX_IO_H
#define UT_ADAPTOR_PORTABLE_POSIX_IO_H

#include "common_types.h"

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/
void UT_PortablePosixIOTest_Set_Selectable(osal_index_t local_id, bool is_selectable);
void UT_PortablePosixIOTest_Set_FD(osal_index_t local_id, int fd);
bool UT_PortablePosixIOTest_Get_Selectable(osal_index_t local_id);
void UT_PortablePosixIOTest_ResetImpl(osal_index_t local_id);

#endif /* UT_ADAPTOR_PORTABLE_POSIX_IO_H */
