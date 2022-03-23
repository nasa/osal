/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-timebase header
 */

#include "os-shared-timebase.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseCreate_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseCreate_Impl, int32);

    UT_GenStub_AddParam(OS_TimeBaseCreate_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TimeBaseCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseDelete_Impl, int32);

    UT_GenStub_AddParam(OS_TimeBaseDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TimeBaseDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseGetInfo_Impl(const OS_object_token_t *token, OS_timebase_prop_t *timer_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_TimeBaseGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_TimeBaseGetInfo_Impl, OS_timebase_prop_t *, timer_prop);

    UT_GenStub_Execute(OS_TimeBaseGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseLock_Impl()
 * ----------------------------------------------------
 */
void OS_TimeBaseLock_Impl(const OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_TimeBaseLock_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TimeBaseLock_Impl, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseSet_Impl()
 * ----------------------------------------------------
 */
int32 OS_TimeBaseSet_Impl(const OS_object_token_t *token, uint32 start_time, uint32 interval_time)
{
    UT_GenStub_SetupReturnBuffer(OS_TimeBaseSet_Impl, int32);

    UT_GenStub_AddParam(OS_TimeBaseSet_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_TimeBaseSet_Impl, uint32, start_time);
    UT_GenStub_AddParam(OS_TimeBaseSet_Impl, uint32, interval_time);

    UT_GenStub_Execute(OS_TimeBaseSet_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_TimeBaseSet_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBaseUnlock_Impl()
 * ----------------------------------------------------
 */
void OS_TimeBaseUnlock_Impl(const OS_object_token_t *token)
{
    UT_GenStub_AddParam(OS_TimeBaseUnlock_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_TimeBaseUnlock_Impl, Basic, NULL);
}
