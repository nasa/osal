/**
 * \file
 *
 * \ingroup  rtems
 *
 */

#ifndef OS_IMPL_LOADER_H
#define OS_IMPL_LOADER_H

#include "osconfig.h"
#include <dlfcn.h>

#include <rtems/rtl/rtl.h>
#include <rtems/rtl/rtl-unresolved.h>

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/*
 * A local lookup table for RTEMS-specific information.
 * This is not directly visible to the outside world.
 */
typedef struct
{
    /* cppcheck-suppress unusedStructMember */
    void *dl_handle;
} OS_impl_module_internal_record_t;

extern OS_impl_module_internal_record_t OS_impl_module_table[OS_MAX_MODULES];

#endif /* OS_IMPL_LOADER_H */
