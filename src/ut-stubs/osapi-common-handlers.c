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
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-common.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_strnlen' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_strnlen(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    const char *s;
    const char *end;
    size_t      maxlen;
    size_t      retval;
    int32       status;

    if (UT_Stub_GetInt32StatusCode(Context, &status))
    {
        /* If a retval was supplied in the test case, then use it */
        retval = status;
    }
    else
    {
        s      = UT_Hook_GetArgValueByName(Context, "s", const char *);
        maxlen = UT_Hook_GetArgValueByName(Context, "maxlen", size_t);

        /* This is basically the real impl of strnlen, as it
         * usually needs to give back the appropriate value in
         * order to follow the expected path */
        end = memchr(s, 0, maxlen);
        if (end == NULL)
        {
            retval = maxlen;
        }
        else
        {
            retval = end - s;
        }
    }

    UT_Stub_SetReturnValue(FuncKey, retval);
}
