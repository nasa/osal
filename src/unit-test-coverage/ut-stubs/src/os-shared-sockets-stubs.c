/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-sockets header
 */

#include "os-shared-sockets.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CreateSocketName()
 * ----------------------------------------------------
 */
void OS_CreateSocketName(const OS_object_token_t *token, const OS_SockAddr_t *Addr, const char *parent_name)
{
    UT_GenStub_AddParam(OS_CreateSocketName, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_CreateSocketName, const OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_CreateSocketName, const char *, parent_name);

    UT_GenStub_Execute(OS_CreateSocketName, Basic, NULL);
}
