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

/* OSAL coverage stub replacement for shellLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "OCS_shellLib.h"

OCS_STATUS OCS_shellGenericInit(const char *config, int stackSize, const char *shellName, char **pShellName,
                                OCS_BOOL interactive, OCS_BOOL loginAccess, int fdin, int fdout, int fderr)
{
    return (UT_DEFAULT_IMPL(OCS_shellGenericInit));
}
