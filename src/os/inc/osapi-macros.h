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

/**
 * \file
 *
 * Macro definitions that are used across all OSAL subsystems
 */

#ifndef OSAPI_MACROS_H
#define OSAPI_MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "osconfig.h"
#include "common_types.h"

#ifdef OSAL_CONFIG_BUGCHECK_DISABLE

/**
 * @brief Placeholder for BUGCHECK
 *
 * When OSAL_CONFIG_BUGCHECK_DISABLE is specified, then
 * the BUGCHECK/BUGREPORT macros become no-ops.
 */
#define BUGCHECK(...)

/**
 * @brief Placeholder for BUGREPORT
 *
 * When OSAL_CONFIG_BUGCHECK_DISABLE is specified, then
 * the BUGCHECK/BUGREPORT macros become no-ops.
 */
#define BUGREPORT(...)

#else /* Bug checking enabled */

#ifdef OSAL_CONFIG_BUGCHECK_STRICT

/*
 * This BUGREPORT implementation aborts the application so that the applicaiton
 * can be debugged immediately.  This prints the message direct to stderr, which is
 * typically not buffered, so it should appear on the console before the abort occurs,
 * but may appear out of order with respect to calls to OS_printf().
 */
#define BUGREPORT(...)                \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        abort();                      \
    } while (false)

#else

#include "osapi-printf.h"

/*
 * This BUGREPORT simply prints the message using OS_printf, which is buffered.  This
 * has a minimal realtime impact as it only copies to the buffer, and the print will
 * appear in order with respect to other calls to OS_printf() by the application.
 */
#define BUGREPORT(...) OS_printf(__VA_ARGS__)

#endif /* OSAL_CONFIG_BUGCHECK_STRICT */

/**
 * @brief Basic Bug-Checking macro
 *
 * This macro checks a conditional, and if it is FALSE, then it generates a report -
 * which may in turn contain additional actions.
 *
 * BUGCHECK should only be used for conditions which are critical and must always be true.
 * If such a condition is ever false then it indicates a bug in the application which
 * must be resolved.  It may or may not be possible to continue operation if a bugcheck
 * fails.
 *
 * @sa ARGCHECK for checking non-critical values
 */
#define BUGCHECK(cond, errcode)                                                                             \
    if (!(cond))                                                                                            \
    {                                                                                                       \
        BUGREPORT("\n**BUG** %s():%d:check \'%s\' FAILED --> %s\n\n", __func__, __LINE__, #cond, #errcode); \
        return errcode;                                                                                     \
    }

#endif /* OSAL_CONFIG_BUGCHECK_DISABLE */

/**
 * @brief Generic argument checking macro for non-critical values
 *
 * This macro checks a conditional that is expected to be true, and return a value
 * if it evaluates false.
 *
 * ARGCHECK can be used to check for out of range or other invalid argument conditions
 * which may (validly) occur at runtime and do not necessarily indicate bugs in the
 * application.
 *
 * These argument checks are NOT considered a fatal errors.  The application
 * continues to run normally.  This does not report the error on the console.
 *
 * As such, ARGCHECK actions are always compiled in - not selectable at compile-time.
 *
 * @sa BUGCHECK for checking critical values that indicate bugs
 */
#define ARGCHECK(cond, errcode) \
    if (!(cond))                \
    {                           \
        return errcode;         \
    }

/**
 * @brief String length limit check macro
 *
 * This macro is a specialized version of ARGCHECK that confirms a string will fit
 * into a buffer of the specified length, and return an error code if it will not.
 *
 * @note this uses ARGCHECK, thus treating a string too long as a normal runtime
 * (i.e. non-bug) error condition with a typical error return to the caller.
 */
#define LENGTHCHECK(str, len, errcode) ARGCHECK(memchr(str, '\0', len), errcode)

#endif /* OSAPI_MACROS_H */
