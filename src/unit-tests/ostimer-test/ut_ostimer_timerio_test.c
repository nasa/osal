/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/*================================================================================*
** File:  ut_ostimer_timerio_test.c
** Owner: Tam Ngo
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_ostimer_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

uint32    g_clkAccuracy = 0;
osal_id_t g_timerIds[UT_OS_TIMER_LIST_LEN];

typedef struct
{
    bool            IsTested;
    OS_timer_prop_t Prop;
} UT_reconf_status_t;

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/
void UT_os_othertimercallback1(osal_id_t timerId) {}
void UT_os_othertimercallback2(osal_id_t timerId, void *arg) {}

void UT_os_reconftimercallback(osal_id_t timerId, void *arg)
{
    UT_reconf_status_t *reconf = arg;

    if (!reconf->IsTested)
    {
        /*
         * Calls to timer configuration from the context of a timer function
         * should be rejected with OS_ERR_INCORRECT_OBJ_STATE.  However because
         * UtAssert is not fully thread-safe, this does not assert here, it just
         * calls the various functions on the first time through and stores the
         * result, which is checked/asserted in the main task.
         */
        UtAssert_INT32_EQ(OS_TimerCreate(&timerId, "reconf", &g_clkAccuracy, UT_os_othertimercallback1),
                          OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimerAdd(&timerId, "reconf", g_timerIds[1], UT_os_othertimercallback2, NULL),
                          OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimerDelete(timerId), OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimerSet(timerId, 100, 100), OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimerGetIdByName(&timerId, g_timerNames[7]), OS_ERR_INCORRECT_OBJ_STATE);
        UtAssert_INT32_EQ(OS_TimerGetInfo(timerId, &reconf->Prop), OS_ERR_INCORRECT_OBJ_STATE);

        reconf->IsTested = true;
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerCreate(uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t
*callback_ptr)
** Purpose: Creates a new timer and associates it with a callback routine
** Parameters: *timer_id - a pointer that will hold the timer id
**             *timer_name - a pointer that holds the name of the timer to be created
**             *clock_accuracy - a pointer that will hold the clock accuracy in microseconds
**             callback_ptr - a function pointer to the timer's callback
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if all of the timers has already been allocated
**          OS_TIMER_ERR_INVALID_ARGS if the callback pointer is null
**          OS_TIMER_ERR_UNAVAILABLE if the timer cannot be created
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      NOTE: If OS_TimerCreate() is implemented, then
**            OS_TimerDelete() and OS_TimerGetIdByName() should be implemented
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null pointer as one of the arguments (exclude callback ptr)
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Name-too-long-argument condition
**   1) Call this routine with a timer name of length greater than OS_MAX_API_NAME as argument
**   2) Expect the returned value to be
**        (a) OS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #3: Name equal to OS_MAX_API_NAME characters test
**   1) Call this routine with a timer name equal to OS_MAX_API_NAME + 1 as argument
**   2) Expect the returned value to be
**        (a) OS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #4: Name-taken-argument condition
**   1) Call this routine with a valid timer name as argument
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine the second time with the timer name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_ERR_NAME_TAKEN
** -----------------------------------------------------
** Test #5: No-free-ids condition
**   1) Call this routine N number of times, where N = OS_MAX_TIMEBASES+1
**   2) Expect the returned value of the last call to be
**        (a) OS_ERR_NO_FREE_IDS
** -----------------------------------------------------
** Test #6: Invalid-argument condition
**   1) Call this routine with a null pointer for callback as argument
**   2) Expect the returned value to be
**        (a) OS_TIMER_ERR_INVALID_ARGS
** -----------------------------------------------------
** Test #7: Timer-unavailable condition
**   1) Set up test to cause the OS call inside this routine to fail
**   2) Call this routine with valid arguments
**   3) Expect the returned value to be
**        (a) OS_TIMER_ERR_UNAVAILABLE
** -----------------------------------------------------
** Test #8: Nominal condition
**   1) Call this routine
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call OS_TimerGetIdByName with the timer name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS __and__
**        (b) the returned timer id is the same as the timer id returned in #1
**   5) Call OS_TimerDelete with the timer id returned in #1 as argument
**   6) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_timercreate_test()
{
    int32 i = 0, j = 0;
    char  tmpStr[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_TimerCreate(NULL, g_timerNames[1], &g_clkAccuracy, &UT_os_timercallback), OS_INVALID_POINTER);
    UT_RETVAL(OS_TimerCreate(&g_timerIds[1], NULL, &g_clkAccuracy, &UT_os_timercallback), OS_INVALID_POINTER);
    UT_RETVAL(OS_TimerCreate(&g_timerIds[1], g_timerNames[1], NULL, &UT_os_timercallback), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Name-too-long */

    UT_RETVAL(OS_TimerCreate(&g_timerIds[2], g_longTimerName, &g_clkAccuracy, &UT_os_timercallback),
              OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Name equal to OS_MAX_API_NAME characters test */

    /* Test Load library returning an error on a too long library name */
    memset(&tmpStr[0], 'a', OS_MAX_API_NAME);
    tmpStr[OS_MAX_API_NAME] = '\0';

    UT_RETVAL(OS_TimerCreate(&g_timerIds[2], tmpStr, &g_clkAccuracy, &UT_os_timercallback), OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #4 Name-taken */

    if (UT_SETUP(OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback)))
    {
        UT_RETVAL(OS_TimerCreate(&g_timerIds[4], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback),
                  OS_ERR_NAME_TAKEN);

        /* Reset test environment */
        UT_TEARDOWN(OS_TimerDelete(g_timerIds[3]));
    }

    /*-----------------------------------------------------*/
    /* #5 No-free-IDs */

    for (i = 0; i <= OS_MAX_TIMEBASES; i++)
    {
        memset(tmpStr, '\0', sizeof(tmpStr));
        UT_os_sprintf(tmpStr, "Timer #%d", (int)i);
        if (i == OS_MAX_TIMEBASES)
        {
            UT_RETVAL(OS_TimerCreate(&g_timerIds[i], tmpStr, &g_clkAccuracy, &UT_os_timercallback), OS_ERR_NO_FREE_IDS);
        }
        else if (!UT_SETUP(OS_TimerCreate(&g_timerIds[i], tmpStr, &g_clkAccuracy, &UT_os_timercallback)))
        {
            break;
        }
    }

    /* Reset test environment */
    for (j = 0; j < OS_MAX_TIMEBASES; j++)
    {
        UT_TEARDOWN(OS_TimerDelete(g_timerIds[j]));
    }

    /*-----------------------------------------------------*/
    /* #6 Invalid-arg */

    UT_RETVAL(OS_TimerCreate(&g_timerIds[5], g_timerNames[5], &g_clkAccuracy, NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #7 Timer-unavailable */

    /*-----------------------------------------------------*/
    /* #8 Nominal */

    UT_NOMINAL(OS_TimerCreate(&g_timerIds[7], g_timerNames[7], &g_clkAccuracy, &UT_os_timercallback));

    /* Reset test environment */
    UT_TEARDOWN(OS_TimerDelete(g_timerIds[7]));
}

/*--------------------------------------------------------------------------------*
** Test case to confirm that attempts to (re-)configure a timer from the context
** of a callback function should fail with OS_ERR_INCORRECT_OBJ_STATE
**--------------------------------------------------------------------------------*/
void UT_os_timerreconf_test()
{
    UT_reconf_status_t reconf;

    memset(&reconf, 0, sizeof(reconf));

    if (UT_SETUP(OS_TimeBaseCreate(&g_timerIds[1], "reconf", NULL)))
    {
        if (UT_SETUP(OS_TimeBaseSet(g_timerIds[1], 50, 50)))
        {
            if (UT_SETUP(OS_TimerAdd(&g_timerIds[2], "reconf", g_timerIds[1], UT_os_reconftimercallback, &reconf)))
            {
                if (UT_SETUP(OS_TimerSet(g_timerIds[2], 50, 50)))
                {
                    while (!reconf.IsTested)
                    {
                        OS_TaskDelay(1);
                    }
                }

                /* Reset test environment */
                UT_TEARDOWN(OS_TimerDelete(g_timerIds[2]));
            }
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_TimeBaseDelete(g_timerIds[1]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerDelete(uint32 timer_id)
** Purpose: Deletes a timer of a given timer id
** Parameters: timer_id - the id of the timer to be deleted
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid timer id
**          OS_TIMER_ERR_INTERNAL if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      Note: If OS_TimerDelete() is implemented, then
**            OS_TimerCreate() and OS_TimerGetIdByName() should be implemented.
** -----------------------------------------------------
** Test #1: Invalid-id-argument condition
**   1) Make sure there's no timer created previously
**   2) Call this routine with some value for timer id as argument
**   3) Expect the returned value to be
**       (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #2: Internal-error condition
**   1) Set up test to cause the OS call inside this routine to fail
**   2) Call this routine with valid arguments
**   3) Expect the returned value to be
**        (a) OS_TIMER_ERR_INTERNAL
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_TimerCreate
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with the timer id returned in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS
**   5) Call OS_TimerGetIdByName with the timer name used in #1 as argument
**   6) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
**   7) Call OS_TimerCreate exactly as in #1
**   8) Expect the returned value to be
**        (a) OS_SUCCESS
**--------------------------------------------------------------------------------*/
void UT_os_timerdelete_test()
{
    /*-----------------------------------------------------*/
    /* #1 Invalid-id-arg */

    UT_RETVAL(OS_TimerDelete(UT_OBJID_INCORRECT), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_TimerDelete(OS_OBJECT_ID_UNDEFINED), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Internal-error */

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback)))
    {
        UT_NOMINAL(OS_TimerDelete(g_timerIds[3]));
        UT_RETVAL(OS_TimerGetIdByName(&g_timerIds[4], g_timerNames[3]), OS_ERR_NAME_NOT_FOUND);
        UT_NOMINAL(OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback));

        /* Reset test environment */
        UT_TEARDOWN(OS_TimerDelete(g_timerIds[3]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerSet(uint32 timer_id, uint32 start_msec, uint32 interval_msec)
** Purpose: Programs a given timer with a start time of microseconds and an
**          interval time of microseconds
** Parameters: timer_id - id of the timer to be programmed
**             start_msec - start time in microseconds of when to first call the
**                          timer's callback function
**             interval_msec - interval in microseconds of what the periodic timer
**                             will be programmed for
** Returns: OS_ERR_INVALID_ID if the timer id passed in is not a valid timer id
**          OS_TIMER_ERR_INTERNAL if the OS call failed
**          OS_SUCCESS if succeeded
**          OS_ERR_NOT_IMPLEMENTED if not implemented
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      Note: If OS_TimerSet() is implemented, then
**            OS_TimerCreate(), OS_TimerDelete() and OS_TimerGetIdByName() should be implemented.
** -----------------------------------------------------
** Test #1: Invalid-id condition
**   1) Make sure there's no timer created previously
**   2) Call this routine with some value for timer id as argument
**   3) Expect the returned value to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #2: Internal-error condition
**   1) Setup test to cause the OS call inside this routine to fail
**   2) Call this routine with valid arguments
**   3) Expect the returned value to be
**        (a) OS_TIMER_ERR_INTERNAL
** -----------------------------------------------------
** Test #3: Interval-too-short condition
**   1) Call OS_TimerCreate
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with a start time of 1,000 and an interval time of 5 microsecs
**   4) When the timer callback registered in #1 gets call, determine if the duration
**      is the set interval time
**   5) Exit test when the timer callback registered in #1 gets call 10 times
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_TimerCreate
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with a start time of 1,000 and an interval time of 500,000 microsecs
**   4) When the timer callback registered in #1 gets call, determine if the duration
**      is within +/- 5% of the set interval time
**   5) Exit test when the timer callback registered in #1 gets call 10 times
**--------------------------------------------------------------------------------*/
void UT_os_timerset_test()
{
    uint32 startTime = 0, intervalTime = 0;

    /*-----------------------------------------------------*/
    /* #1 Invalid-id-arg */

    UT_RETVAL(OS_TimerSet(UT_OBJID_INCORRECT, 10000, 10000), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_TimerSet(OS_OBJECT_ID_UNDEFINED, 10000, 10000), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #2 Internal-error */

    /*-----------------------------------------------------*/
    /* #3 Interval-too-short */

    if (UT_SETUP(OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback)))
    {
        UT_RETVAL(OS_TimerSet(g_timerIds[3], 0, 0), OS_TIMER_ERR_INVALID_ARGS);

        g_status       = 0;
        g_timerId      = g_timerIds[3];
        g_timerFirst   = 1;
        g_cbLoopCntMax = 10;
        startTime      = 1000;
        intervalTime   = 5;
        g_toleranceVal = 0;

        UtPrintf("\nOS_TimerSet() - #3 Interval-too-short (clk_accuracy=%d)\n", (int)g_clkAccuracy);
        if (UT_NOMINAL(OS_TimerSet(g_timerIds[3], startTime, intervalTime)))
        {
            while (g_status == 0)
            {
                OS_TaskDelay(1);
            }

            UtAssert_True(g_status < 0, "4# Nominal - callback status %d", (int)g_status);
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_TimerDelete(g_timerIds[3]));
    }

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    if (UT_SETUP(OS_TimerCreate(&g_timerIds[4], g_timerNames[4], &g_clkAccuracy, &UT_os_timercallback)))
    {
        g_status       = 0;
        g_timerId      = g_timerIds[4];
        g_timerFirst   = 1;
        g_cbLoopCntMax = 10;
        startTime      = 1000;
        intervalTime   = 500000;
        g_toleranceVal = intervalTime / 20; /* 5% */

        UtPrintf("\nOS_TimerSet() - #4 Nominal condition (clk_accuracy=%d)\n", (int)g_clkAccuracy);
        if (UT_NOMINAL(OS_TimerSet(g_timerIds[4], startTime, intervalTime)))
        {
            while (g_status == 0)
            {
                OS_TaskDelay(1);
            }

            UtAssert_True(g_status > 0, "4# Nominal - callback status %d", (int)g_status);
        }

        /* Reset test environment */
        UT_TEARDOWN(OS_TimerDelete(g_timerIds[4]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerGetIdByName(uint32 *timer_id, const char *timer_name)
** Purpose: Returns the timer id of a given timer name
** Parameters: *timer_id - a pointer that will hold the timer id
**             *timer_name - a pointer that holds the timer name
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name passed in is not found in the timer table
**          OS_SUCCESS if succeeded
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      Note: If OS_TimerGetIdByName() is implemented, then
**            OS_TimerCreate() and OS_TimerDelete() should be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Name-too-long condition
**   1) Call this routine with a timer name of length greater than OS_MAX_API_NAME as argument
**   2) Expect the returned value to be
**        (a) OS_ERR_NAME_TOO_LONG
** -----------------------------------------------------
** Test #3: Name-not-found condition
**   1) Make sure there's no timer created previously
**   2) Call this routine with a valid timer name
**   3) Expect the returned value to be
**        (a) OS_ERR_NAME_NOT_FOUND
** -----------------------------------------------------
** Test #4: Nominal condition
**   1) Call OS_TimerCreate
**   2) Expected the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with the timer name used in #1 as argument
**   4) Expect the returned value to be
**        (a) OS_SUCCESS __and__
**        (b) the returned timer id is the same as the timer id returned in #1
**--------------------------------------------------------------------------------*/
void UT_os_timergetidbyname_test()
{
    /*-----------------------------------------------------*/
    /* #1 Null-pointer-arg */

    UT_RETVAL(OS_TimerGetIdByName(NULL, g_timerNames[1]), OS_INVALID_POINTER);
    UT_RETVAL(OS_TimerGetIdByName(&g_timerIds[1], NULL), OS_INVALID_POINTER);

    /*-----------------------------------------------------*/
    /* #2 Name-too-long */

    UT_RETVAL(OS_TimerGetIdByName(&g_timerIds[2], g_longTimerName), OS_ERR_NAME_TOO_LONG);

    /*-----------------------------------------------------*/
    /* #3 Name-not-found */

    UT_RETVAL(OS_TimerGetIdByName(&g_timerIds[3], g_timerNames[3]), OS_ERR_NAME_NOT_FOUND);

    /*-----------------------------------------------------*/
    /* #4 Nominal */

    if (UT_SETUP(OS_TimerCreate(&g_timerIds[4], g_timerNames[4], &g_clkAccuracy, &UT_os_timercallback)))
    {
        UT_NOMINAL(OS_TimerGetIdByName(&g_timerIds[5], g_timerNames[4]));

        UtAssert_True(OS_ObjectIdEqual(g_timerIds[4], g_timerIds[5]), "OS_TimerGetIdByName() ID (%lu) == %lu",
                      OS_ObjectIdToInteger(g_timerIds[4]), OS_ObjectIdToInteger(g_timerIds[5]));

        /* Reset test environment */
        UT_TEARDOWN(OS_TimerDelete(g_timerIds[4]));
    }
}

/*--------------------------------------------------------------------------------*
** Syntax: int32 OS_TimerGetInfo(uint32 timer_id, OS_timer_prop_t *timer_prop)
** Purpose: Returns timer information about a given timer
** Parameters: timer_id - the timer id to look up
**             *timer_prop - a pointer to an OS_timer_prop_t structure that will
**                           hold a timer's information
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid timer id
**          OS_SUCCESS if succeeded
** -----------------------------------------------------
** Test #0: Not-implemented condition
**   1) Call this routine
**   2) If the returned value is OS_ERR_NOT_IMPLEMENTED, then exit test
**   3) Otherwise, continue
**      Note: If OS_TimerGetInfo() is implemented, then
**            OS_TimerCreate() and OS_TimerDelete() should be implemented.
** -----------------------------------------------------
** Test #1: Null-pointer-argument condition
**   1) Call this routine with a null pointer as argument
**   2) Expect the returned value to be
**        (a) OS_INVALID_POINTER
** -----------------------------------------------------
** Test #2: Invalid-id-argument condition
**   1) Make sure there's no timer created previously
**   2) Call this routine with some value for timer id as argument
**   3) Expect the returned value to be
**        (a) OS_ERR_INVALID_ID
** -----------------------------------------------------
** Test #3: Nominal condition
**   1) Call OS_TimerCreate
**   2) Expect the returned value to be
**        (a) OS_SUCCESS
**   3) Call this routine with the timer id returned in #1
**   4) Expect the returned value to be
**       (a) OS_SUCCESS __and__
**       (b) timer name returned for timer properties is the same as timer name used in #1
**--------------------------------------------------------------------------------*/
void UT_os_timergetinfo_test()
{
    OS_timer_prop_t timerProps;

    /*-----------------------------------------------------*/
    /* Invalid-id-arg */

    UT_RETVAL(OS_TimerGetInfo(UT_OBJID_INCORRECT, &timerProps), OS_ERR_INVALID_ID);
    UT_RETVAL(OS_TimerGetInfo(OS_OBJECT_ID_UNDEFINED, &timerProps), OS_ERR_INVALID_ID);

    /*-----------------------------------------------------*/
    /* #3 Nominal */

    if (UT_SETUP(OS_TimerCreate(&g_timerIds[3], g_timerNames[3], &g_clkAccuracy, &UT_os_timercallback)))
    {
        /*-----------------------------------------------------*/
        /* Null-pointer-arg */

        UT_RETVAL(OS_TimerGetInfo(g_timerIds[3], NULL), OS_INVALID_POINTER);

        memset(&timerProps, 0x00, sizeof(timerProps));

        UT_NOMINAL(OS_TimerGetInfo(g_timerIds[3], &timerProps));

        UtAssert_StrCmp(timerProps.name, g_timerNames[3], "%s == %s", timerProps.name, g_timerNames[3]);

        /* Reset test environment */
        UT_TEARDOWN(OS_TimerDelete(g_timerIds[3]));
    }
}

/*================================================================================*
** End of File: ut_ostimer_timerio_test.c
**================================================================================*/
