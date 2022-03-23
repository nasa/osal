/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-select header
 */

#include "osapi-select.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectFdAdd()
 * ----------------------------------------------------
 */
int32 OS_SelectFdAdd(OS_FdSet *Set, osal_id_t objid)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectFdAdd, int32);

    UT_GenStub_AddParam(OS_SelectFdAdd, OS_FdSet *, Set);
    UT_GenStub_AddParam(OS_SelectFdAdd, osal_id_t, objid);

    UT_GenStub_Execute(OS_SelectFdAdd, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectFdAdd, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectFdClear()
 * ----------------------------------------------------
 */
int32 OS_SelectFdClear(OS_FdSet *Set, osal_id_t objid)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectFdClear, int32);

    UT_GenStub_AddParam(OS_SelectFdClear, OS_FdSet *, Set);
    UT_GenStub_AddParam(OS_SelectFdClear, osal_id_t, objid);

    UT_GenStub_Execute(OS_SelectFdClear, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectFdClear, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectFdIsSet()
 * ----------------------------------------------------
 */
bool OS_SelectFdIsSet(const OS_FdSet *Set, osal_id_t objid)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectFdIsSet, bool);

    UT_GenStub_AddParam(OS_SelectFdIsSet, const OS_FdSet *, Set);
    UT_GenStub_AddParam(OS_SelectFdIsSet, osal_id_t, objid);

    UT_GenStub_Execute(OS_SelectFdIsSet, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectFdIsSet, bool);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectFdZero()
 * ----------------------------------------------------
 */
int32 OS_SelectFdZero(OS_FdSet *Set)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectFdZero, int32);

    UT_GenStub_AddParam(OS_SelectFdZero, OS_FdSet *, Set);

    UT_GenStub_Execute(OS_SelectFdZero, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectFdZero, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectMultiple()
 * ----------------------------------------------------
 */
int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectMultiple, int32);

    UT_GenStub_AddParam(OS_SelectMultiple, OS_FdSet *, ReadSet);
    UT_GenStub_AddParam(OS_SelectMultiple, OS_FdSet *, WriteSet);
    UT_GenStub_AddParam(OS_SelectMultiple, int32, msecs);

    UT_GenStub_Execute(OS_SelectMultiple, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectMultiple, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectSingle()
 * ----------------------------------------------------
 */
int32 OS_SelectSingle(osal_id_t objid, uint32 *StateFlags, int32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectSingle, int32);

    UT_GenStub_AddParam(OS_SelectSingle, osal_id_t, objid);
    UT_GenStub_AddParam(OS_SelectSingle, uint32 *, StateFlags);
    UT_GenStub_AddParam(OS_SelectSingle, int32, msecs);

    UT_GenStub_Execute(OS_SelectSingle, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectSingle, int32);
}
