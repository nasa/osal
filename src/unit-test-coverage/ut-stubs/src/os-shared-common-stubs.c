/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-common header
 */

#include "os-shared-common.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_NotifyEvent()
 * ----------------------------------------------------
 */
int32 OS_NotifyEvent(OS_Event_t event, osal_id_t object_id, void *data)
{
    UT_GenStub_SetupReturnBuffer(OS_NotifyEvent, int32);

    UT_GenStub_AddParam(OS_NotifyEvent, OS_Event_t, event);
    UT_GenStub_AddParam(OS_NotifyEvent, osal_id_t, object_id);
    UT_GenStub_AddParam(OS_NotifyEvent, void *, data);

    UT_GenStub_Execute(OS_NotifyEvent, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_NotifyEvent, int32);
}
