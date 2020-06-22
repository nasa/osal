/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * File: ostimer_copiedstatictypes.h
 *
 * Purpose:
 *   Provides a copy of the types from ostimer.c to support
 *   use of private accessors.
 *
 * History:
 *   06/30/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSTIMER_COPIEDSTATICTYPES_H_
#define _OSTIMER_COPIEDSTATICTYPES_H_

/*
 * File: ostimer_accessor.h
 *
 * Purpose:
 *   Provide accesssor functions for unit testing ostimer.c
 *
 * History:
 *   06/30/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#include "uttools.h"
#include "vxworks6-coverage-stubs.h"
#include "common_types.h"
#include "osapi.h"

/* This type is copied directly from ostimer.c
 * so that the tests can access them.  We put
 * it here in a separate header so we don't get
 * multiply-defined types in the same compilation
 * unit as the ostimer.c. */
typedef struct
{
   uint32              free;
   char                name[OS_MAX_API_NAME];
   uint32              creator;
   uint32              start_time;
   uint32              interval_time;
   uint32              accuracy;
   OS_TimerCallback_t  callback_ptr;
   uint32              host_timerid;

} OS_timer_record_t;

#endif /* _OSTIMER_COPIEDSTATICTYPES_H_ */

