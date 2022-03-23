/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-queue header
 */

#include "os-shared-queue.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueCreate_Impl()
 * ----------------------------------------------------
 */
int32 OS_QueueCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueCreate_Impl, int32);

    UT_GenStub_AddParam(OS_QueueCreate_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_QueueCreate_Impl, uint32, flags);

    UT_GenStub_Execute(OS_QueueCreate_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_QueueCreate_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueDelete_Impl()
 * ----------------------------------------------------
 */
int32 OS_QueueDelete_Impl(const OS_object_token_t *token)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueDelete_Impl, int32);

    UT_GenStub_AddParam(OS_QueueDelete_Impl, const OS_object_token_t *, token);

    UT_GenStub_Execute(OS_QueueDelete_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_QueueDelete_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueGetInfo_Impl()
 * ----------------------------------------------------
 */
int32 OS_QueueGetInfo_Impl(const OS_object_token_t *token, OS_queue_prop_t *queue_prop)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueGetInfo_Impl, int32);

    UT_GenStub_AddParam(OS_QueueGetInfo_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_QueueGetInfo_Impl, OS_queue_prop_t *, queue_prop);

    UT_GenStub_Execute(OS_QueueGetInfo_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_QueueGetInfo_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueueGet_Impl()
 * ----------------------------------------------------
 */
int32 OS_QueueGet_Impl(const OS_object_token_t *token, void *data, size_t size, size_t *size_copied, int32 timeout)
{
    UT_GenStub_SetupReturnBuffer(OS_QueueGet_Impl, int32);

    UT_GenStub_AddParam(OS_QueueGet_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_QueueGet_Impl, void *, data);
    UT_GenStub_AddParam(OS_QueueGet_Impl, size_t, size);
    UT_GenStub_AddParam(OS_QueueGet_Impl, size_t *, size_copied);
    UT_GenStub_AddParam(OS_QueueGet_Impl, int32, timeout);

    UT_GenStub_Execute(OS_QueueGet_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_QueueGet_Impl, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_QueuePut_Impl()
 * ----------------------------------------------------
 */
int32 OS_QueuePut_Impl(const OS_object_token_t *token, const void *data, size_t size, uint32 flags)
{
    UT_GenStub_SetupReturnBuffer(OS_QueuePut_Impl, int32);

    UT_GenStub_AddParam(OS_QueuePut_Impl, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_QueuePut_Impl, const void *, data);
    UT_GenStub_AddParam(OS_QueuePut_Impl, size_t, size);
    UT_GenStub_AddParam(OS_QueuePut_Impl, uint32, flags);

    UT_GenStub_Execute(OS_QueuePut_Impl, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_QueuePut_Impl, int32);
}
