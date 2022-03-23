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

#include "osapi-version.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GetVersionString' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GetVersionString(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    static const char DEFAULT[] = "UT";
    void *            Buffer;
    const char *      RetVal;

    UT_GetDataBuffer(UT_KEY(OS_GetVersionString), &Buffer, NULL, NULL);
    if (Buffer == NULL)
    {
        RetVal = DEFAULT;
    }
    else
    {
        RetVal = Buffer;
    }

    UT_Stub_SetReturnValue(FuncKey, RetVal);
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_GetVersionCodeName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_GetVersionCodeName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    static const char DEFAULT[] = "UT";
    void *            Buffer;
    const char *      RetVal;

    UT_GetDataBuffer(UT_KEY(OS_GetVersionCodeName), &Buffer, NULL, NULL);
    if (Buffer == NULL)
    {
        RetVal = DEFAULT;
    }
    else
    {
        RetVal = Buffer;
    }

    UT_Stub_SetReturnValue(FuncKey, RetVal);
}
