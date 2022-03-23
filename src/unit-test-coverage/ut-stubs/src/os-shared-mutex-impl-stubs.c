/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-mutex header
 */

#include "os-shared-mutex.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemCreate_Impl(const OS_object_token_t *token, uint32 options)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemCreate_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemCreate_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_MutSemCreate_Impl, uint32, options);

    UT_GenStub_Execute(OS_MutSemCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemDelete_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_MutSemDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemGetInfo_Impl(const OS_object_token_t *token, OS_mut_sem_prop_t *mut_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_MutSemGetInfo_Impl, OS_mut_sem_prop_t *, mut_prop);

    UT_GenStub_Execute(OS_MutSemGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemGive_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemGive_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemGive_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemGive_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_MutSemGive_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemGive_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_MutSemTake_Impl()
 * ----------------------------------------------------
 */
int32 OS_MutSemTake_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_MutSemTake_Impl, int32);

    UT_GenStub_AddParam(OS_MutSemTake_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_MutSemTake_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_MutSemTake_Impl, int32);
}
