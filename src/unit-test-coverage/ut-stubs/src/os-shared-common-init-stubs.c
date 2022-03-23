/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-common header
 */

#include "os-shared-common.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_API_Impl_Init()
 * ----------------------------------------------------
 */
int32 OS_API_Impl_Init(osal_objtype_t idtype)
{
    UT_GenStub_SetupReturnBuffer(OS_API_Impl_Init, int32);

    UT_GenStub_AddParam(OS_API_Impl_Init, osal_objtype_t, idtype);

    UT_GenStub_Execute(OS_API_Impl_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_API_Impl_Init, int32);
}
