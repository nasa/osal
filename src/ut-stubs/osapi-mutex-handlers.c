/**
 * \file
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "osapi-mutex.h" /* OSAL public API for this subsystem */
#include "utstub-helpers.h"

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_MutSemCreate' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_MutSemCreate(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        *sem_id = UT_AllocStubObjId(OS_OBJECT_TYPE_OS_MUTEX);
    }
    else
    {
        *sem_id = UT_STUB_FAKE_OBJECT_ID;
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_MutSemDelete' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_MutSemDelete(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t);
    int32     status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS)
    {
        UT_DeleteStubObjId(OS_OBJECT_TYPE_OS_MUTEX, sem_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_MutSemGetIdByName' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_MutSemGetIdByName(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    osal_id_t *sem_id = UT_Hook_GetArgValueByName(Context, "sem_id", osal_id_t *);
    int32      status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_MutSemGetIdByName), sem_id, sizeof(*sem_id)) < sizeof(*sem_id))
    {
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_MUTEX, sem_id);
    }
}

/*
 * -----------------------------------------------------------------
 * Default handler implementation for 'OS_MutSemGetInfo' stub
 * -----------------------------------------------------------------
 */
void UT_DefaultHandler_OS_MutSemGetInfo(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_mut_sem_prop_t *mut_prop = UT_Hook_GetArgValueByName(Context, "mut_prop", OS_mut_sem_prop_t *);
    int32              status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS &&
        UT_Stub_CopyToLocal(UT_KEY(OS_MutSemGetInfo), mut_prop, sizeof(*mut_prop)) < sizeof(*mut_prop))
    {
        strncpy(mut_prop->name, "Name", sizeof(mut_prop->name) - 1);
        mut_prop->name[sizeof(mut_prop->name) - 1] = '\0';
        UT_ObjIdCompose(1, OS_OBJECT_TYPE_OS_TASK, &mut_prop->creator);
    }
}
