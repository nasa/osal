/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-common header
 */

#include "osapi-common.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_API_Init()
 * ----------------------------------------------------
 */
int32 OS_API_Init(void)
{
    UT_GenStub_SetupReturnBuffer(OS_API_Init, int32);

    UT_GenStub_Execute(OS_API_Init, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_API_Init, int32);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_API_Teardown()
 * ----------------------------------------------------
 */
void OS_API_Teardown(void)
{

    UT_GenStub_Execute(OS_API_Teardown, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ApplicationExit()
 * ----------------------------------------------------
 */
void OS_ApplicationExit(int32 Status)
{
    UT_GenStub_AddParam(OS_ApplicationExit, int32, Status);

    UT_GenStub_Execute(OS_ApplicationExit, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_ApplicationShutdown()
 * ----------------------------------------------------
 */
void OS_ApplicationShutdown(uint8 flag)
{
    UT_GenStub_AddParam(OS_ApplicationShutdown, uint8, flag);

    UT_GenStub_Execute(OS_ApplicationShutdown, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DeleteAllObjects()
 * ----------------------------------------------------
 */
void OS_DeleteAllObjects(void)
{

    UT_GenStub_Execute(OS_DeleteAllObjects, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_IdleLoop()
 * ----------------------------------------------------
 */
void OS_IdleLoop(void)
{

    UT_GenStub_Execute(OS_IdleLoop, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_RegisterEventHandler()
 * ----------------------------------------------------
 */
int32 OS_RegisterEventHandler(OS_EventHandler_t handler)
{
    UT_GenStub_SetupReturnBuffer(OS_RegisterEventHandler, int32);

    UT_GenStub_AddParam(OS_RegisterEventHandler, OS_EventHandler_t, handler);

    UT_GenStub_Execute(OS_RegisterEventHandler, Basic, NULL);

    return UT_GenStub_GetReturnValue(OS_RegisterEventHandler, int32);
}
