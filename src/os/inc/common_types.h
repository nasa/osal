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
 *  Purpose:
 *	    Unit specification for common types.
 *
 *  Design Notes:
 *         Assumes make file has defined processor family
 */

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
** Includes
*/

/*
** Macro Definitions
*/

/*
** Condition = TRUE is ok, Condition = FALSE is error
*/
#define CompileTimeAssert(Condition, Message) typedef char Message[(Condition) ? 1 : -1]

/*
** Define compiler specific macros
** The __extension__ compiler pragma is required
** for the uint64 type using GCC with the ANSI C90 standard.
** Other macros can go in here as needed.
**
** NOTE: The white-box (coverage) unit testing may need to disable
** these extra attributes.  These test builds define the OSAPI_NO_SPECIAL_ATTRIBS
** macro to disable this.
*/
#if defined(__GNUC__) && !defined(OSAPI_NO_SPECIAL_ATTRIBS)
#define _EXTENSION_     __extension__
#define OS_USED         __attribute__((used))
#define OS_PRINTF(n, m) __attribute__((format(printf, n, m)))
#else
#define _EXTENSION_
#define OS_USED
#define OS_PRINTF(n, m)
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

    /*
     * NOTE - NOT DEFINING STRUCT_LOW_BIT_FIRST or STRUCT_HIGH_BIT_FIRST
     * We should not make assumptions about the bit order here
     */

    typedef int8_t    int8;
    typedef int16_t   int16;
    typedef int32_t   int32;
    typedef int64_t   int64;
    typedef uint8_t   uint8;
    typedef uint16_t  uint16;
    typedef uint32_t  uint32;
    typedef uint64_t  uint64;
    typedef intptr_t  intptr;
    typedef uintptr_t cpuaddr;
    typedef size_t    cpusize;
    typedef ptrdiff_t cpudiff;

#ifdef OSAL_OMIT_DEPRECATED
    /**
     * A type to be used for OSAL resource identifiers.
     * This is a type-safe ID, and cannot be implicitly converted to an integer.
     * Use the provided inline functions in osapi-idmap.h to interpret ID values.
     */
    typedef struct
    {
        uint32_t v;
    } osal_id_t;
#else

/**
 * A type to be used for OSAL resource identifiers.
 * This typedef is backward compatible with the IDs from older versions of OSAL
 */
typedef uint32 osal_id_t;
#endif

    /**
     * A type used to represent a number of blocks or buffers
     *
     * This is used with file system and queue implementations.
     */
    typedef size_t osal_blockcount_t;

    /**
     * A type used to represent an index into a table structure
     *
     * This is used when referring directly to a table index as
     * opposed to an object ID.  It is primarily intended for
     * internal use, but is also output from public APIs such as
     * OS_ObjectIdToArrayIndex().
     */
    typedef uint32 osal_index_t;

    /**
     * A type used to represent the runtime type or category of an OSAL object
     */
    typedef uint32 osal_objtype_t;

    /**
     * The preferred type to represent OSAL status codes defined in osapi-error.h
     */
    typedef int32 osal_status_t;

    /**
     * @brief General purpose OSAL callback function
     *
     * This may be used by multiple APIS
     */
    typedef void (*OS_ArgCallback_t)(osal_id_t object_id, void *arg);

    /*
    ** Check Sizes
    */
    CompileTimeAssert(sizeof(uint8) == 1, TypeUint8WrongSize);
    CompileTimeAssert(sizeof(uint16) == 2, TypeUint16WrongSize);
    CompileTimeAssert(sizeof(uint32) == 4, TypeUint32WrongSize);
    CompileTimeAssert(sizeof(uint64) == 8, TypeUint64WrongSize);
    CompileTimeAssert(sizeof(int8) == 1, Typeint8WrongSize);
    CompileTimeAssert(sizeof(int16) == 2, Typeint16WrongSize);
    CompileTimeAssert(sizeof(int32) == 4, Typeint32WrongSize);
    CompileTimeAssert(sizeof(int64) == 8, Typeint64WrongSize);
    CompileTimeAssert(sizeof(cpuaddr) >= sizeof(void *), TypePtrWrongSize);

#ifdef __cplusplus
}
#endif

/*
 * Type macros for literals
 *
 * These macros enforce that a literal or other value is
 * interpreted as the intended type.  Although implicit
 * conversions between these types are often possible, using
 * this makes it explicit in the code where a type conversion
 * is expected.
 */
#define OSAL_SIZE_C(X)       ((size_t)(X))
#define OSAL_BLOCKCOUNT_C(X) ((osal_blockcount_t)(X))
#define OSAL_INDEX_C(X)      ((osal_index_t)(X))
#define OSAL_OBJTYPE_C(X)    ((osal_objtype_t)(X))
#define OSAL_STATUS_C(X)     ((osal_status_t)(X))

#endif /* COMMON_TYPES_H */
