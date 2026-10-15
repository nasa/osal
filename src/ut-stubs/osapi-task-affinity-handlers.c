/*
 * File: osapi-task-affinity-handlers.c
 * Purpose: Custom UT handlers for OSAL task affinity functions
 */

#include "osapi.h"
#include "utstubs.h"
#include "utgenstub.h"

/*
 * Custom handler for OS_TaskAffinityGetAffinity
 * (Allows the unit tests to pass back mock cpuset arrays)
 */
void UT_DefaultHandler_OS_TaskAffinityGetAffinity(void *UserObj, UT_EntryKey_t FuncKey, const UT_StubContext_t *Context)
{
    OS_cpuset_t *AffinityMask = UT_Hook_GetArgValueByName(Context, "cpuset", OS_cpuset_t *);
    int32        status;

    UT_Stub_GetInt32StatusCode(Context, &status);

    if (status == OS_SUCCESS && AffinityMask != NULL)
    {
        /* Safely copy the mock data provided by UT_SetDataBuffer in the test into the pointer */
        UT_Stub_CopyToLocal(FuncKey, AffinityMask, sizeof(*AffinityMask));
    }
}
