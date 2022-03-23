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
