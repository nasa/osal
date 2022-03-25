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

/*
 * File:  bsp_default_resourcecfg.c
 *
 * Purpose:
 *   Simple integer key/value table lookup to allow BSP-specific flags/options
 *   to be set for various resource types.  Meanings are all platform-defined.
 *
 */

#include "osapi-idmap.h"
#include "bsp-impl.h"

/* ---------------------------------------------------------
    OS_BSP_SetResourceTypeConfig()

    Helper function to register BSP-specific options.
   --------------------------------------------------------- */
void OS_BSP_SetResourceTypeConfig(uint32 ResourceType, uint32 ConfigOptionValue)
{
    if (ResourceType < OS_OBJECT_TYPE_USER)
    {
        OS_BSP_Global.ResoureConfig[ResourceType] = ConfigOptionValue;
    }
}

/* ---------------------------------------------------------
    OS_BSP_GetResourceTypeConfig()

    Helper function to register BSP-specific options.
   --------------------------------------------------------- */
uint32 OS_BSP_GetResourceTypeConfig(uint32 ResourceType)
{
    uint32 ConfigOptionValue;

    if (ResourceType < OS_OBJECT_TYPE_USER)
    {
        ConfigOptionValue = OS_BSP_Global.ResoureConfig[ResourceType];
    }
    else
    {
        ConfigOptionValue = 0;
    }

    return ConfigOptionValue;
}
