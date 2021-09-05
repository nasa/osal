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
 * Owner: Tam Ngo
 * Date:  May 2013
 */

#ifndef UT_OS_SUPPORT_H
#define UT_OS_SUPPORT_H

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utassert.h"
#include "uttest.h"
#include "osapi.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*
 * Buffers to hold names of various objects
 *
 * These are sized somewhat larger than the osconfig.h specification,
 * so that test cases may create names that exceed the allowed length
 */
#define UT_OS_NAME_BUFF_SIZE       (OS_MAX_API_NAME + 10)
#define UT_OS_FILE_BUFF_SIZE       (OS_MAX_FILE_NAME + 10)
#define UT_OS_PATH_BUFF_SIZE       (OS_MAX_PATH_LEN + 10)
#define UT_OS_PHYS_NAME_BUFF_SIZE  (OS_FS_PHYS_NAME_LEN + 10)
#define UT_OS_LOCAL_PATH_BUFF_SIZE (OS_MAX_LOCAL_PATH_LEN + 10)

/*
 * Generic buffer for I/O operations
 */
#define UT_OS_IO_BUFF_SIZE 128

static inline bool UtOsalRetVal(int32 Fn, int32 Exp, bool NotImplAllowed, UtAssert_CaseType_t casetype,
                                const char *File, uint32 Line, const char *FnTxt, const char *ExpTxt)
{
    /* If "not implemented" is acceptable, override the casetype to be N/A */
    if (NotImplAllowed && (Fn == OS_ERR_NOT_IMPLEMENTED || Fn == OS_ERR_OPERATION_NOT_SUPPORTED))
    {
        casetype = UTASSERT_CASETYPE_NA;
    }

    return UtAssertEx(Fn == Exp, casetype, File, Line, "%s (%d) == %s (%d)", FnTxt, (int)Fn, ExpTxt, (int)Exp);
}

static inline bool UtOsalNotSuccess(int32 Fn, UtAssert_CaseType_t casetype, const char *File, uint32 Line,
                                    const char *FnTxt)
{
    /* Check result is negative to support APIs that return nonzero on success (e.g. read/write) */
    return UtAssertEx(Fn < 0, casetype, File, Line, "%s (%d) not successful", FnTxt, (int)Fn);
}

static inline bool UtManualInspectionWithStatus(int32 Fn, const char *File, uint32 Line, const char *FnTxt)
{
    UtAssert_MIR("%s value=%d", FnTxt, (int)Fn);
    return (Fn >= 0);
}

/* Only report errors */
static inline bool UtOsalCheck(int32 Fn, UtAssert_CaseType_t casetype, const char *File, uint32 Line, const char *FnTxt)
{
    /* Check non-negative to support APIs that return nonzero on success (e.g. read/write) */
    if (Fn >= 0)
    {
        return true;
    }

    return UtAssertEx(false, casetype, File, Line, "%s (%d) >= %s", FnTxt, (int)Fn, "OS_SUCCESS");
}

static inline bool UtOsalImplemented(int32 Fn, const char *File, uint32 Line)
{
    if (Fn == OS_ERR_NOT_IMPLEMENTED)
    {
        UtAssertEx(false, UTASSERT_CASETYPE_NA, File, Line, "API not implemented (%d)", (int)Fn);
        return false;
    }

    return true;
}

#define UT_RETVAL(Fn, Exp) UtOsalRetVal(Fn, Exp, false, UTASSERT_CASETYPE_FAILURE, __FILE__, __LINE__, #Fn, #Exp)
#define UT_NOMINAL(Fn) \
    UtOsalRetVal(Fn, OS_SUCCESS, false, UTASSERT_CASETYPE_FAILURE, __FILE__, __LINE__, #Fn, "OS_SUCCESS")
#define UT_NOMINAL_OR_NOTIMPL(Fn) \
    UtOsalRetVal(Fn, OS_SUCCESS, true, UTASSERT_CASETYPE_FAILURE, __FILE__, __LINE__, #Fn, "OS_SUCCESS")
#define UT_NOT_SUCCESS(Fn) UtOsalNotSuccess(Fn, UTASSERT_CASETYPE_FAILURE, __FILE__, __LINE__, #Fn)

#define UT_MIR_STATUS(Fn) UtManualInspectionWithStatus(Fn, __FILE__, __LINE__, #Fn)
#define UT_MIR_VOID(Fn)   Fn, UtAssert_MIR("%s", #Fn)

#define UT_SETUP(Fn)    UtOsalCheck(Fn, UTASSERT_CASETYPE_TSF, __FILE__, __LINE__, #Fn)
#define UT_TEARDOWN(Fn) UtOsalCheck(Fn, UTASSERT_CASETYPE_TTF, __FILE__, __LINE__, #Fn)
#define UT_IMPL(Fn)     UtOsalImplemented(Fn, __FILE__, __LINE__)

/*--------------------------------------------------------------------------------*/

#define UT_os_sprintf(buf, ...)                          \
    do                                                   \
    {                                                    \
        int x = snprintf(buf, sizeof(buf), __VA_ARGS__); \
        if (x > 0 && x < sizeof(buf))                    \
            buf[x] = 0;                                  \
    } while (0)

/*--------------------------------------------------------------------------------*/

/*
 * An osal_id_t value which is not OS_OBJECT_ID_UNDEFINED and also
 * not aliasing any other valid value/type.
 *
 * This is used to test for proper rejection of bad ID values.
 */
#define UT_OBJID_INCORRECT ((osal_id_t) {0xDEADBEEF})

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

#endif /* UT_OS_SUPPORT_H */
