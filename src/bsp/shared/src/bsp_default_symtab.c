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
 * File:  bsp_symtab.c
 *
 * Purpose:
 *   Application static symbol table default implementation.
 *
 * NOTE: This is isolated in a separate compilation unit, so that a user
 *   application may directly provide an OS_STATIC_SYMBOL_TABLE definition
 *   which will override this default.
 */

#include "osapi-module.h"
#include "bsp-impl.h"

OS_static_symbol_record_t OS_STATIC_SYMBOL_TABLE[] = {
    {"OS_Application_Startup", OS_Application_Startup}, {"OS_Application_Run", OS_Application_Run}, {NULL, NULL}};
