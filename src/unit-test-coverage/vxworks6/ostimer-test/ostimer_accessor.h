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
 * File: ostimer_accessor.h
 *
 * Purpose:
 *   Provide private accessors for getting/manipulating ostimer.c's static data.
 *
 * History:
 *   06/30/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSTIMER_ACCESSOR_H_
#define _OSTIMER_ACCESSOR_H_

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

/* --------------- Prototypes for non-exported functions -------------------- */
void OS_UsecToTimespec(uint32 usecs, struct timespec *time_spec);
void OS_TimespecToUsec(struct timespec time_spec, uint32 *usecs);

/* -------------- Delegation function for static functions ------------------ */
void Pvt_OS_TimerSignalHandler(int host_timer_id);

/* --------------------- Custom Accessors ----------------------------------- */
/* Note, the types that support these prototypes
 * are in ostimer_copiedstatictypes.h */

/* Setters & getters for pointers to the OS_timer_table and 
 * os_clock_accuracy. */
OS_timer_record_t* getOSTimeTablePtr(void);
void setOSTimeTablePtr(OS_timer_record_t* ptr);

uint32* getOSClockAccuracyPtr(void);
uint32 getOSClockAccuracy(void);
void setOSClockAccuracyPtr(uint32* ptr);
void setOSClockAccuracy(uint32 accuracy);

uint32 getMaxSecsInUSec(void);

/* An initialization function to set the pointers before any
* ostimer.c calls occur. */
void initStaticPointers(void);

#endif /* _OSTIMER_ACCESSOR_H_ */

