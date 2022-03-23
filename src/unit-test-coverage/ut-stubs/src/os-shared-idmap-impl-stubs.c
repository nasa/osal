/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-idmap header
 */

#include "os-shared-idmap.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_Lock_Global_Impl()
 * ----------------------------------------------------
 */
void OS_Lock_Global_Impl(osal_objtype_t idtype)
{
    UT_GenStub_AddParam(OS_Lock_Global_Impl, osal_objtype_t, idtype);

    UT_GenStub_Execute(OS_Lock_Global_Impl, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_Unlock_Global_Impl()
 * ----------------------------------------------------
 */
void OS_Unlock_Global_Impl(osal_objtype_t idtype)
{
    UT_GenStub_AddParam(OS_Unlock_Global_Impl, osal_objtype_t, idtype);

    UT_GenStub_Execute(OS_Unlock_Global_Impl, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_WaitForStateChange_Impl()
 * ----------------------------------------------------
 */
void OS_WaitForStateChange_Impl(osal_objtype_t objtype, uint32 attempts)
{
    UT_GenStub_AddParam(OS_WaitForStateChange_Impl, osal_objtype_t, objtype);
    UT_GenStub_AddParam(OS_WaitForStateChange_Impl, uint32, attempts);

    UT_GenStub_Execute(OS_WaitForStateChange_Impl, Basic, NULL);
}
