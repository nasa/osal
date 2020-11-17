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
 *  Filename: common_types.h
 *
 *  Purpose:
 *	    Unit specification for common types.
 *
 *  Design Notes:
 *         Assumes make file has defined processor family
 */

#ifndef _common_types_
#define _common_types_

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
** Other macros can go in here as needed, for example alignment
** pragmas.
**
** NOTE: The white-box (coverage) unit testing may need to disable
** these extra attributes.  These test builds define the OSAPI_NO_SPECIAL_ATTRIBS
** macro to disable this.
*/
#if defined(__GNUC__) && !defined(OSAPI_NO_SPECIAL_ATTRIBS)
#define _EXTENSION_     __extension__
#define OS_PACK         __attribute__((packed))
#define OS_ALIGN(n)     __attribute__((aligned(n)))
#define OS_USED         __attribute__((used))
#define OS_PRINTF(n, m) __attribute__((format(printf, n, m)))
#else
#define _EXTENSION_
#define OS_PACK
#define OS_ALIGN(n)
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

    /**
     * A type to be used for OSAL resource identifiers.
     */
    typedef uint32_t osal_id_t;

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

#ifndef NULL /* pointer to nothing */
#define NULL ((void *)0)
#endif

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

/*
 * TEMPORARY COMPATIBILITY MACRO
 *
 * Any code that depends on this macro should be fixed so as to not need it.
 * The value for this had been set by the BSP makefiles but this is not reliable,
 * especially on processors that support both big- and little- endian modes e.g.
 * ARM and MIPS.
 *
 * This is deprecated and only here to bridge the gap until code that depends
 * on this can be fixed.  Do not write any new code that uses this macro.
 *
 * If using an older makefile that defines one of the BIT_ORDER macros already,
 * then this entire section is skipped and the macro is used as-is.
 */
#if !defined(SOFTWARE_BIG_BIT_ORDER) && !defined(SOFTWARE_LITTLE_BIT_ORDER)

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || defined(__BIG_ENDIAN__) || defined(__ARMEB__) || \
    defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
/* It is a big-endian target architecture */
#define SOFTWARE_BIG_BIT_ORDER
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || \
    defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
/* It is a little-endian target architecture */
#define SOFTWARE_LITTLE_BIT_ORDER
#endif

#endif /* !defined(SOFTWARE_BIG_BIT_ORDER) && !defined(SOFTWARE_LITTLE_BIT_ORDER) */

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

#endif /* _common_types_ */
