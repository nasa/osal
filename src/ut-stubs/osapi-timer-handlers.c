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

#include "osapi-timer.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerAdd' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerAdd(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *timer_id = UT_Hook_GetArgValueByName(Context, "timer_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_TIMECB);
    }
    else
    {
        *timer_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *timer_id = UT_Hook_GetArgValueByName(Context, "timer_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *timer_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_TIMECB);
    }
    else
    {
        *timer_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t timer_id = UT_Hook_GetArgValueByName(Context, "timer_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_TIMECB, timer_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *timer_id = UT_Hook_GetArgValueByName(Context, "timer_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimerGetIdByName), timer_id, sizeof(*timer_id)) < sizeof(*timer_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TIMECB, timer_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_TimerGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_TimerGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_timer_prop_t *timer_prop = UT_Hook_GetArgValueByName(Context, "timer_prop", OS_timer_prop_t *);
    int32            status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_TimerGetInfo), timer_prop, sizeof(*timer_prop)) < sizeof(*timer_prop))
    {
        memset(timer_prop, 0, sizeof(*timer_prop));
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &timer_prop->creator);
    }
}
