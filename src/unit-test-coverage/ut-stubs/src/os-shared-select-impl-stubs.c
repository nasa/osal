/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-select header
 */

#include "os-shared-select.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectMultiple_Impl()
 * ----------------------------------------------------
 */
int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectMultiple_Impl, int32);

    UT_GenStub_AddParam(OS_SelectMultiple_Impl, OS_FdSet *, ReadSet);
    UT_GenStub_AddParam(OS_SelectMultiple_Impl, OS_FdSet *, WriteSet);
    UT_GenStub_AddParam(OS_SelectMultiple_Impl, int32, msecs);

    UT_GenStub_Execute(OS_SelectMultiple_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectMultiple_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_SelectSingle_Impl()
 * ----------------------------------------------------
 */
int32 OS_SelectSingle_Impl(const OS_object_token_t *token, uint32 *SelectFlags, int32 msecs)
{
    UT_GenStub_SetupReturnBuffer(OS_SelectSingle_Impl, int32);

    UT_GenStub_AddParam(OS_SelectSingle_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_SelectSingle_Impl, uint32 *, SelectFlags);
    UT_GenStub_AddParam(OS_SelectSingle_Impl, int32, msecs);

    UT_GenStub_Execute(OS_SelectSingle_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_SelectSingle_Impl, int32);
}
