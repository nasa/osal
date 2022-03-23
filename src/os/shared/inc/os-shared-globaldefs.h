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
 * Internal type/macro definitions used across the "shared" OSAPI layer.
 * These definitions are internal to OSAL but shared/referenced across all subsystems
 * so they are put into a common header file.
 */

#ifndef OS_SHARED_GLOBALDEFS_H
#define OS_SHARED_GLOBALDEFS_H

/* All subsystems reference the same config, common types, and other constants */
#include "osconfig.h"
#include "common_types.h"
#include "osapi-constants.h"
#include "osapi-error.h"
#include "osapi-macros.h"

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
 * The "OS_object_token" tracks to the type of lock currently held
 * and the specific object record the requested operation should
 * execute on.  All operations start by obtaining a token, which must
 * be released when the operation is complete.
 */
struct OS_object_token;
typedef struct OS_object_token OS_object_token_t;

/*
 * Wrapper for encoding of other types into a generic void* type required as argument
 * to callbacks and pthread entry/return values, etc.
 *
 * This is used where OSAL needs to pass non-pointer/integer values through an interface
 * that accepts a void* opaque pass-through argument.
 *
 * Note this can only encode types with sizes <= sizeof(void*)
 */
typedef union
{
    void *           opaque_arg;
    OS_ArgCallback_t arg_callback_func;
    osal_id_t        id;
    osal_index_t     idx;
} OS_VoidPtrValueWrapper_t;

/*
 * The wrapper structure size should be equal to void* - if not this means
 * one or more of the other members are bigger than void*, and therefore cannot
 * be passed directly through the intended interface
 */
CompileTimeAssert(sizeof(OS_VoidPtrValueWrapper_t) == sizeof(void *), VoidValueWrapperSize);

/*
 * The "OS_DEBUG" is a no-op unless OSAL_CONFIG_DEBUG_PRINTF is enabled.
 * When enabled, it is a macro that includes function/line number info.
 */
#if defined(OSAL_CONFIG_DEBUG_PRINTF)
extern void OS_DebugPrintf(uint32 Level, const char *Func, uint32 Line, const char *Format, ...);
/* Debug printfs are compiled in, but also can be disabled by a run-time flag.
 * Note that the ##__VA_ARGS__ syntax works on GCC but might need tweaks for other compilers... */
#define OS_DEBUG_LEV(l, ...) OS_DebugPrintf(l, __func__, __LINE__, __VA_ARGS__);
#define OS_DEBUG(...)        OS_DEBUG_LEV(1, __VA_ARGS__)
#else
/* Debug printfs are not compiled in at all */
#define OS_DEBUG(...)
#endif

/*
 * An OSAL-specific check macro for NULL pointer.
 * Checked via BUGCHECK - considered a bug/fatal error if check fails.
 *
 * Returns OS_INVALID_POINTER if pointer is NULL.
 */
#define OS_CHECK_POINTER(ptr) BUGCHECK((ptr) != NULL, OS_INVALID_POINTER)

/*
 * An OSAL-specific check macro for an input buffer size.
 * Checked via ARGCHECK - non-fatal if check fails.
 *
 * Returns OS_ERR_INVALID_SIZE if size is 0.
 *
 * Also returns OS_ERR_INVALID_SIZE if size is excessively large.
 * Currently (UINT32_MAX/2) is used as the upper limit, as some API calls
 * (e.g. read/write) return a size as an int32 type, and therefore the
 * operation cannot exceed the bounds of this type.
 */
#define OS_CHECK_SIZE(val) ARGCHECK((val) > 0 && (val) < (UINT32_MAX / 2), OS_ERR_INVALID_SIZE)

/*
 * An OSAL-specific check macro for arbitrary string argument validation.
 *
 * First confirms string is not null using OS_CHECK_POINTER, then checks the maximum
 * length of the string using LENGTHCHECK.
 */
#define OS_CHECK_STRING(str, maxlen, errcode) \
    do                                        \
    {                                         \
        OS_CHECK_POINTER(str);                \
        LENGTHCHECK(str, maxlen, errcode);    \
    } while (0)

/*
 * An OSAL-specific check macro for object name strings.
 *
 * Returns OS_ERR_NAME_TOO_LONG if length is exceeded.
 */
#define OS_CHECK_APINAME(str) OS_CHECK_STRING(str, OS_MAX_API_NAME, OS_ERR_NAME_TOO_LONG)

/*
 * An OSAL specific argument check macro for path names
 *
 * Returns OS_FS_ERR_PATH_TOO_LONG if length is exceeded.
 */
#define OS_CHECK_PATHNAME(str) OS_CHECK_STRING(str, OS_MAX_PATH_LEN, OS_FS_ERR_PATH_TOO_LONG)

#endif /* OS_SHARED_GLOBALDEFS_H */
