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

/**
 * \file
 *
 * Owner: Tam Ngo
 * Date:  May 2013
 */

#ifndef UT_OSTIMER_TEST_H
#define UT_OSTIMER_TEST_H

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_support.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_TIMER_LIST_LEN (OS_MAX_TIMERS + 10)

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

extern const char *g_timerNames[UT_OS_TIMER_LIST_LEN];
extern char        g_longTimerName[UT_OS_NAME_BUFF_SIZE];

extern uint32    g_cbLoopCntMax;
extern uint32    g_toleranceVal;
extern uint32    g_timerFirst;
extern int32     g_status;
extern osal_id_t g_timerId;

extern int32 TimerCreateRc;
extern int32 TimerDeleteRc;
extern int32 TimerSetRc;
extern int32 TimerGetByNameRc;
extern int32 TimerGetInfoRc;

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

void UT_os_timercallback(osal_id_t timerId);

void UT_os_timercreate_test(void);
void UT_os_timerdelete_test(void);
void UT_os_timerset_test(void);
void UT_os_timerreconf_test(void);
void UT_os_timergetidbyname_test(void);
void UT_os_timergetinfo_test(void);

/*--------------------------------------------------------------------------------*/

#endif /* UT_OSTIMER_TEST_H */
