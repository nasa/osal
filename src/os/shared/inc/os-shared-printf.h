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
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_PRINTF_H
#define OS_SHARED_PRINTF_H

#include "osapi-printf.h"
#include "os-shared-console.h"
#include "os-shared-globaldefs.h"

/****************************************************************************************
                 CONSOLE / DEBUG API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*----------------------------------------------------------------

    Purpose: Basic Console output implementation

   This function forwards the data from the console
   ring buffer into the actual output device/descriptor

   The data is already formatted, this just writes the characters.
 ------------------------------------------------------------------*/
void OS_ConsoleOutput_Impl(const OS_object_token_t *token);

#endif /* OS_SHARED_PRINTF_H */
