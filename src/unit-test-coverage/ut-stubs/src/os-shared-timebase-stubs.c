/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-timebase header
 */

#include "os-shared-timebase.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_Milli2Ticks()
 * ----------------------------------------------------
 */
int32 OS_Milli2Ticks(uint32 milli_seconds, int *ticks)
{
    UT_GenStub_SetupReturnBuffer(OS_Milli2Ticks, int32);

    UT_GenStub_AddParam(OS_Milli2Ticks, uint32, milli_seconds);
    UT_GenStub_AddParam(OS_Milli2Ticks, int *, ticks);

    UT_GenStub_Execute(OS_Milli2Ticks, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_Milli2Ticks, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_TimeBase_CallbackThread()
 * ----------------------------------------------------
 */
void OS_TimeBase_CallbackThread(osal_id_t timebase_id)
{
    UT_GenStub_AddParam(OS_TimeBase_CallbackThread, osal_id_t, timebase_id);

    UT_GenStub_Execute(OS_TimeBase_CallbackThread, Basic, NULL);
}
