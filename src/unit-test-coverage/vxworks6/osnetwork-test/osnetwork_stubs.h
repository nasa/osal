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
 * File: osnetwork_stubs.h
 *
 * Purpose:
 *   Provide stubs for unit testing osnetwork.c
 *
 * History:
 *   06/23/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSNETWORK_STUBS_H_
#define _OSNETWORK_STUBS_H_

#include "uttools.h"
#include "vxworks6-coverage-stubs.h"


/* Define missing types */
typedef struct
{
    int32 Dir;
} __dirstream;

typedef enum
{
    OSNETWORK_GETHOSTNAME_INDEX,
    OSNETWORK_HOSTGETBYNAME_INDEX,
    OSNETWORK_MAX_INDEX
} Osnetwork_Index_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} Osnetwork_ReturnCodeTable_t;

typedef struct
{
  int     (*gethostname)(char *name, int);
  int     (*hostGetByName)(char *name);
} Osnetwork_HookTable_t;

extern const char testFileName[];


void Osnetwork_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Osnetwork_SetFunctionHook(uint32 Index, void *FunPtr);
void Osnetwork_Reset(void);

int32 getNSemTake(void);
int32 getNSemGive(void);
int32 getRewinddirCalls(void);

#endif /* _OSNETWORK_STUBS_H_ */
