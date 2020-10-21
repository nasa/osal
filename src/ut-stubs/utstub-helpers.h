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

/**
 * \file utstub-helpers.h
 *
 *  Created on: May 24, 2018
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Internal header file for OSAL UT stub functions
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#ifndef __OSAPI_UTSTUB_INTERNAL_H_
#define __OSAPI_UTSTUB_INTERNAL_H_

/*
 * Commonly used C library headers
 */
#include <string.h>

/*
 * Include all relevant OSAPI (public) and UT-assert headers
 */
#include "osapi.h"
#include "utstubs.h"
#include "utbsp.h"
#include "utassert.h"
#include "uttools.h"

typedef enum
{
    UT_OBJTYPE_NONE = 0,
    UT_OBJTYPE_TASK,
    UT_OBJTYPE_QUEUE,
    UT_OBJTYPE_COUNTSEM,
    UT_OBJTYPE_BINSEM,
    UT_OBJTYPE_MUTEX,
    UT_OBJTYPE_TIMECB,
    UT_OBJTYPE_MODULE,
    UT_OBJTYPE_FILESTREAM,
    UT_OBJTYPE_SOCKET,
    UT_OBJTYPE_TIMEBASE,
    UT_OBJTYPE_DIR,
    UT_OBJTYPE_FILESYS,
    UT_OBJTYPE_MAX
} UT_ObjType_t;

/*
 * A constant to use in stubs where no other value is applicable
 */
#define UT_STUB_FAKE_OBJECT_ID ((osal_id_t) {0xDEADBEEFU})

/*
 * Size of the bitmask for the OSAL fake object ID validity table
 * Set this according to the highest anticipated number of OSAL objects
 * simultaneously created by any given test case (it is reset when the
 * test is reset).
 *
 * Keep as a power of two, and actual number of bits is 8x this value
 * (Default value of 16 allows for up to 128 objects to be created in
 * a single test case, far more than anything I've seen yet)
 */
#define OSAL_MAX_VALID_PER_TYPE 16

typedef struct
{
    uint32 LastIssueNumber;
    uint8  ValidBits[OSAL_MAX_VALID_PER_TYPE];
} UT_ObjTypeState_t;

/**
 * Table lookup for the maximum number of OSAL objects by object type
 */
extern const uint32 UT_MAXOBJS[];

/*
 * Helper function - "allocate" a fake object ID of the given type
 */
osal_id_t UT_AllocStubObjId(UT_ObjType_t ObjType);

/*
 * Helper function - "deallocate" a fake object ID of the given type
 */
void UT_DeleteStubObjId(UT_ObjType_t ObjType, osal_id_t ObjId);

/*
 * Helper function - Report any queue objects found open
 * (for compatibility with CFE tests, only checks queues)
 */
void UT_CheckForOpenSockets(void);

/*
 * Helper function - Clear all OSAL UT stub objects
 * Resets the stub object table back to its initial/empty state
 */
void UT_ClearAllStubObjects(void);

/*
 * Compose/Decompose a unit test object ID from an index and type.
 * This is the UT-specific version not related to the OSAL runtime version.
 */
void UT_ObjIdCompose(uint32 indx, UT_ObjType_t objtype, osal_id_t *id);
void UT_ObjIdDecompose(osal_id_t id, uint32 *indx, UT_ObjType_t *objtype);

#endif
