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
 * Declarations and prototypes for OSAL BSP
 */

#ifndef OSAPI_BSP_H
#define OSAPI_BSP_H

#include "osconfig.h"
#include "common_types.h"

/****************************************************************************************
                    BSP LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/** @defgroup OSAPIBsp OSAL BSP low level access APIs
 *
 * These are for OSAL internal BSP information access to pass any BSP-specific
 * boot/command line/startup arguments through to the application, and return a
 * status code back to the OS after exit.
 *
 * Not intended for user application use
 * @{
 */

/*----------------------------------------------------------------

    Purpose: Sets BSP/platform-specific flags for the given resource type
             Flags and bit meanings are all platform defined.
 ------------------------------------------------------------------*/
void OS_BSP_SetResourceTypeConfig(uint32 ResourceType, uint32 ConfigOptionValue);

/*----------------------------------------------------------------

    Purpose: Gets BSP/platform-specific flags for the given resource type
             Flags and bit meanings are all platform defined.
 ------------------------------------------------------------------*/
uint32 OS_BSP_GetResourceTypeConfig(uint32 ResourceType);

/*----------------------------------------------------------------

    Purpose: Obtain the number of boot arguments passed from the bootloader
             or shell if supported by the platform

    Returns: The number of boot arguments, or 0 if no arguments were passed
             or not supported by the BSP.
 ------------------------------------------------------------------*/
uint32 OS_BSP_GetArgC(void);

/*----------------------------------------------------------------

    Purpose: Obtain an array of boot argument strings passed from the bootloader
             or shell if supported by the platform

    Returns: Pointer to char* array containing the argument strings, or NULL if
             no arguments are available or not supported by the BSP.

             The array is sized according to OS_BSP_GetArgC()
 ------------------------------------------------------------------*/
char *const *OS_BSP_GetArgV(void);

/*----------------------------------------------------------------

    Purpose: Sets the status to be returned to the shell or bootloader
             if supported by the platform.  The value is an integer with
             platform and application-defined meaning, but BSP's should
             attempt to provide consistent meaning for the following values

             OS_SUCCESS: normal status (default)
             OS_ERROR: any abnormal status

             Other more specific status values may be passed, with
             implementation-defined behavior.  Depending on the system
             capabilities, the BSP implementation may either pass the
             value through as-is, translate it to defined value, or
             ignore it.

             Note this does NOT cause the application to exit, it only
             sets the state that will be returned if/when the application
             exits itself at a future time.

 ------------------------------------------------------------------*/
void OS_BSP_SetExitCode(int32 code);

/**@}*/

#endif /* OSAPI_BSP_H */
