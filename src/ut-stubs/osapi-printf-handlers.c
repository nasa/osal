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

#include "osapi-printf.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_printf' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_printf(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context, va_list va)
{
    const char *string = UT_Hook_GetArgValueByName(Context, "string", const char *);
    size_t      length = strlen(string);
    char        str[128];
    va_list     va_debugcopy;
    int32       status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    va_copy(va_debugcopy, va);
    /* Output the message when in debug mode (uses a copy of the va list) */
    vsnprintf(str, sizeof(str), string, va_debugcopy);
    UtDebug("OS_printf: %s", str);
    va_end(va_debugcopy);

    if (status >= 0)
    {
        /*
         * Special treatment of a format string which is only "%s" ...
         * This is merely a way to avoid having to do full-blown printf processing
         * inside the UT stub (which would make it the full version, not a stub)
         */
        if (strcmp(string, "%s") == 0 || strcmp(string, "%s\n") == 0)
        {
            string = va_arg(va, const char *);
        }
        length = strlen(string);
        if (length > 0)
        {
            /*
             * prune any supplied newlines -
             * one will be explicitly added
             *
             * (this is to ensure a consistent separator in the output buffer)
             */
            while (length > 0 && string[length - 1] == '\n')
            {
                --length;
            }
            UT_Stub_CopyFromLocal(UT_KEY(OS_printf), string, length);
            UT_Stub_CopyFromLocal(UT_KEY(OS_printf), "\n", 1);
        }
    }
}
