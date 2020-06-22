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
 * File: osloader_stubs.h
 *
 * Purpose:
 *   Provide stubs for unit testing osnetwork.c
 *
 * History:
 *   06/23/2015  Allen Brown, Odyssey Space Research, LLC
 *    * Created
 */

#ifndef _OSLOADER_STUBS_H_
#define _OSLOADER_STUBS_H_

#include "uttools.h"
#include "vxworks6-coverage-stubs.h"

/* Define missing types */

typedef enum
{
    OSLOADER_SEMMCREATE_INDEX,
    OSLOADER_OS_TANSLATE_PATH_INDEX,
    OSLOADER_SYMFINDBYNAME_INDEX,
    OSLOADER_SYMEACH_INDEX,
    OSLOADER_MODULEINFOGET_INDEX,
    OSLOADER_LOADMODULE_INDEX,
    OSLOADER_UNLDBYMODULEID_INDEX,
    OSLOADER_WRITE_INDEX,
    OSLOADER_OPEN_INDEX,
    OSLOADER_CLOSE_INDEX,
    OSLOADER_MAX_INDEX
} Osloader_Index_t;

typedef struct
{
    int32   Value;
    uint32  Count;
} Osloader_ReturnCodeTable_t;

typedef struct
{
  VCS_SEM_ID    (*semMCreate)(int);
  int32     (*OS_TranslatePath)(const char *, char *);
  VCS_STATUS    (*symFindByName)(VCS_SYMTAB_ID, char *, char **,
          VCS_SYM_TYPE *);
  VCS_SYMBOL*   (*symEach)(VCS_SYMTAB_ID, VCS_FUNCPTR, int);
  VCS_STATUS    (*moduleInfoGet)(VCS_MODULE_ID, VCS_MODULE_INFO *);
  VCS_MODULE_ID (*loadModule)(int, int);
  VCS_STATUS    (*unldByModuleId)(VCS_MODULE_ID, int);
  ssize_t   (*write) (int, const void *, size_t);
  int       (*open) (const char *, int, ...);
  VCS_STATUS    (*close) (int);
} Osloader_HookTable_t;

extern const char testFileName[];


void Osloader_SetReturnCode(uint32 Index, int32 RtnVal, uint32 CallCnt);
void Osloader_SetFunctionHook(uint32 Index, void *FunPtr);
void Osloader_Reset(void);

int32 OS_TranslatePath(const char *VirtualPath, char *LocalPath);

int32 getNSemTake(void);
int32 getNSemGive(void);

boolean isSemCreated(void);

/* For other stub functions, see VxWorks.h */

#endif /* _OSLOADER_STUBS_H_ */
