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
 * \file osapi_stubs.c
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Stub implementations for the functions defined in the OSAL API
 *
 * The stub implementation can be used for unit testing applications built
 * on top of OSAL.  The stubs do not do any real function, but allow
 * the return code to be crafted such that error paths in the application
 * can be executed.
 */

#include "utstub-helpers.h"

const uint32 UT_MAXOBJS[UT_OBJTYPE_MAX] = {[UT_OBJTYPE_TASK]       = OS_MAX_TASKS,
                                           [UT_OBJTYPE_QUEUE]      = OS_MAX_QUEUES,
                                           [UT_OBJTYPE_COUNTSEM]   = OS_MAX_COUNT_SEMAPHORES,
                                           [UT_OBJTYPE_BINSEM]     = OS_MAX_BIN_SEMAPHORES,
                                           [UT_OBJTYPE_MUTEX]      = OS_MAX_MUTEXES,
                                           [UT_OBJTYPE_TIMECB]     = OS_MAX_TIMERS,
                                           [UT_OBJTYPE_MODULE]     = OS_MAX_MODULES,
                                           [UT_OBJTYPE_FILESTREAM] = OS_MAX_NUM_OPEN_FILES,
                                           [UT_OBJTYPE_TIMEBASE]   = OS_MAX_TIMEBASES,
                                           [UT_OBJTYPE_FILESYS]    = OS_MAX_FILE_SYSTEMS,
                                           [UT_OBJTYPE_DIR]        = OS_MAX_NUM_OPEN_DIRS};

static UT_ObjTypeState_t UT_ObjState[UT_OBJTYPE_MAX];

/**
 * Initialization function
 */
void UT_ClearAllStubObjects(void)
{
    /*
     * Reset the fake ID numbers for create/delete operations
     */
    memset(UT_ObjState, 0, sizeof(UT_ObjState));
}

/*
 * Helper function - "allocate" a fake object ID of the given type
 */
osal_id_t UT_AllocStubObjId(UT_ObjType_t ObjType)
{
    UT_ObjTypeState_t *StatePtr;
    uint8              ObjMask;
    uint32             indx;
    osal_id_t          Result;

    UT_Stub_CallOnce(UT_ClearAllStubObjects);

    if (ObjType == UT_OBJTYPE_NONE || ObjType >= UT_OBJTYPE_MAX)
    {
        /* Code is broken, abort the test
         * (This signifies an error in the stub code itself hence the abort)
         */
        UtAssert_Abort("ObjType out of range");
    }

    StatePtr = &UT_ObjState[ObjType];

    if (StatePtr->LastIssueNumber < ((8 * sizeof(StatePtr->ValidBits)) - 1))
    {
        ++StatePtr->LastIssueNumber;
    }

    indx = StatePtr->LastIssueNumber;

    ObjMask = 1 << (indx & 0x07);
    /*
     * Check for overlap/re-issue - this COULD happen when using
     * the original (non-opaque) object IDs if a UT creates too many
     * objects of a certain type.  There is really no way around this
     * and it means the test needs to be revised to not create so many
     * objects OR it needs to support opaque object IDs
     */
    if ((StatePtr->ValidBits[indx >> 3] & ObjMask) != 0)
    {
        UtAssert_Failed("OSAPI UT stub object overlap");
    }
    StatePtr->ValidBits[indx >> 3] |= ObjMask;

    /*
     * Finalize Object ID - put into proper range for type
     */
    UT_ObjIdCompose(indx, ObjType, &Result);

    return Result;
}

/*
 * Helper function - "deallocate" a fake object ID of the given type
 */
void UT_DeleteStubObjId(UT_ObjType_t ObjType, osal_id_t ObjId)
{
    UT_ObjTypeState_t *StatePtr;
    uint8              ObjMask;
    UT_ObjType_t       checktype;
    uint32             checkidx;
    bool               ObjWasValid;

    UT_Stub_CallOnce(UT_ClearAllStubObjects);

    UT_ObjIdDecompose(ObjId, &checkidx, &checktype);

    /*
     * Verify the object type
     */
    if (ObjType != checktype)
    {
        /* Calling code is broken, abort the test */
        UtAssert_Failed("Object type is not correct");
    }

    if (checkidx >= (8 * sizeof(StatePtr->ValidBits)))
    {
        /* Calling code is broken */
        UtAssert_Failed("ObjId out of range");
    }

    StatePtr = &UT_ObjState[ObjType];

    /* Clear out any bit it could have been */
    ObjWasValid = false;
    ObjMask     = 1 << (checkidx & 0x07);
    if ((StatePtr->ValidBits[checkidx >> 3] & ObjMask) != 0)
    {
        ObjWasValid = true;
        StatePtr->ValidBits[checkidx >> 3] &= ~ObjMask;
    }

    /* Unfortunately, some code has a habit of just blindly calling "Delete"
     * regardless of whether those entities were created or not.  This is
     * an issue on the non-opaque OSAL implementations and will produce
     * thousands of failed test cases, so this can only be verified
     * on the opaque objid versions.
     */
    if (!ObjWasValid)
    {
        /*
         * These stubs can be more strict about the usage patterns.
         *
         * In general any app calling delete on an ID they did not create likely
         * signifies an error in that application.
         */
        UtAssert_Failed("Deleted object that was never created");
    }
}

void UT_ObjIdCompose(uint32 indx, UT_ObjType_t objtype, osal_id_t *id)
{
    /* note - the OS_ObjectIdFromInteger() is an inline function,
     * and therefore this uses the real thing and not a stub  */
    *id = OS_ObjectIdFromInteger((unsigned long)indx | ((0x4000UL | objtype) << 16));
}

void UT_ObjIdDecompose(osal_id_t id, uint32 *indx, UT_ObjType_t *objtype)
{
    unsigned long idv = OS_ObjectIdToInteger(id);
    *indx             = idv & 0xFFFFUL;
    *objtype          = (idv >> 16) ^ 0x4000UL;
}

/*
** Report and close any sockets found open
** Moved here temporarily to ensure full compatibility with CFE implementation
**
** NOTE - this historically only checked for queues that were created but not
** cleaned up.  Although the current impl could check for anything, only queues
** are done for now.
*/
void UT_CheckForOpenSockets(void)
{
    UT_ObjTypeState_t *StatePtr;
    uint32             i;

    StatePtr = &UT_ObjState[UT_OBJTYPE_QUEUE];
    for (i = 0; i <= StatePtr->LastIssueNumber; ++i)
    {
        if ((StatePtr->ValidBits[i >> 3] & (1 << (i & 0x07))) != 0)
        {
            UtAssert_Failed("UT_Queue %d left open.\n", (int)i);
        }
    }
}
