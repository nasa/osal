/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     os-shared-globaldefs.h
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 * Internal type/macro definitions used across the "shared" OSAPI layer.
 * These definitions are internal to OSAL but shared/referenced across all subsystems
 * so they are put into a common header file.
 */

#ifndef INCLUDE_OSAPI_SHARED_GLOBALDEFS_H_
#define INCLUDE_OSAPI_SHARED_GLOBALDEFS_H_

/* All subsystems reference the public API */
#include <osapi.h>

/*
 * The "common_record" is part of the generic ID mapping -
 * Functions in the internal API may use this as an abstract pointer.
 */
struct OS_common_record;
typedef struct OS_common_record OS_common_record_t;

/*
 * The "OS_shared_global_vars" keeps global state -
 * Functions in the internal API may use this as an abstract pointer.
 */
struct OS_shared_global_vars;
typedef struct OS_shared_global_vars OS_SharedGlobalVars_t;

/*
 * Wrapper for encoding of other types into a generic void* type required as argument
 * to callbacks and pthread entry/return values, etc.
 *
 * Note this can only encode types with sizes <= sizeof(void*)
 */
typedef union
{
   void *opaque_arg;
   OS_ArgCallback_t arg_callback_func;
   OS_TimerCallback_t timer_callback_func;
   osal_task_entry entry_func;
   uint32 value;
} OS_U32ValueWrapper_t;



/*
 * The "OS_DEBUG" is a no-op unless OSAL_CONFIG_DEBUG_PRINTF is enabled.
 * When enabled, it is a macro that includes function/line number info.
 */
#if defined(OSAL_CONFIG_DEBUG_PRINTF)
extern void OS_DebugPrintf(uint32 Level, const char *Func, uint32 Line, const char *Format, ...);
/* Debug printfs are compiled in, but also can be disabled by a run-time flag.
 * Note that the ##__VA_ARGS__ syntax works on GCC but might need tweaks for other compilers... */
#define OS_DEBUG_LEV(l,...)   OS_DebugPrintf(l, __func__, __LINE__, __VA_ARGS__);
#define OS_DEBUG(...)         OS_DEBUG_LEV(1,__VA_ARGS__)
#else
/* Debug printfs are not compiled in at all */
#define OS_DEBUG(...)
#endif



#endif  /* INCLUDE_OS_SHARED_GLOBALDEFS_H_ */

