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
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 * This file contains utility functions to manipulate/interpret OSAL IDs
 * in a generic/common manner.
 *
 * In order to add additional verification capabilities, each class of fundamental
 * objects will use its own ID space within the 32-bit integer ID value.  This way
 * one could not mistake a Task ID for a Queue ID or vice versa.  Also, all IDs will
 * become nonzero and an ID of zero is ALWAYS invalid.
 *
 * These functions provide a consistent way to validate a 32-bit OSAL ID as
 * well as determine its internal type and index.
 *
 * NOTE: This file includes local helpers, OSAL scope, and public API implementations
 * as documented in the function headers
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * User defined include files
 */
#include "os-shared-common.h"
#include "os-shared-idmap.h"
#include "os-shared-task.h"

/*
 * A fixed nonzero value to put into the upper 8 bits
 * of lock keys.
 */
#define OS_LOCK_KEY_FIXED_VALUE 0x4D000000
#define OS_LOCK_KEY_INVALID     ((osal_key_t) {0})

/*
 * A structure containing the user-specified
 * details of a "foreach" iteration request
 */
typedef struct
{
    osal_id_t        creator_id;
    OS_ArgCallback_t user_callback;
    void *           user_arg;
} OS_creator_filter_t;

/*
 * Global ID storage tables
 */

/* Tables where the OS object information is stored */
static OS_common_record_t OS_common_table[OS_MAX_TOTAL_RECORDS];

typedef struct
{
    /* Keep track of the last successfully-issued object ID of each type */
    osal_id_t last_id_issued;

    /* The number of individual transactions (lock/unlock cycles) on this type */
    uint32 transaction_count;

    /* The key required to unlock this table */
    osal_key_t owner_key;
} OS_objtype_state_t;

OS_objtype_state_t OS_objtype_state[OS_OBJECT_TYPE_USER];

OS_common_record_t *const OS_global_task_table      = &OS_common_table[OS_TASK_BASE];
OS_common_record_t *const OS_global_queue_table     = &OS_common_table[OS_QUEUE_BASE];
OS_common_record_t *const OS_global_bin_sem_table   = &OS_common_table[OS_BINSEM_BASE];
OS_common_record_t *const OS_global_count_sem_table = &OS_common_table[OS_COUNTSEM_BASE];
OS_common_record_t *const OS_global_mutex_table     = &OS_common_table[OS_MUTEX_BASE];
OS_common_record_t *const OS_global_stream_table    = &OS_common_table[OS_STREAM_BASE];
OS_common_record_t *const OS_global_dir_table       = &OS_common_table[OS_DIR_BASE];
OS_common_record_t *const OS_global_timebase_table  = &OS_common_table[OS_TIMEBASE_BASE];
OS_common_record_t *const OS_global_timecb_table    = &OS_common_table[OS_TIMECB_BASE];
OS_common_record_t *const OS_global_module_table    = &OS_common_table[OS_MODULE_BASE];
OS_common_record_t *const OS_global_filesys_table   = &OS_common_table[OS_FILESYS_BASE];
OS_common_record_t *const OS_global_console_table   = &OS_common_table[OS_CONSOLE_BASE];
OS_common_record_t *const OS_global_condvar_table   = &OS_common_table[OS_CONDVAR_BASE];

/*
 *********************************************************************************
 *          IDENTIFIER MAP / UNMAP FUNCTIONS
 *********************************************************************************
 */

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           clears the entire table and brings it to a proper initial state
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdInit(void)
{
    memset(OS_common_table, 0, sizeof(OS_common_table));
    memset(OS_objtype_state, 0, sizeof(OS_objtype_state));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
uint32 OS_GetMaxForObjectType(osal_objtype_t idtype)
{
    switch (idtype)
    {
        case OS_OBJECT_TYPE_OS_TASK:
            return OS_MAX_TASKS;
        case OS_OBJECT_TYPE_OS_QUEUE:
            return OS_MAX_QUEUES;
        case OS_OBJECT_TYPE_OS_BINSEM:
            return OS_MAX_BIN_SEMAPHORES;
        case OS_OBJECT_TYPE_OS_COUNTSEM:
            return OS_MAX_COUNT_SEMAPHORES;
        case OS_OBJECT_TYPE_OS_MUTEX:
            return OS_MAX_MUTEXES;
        case OS_OBJECT_TYPE_OS_STREAM:
            return OS_MAX_NUM_OPEN_FILES;
        case OS_OBJECT_TYPE_OS_DIR:
            return OS_MAX_NUM_OPEN_DIRS;
        case OS_OBJECT_TYPE_OS_TIMEBASE:
            return OS_MAX_TIMEBASES;
        case OS_OBJECT_TYPE_OS_TIMECB:
            return OS_MAX_TIMERS;
        case OS_OBJECT_TYPE_OS_MODULE:
            return OS_MAX_MODULES;
        case OS_OBJECT_TYPE_OS_FILESYS:
            return OS_MAX_FILE_SYSTEMS;
        case OS_OBJECT_TYPE_OS_CONSOLE:
            return OS_MAX_CONSOLES;
        case OS_OBJECT_TYPE_OS_CONDVAR:
            return OS_MAX_CONDVARS;
        default:
            return 0;
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
uint32 OS_GetBaseForObjectType(osal_objtype_t idtype)
{
    switch (idtype)
    {
        case OS_OBJECT_TYPE_OS_TASK:
            return OS_TASK_BASE;
        case OS_OBJECT_TYPE_OS_QUEUE:
            return OS_QUEUE_BASE;
        case OS_OBJECT_TYPE_OS_BINSEM:
            return OS_BINSEM_BASE;
        case OS_OBJECT_TYPE_OS_COUNTSEM:
            return OS_COUNTSEM_BASE;
        case OS_OBJECT_TYPE_OS_MUTEX:
            return OS_MUTEX_BASE;
        case OS_OBJECT_TYPE_OS_STREAM:
            return OS_STREAM_BASE;
        case OS_OBJECT_TYPE_OS_DIR:
            return OS_DIR_BASE;
        case OS_OBJECT_TYPE_OS_TIMEBASE:
            return OS_TIMEBASE_BASE;
        case OS_OBJECT_TYPE_OS_TIMECB:
            return OS_TIMECB_BASE;
        case OS_OBJECT_TYPE_OS_MODULE:
            return OS_MODULE_BASE;
        case OS_OBJECT_TYPE_OS_FILESYS:
            return OS_FILESYS_BASE;
        case OS_OBJECT_TYPE_OS_CONSOLE:
            return OS_CONSOLE_BASE;
        case OS_OBJECT_TYPE_OS_CONDVAR:
            return OS_CONDVAR_BASE;
        default:
            return 0;
    }
}

/**************************************************************
 * LOCAL HELPER FUNCTIONS
 * (not used outside of this unit)
 **************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Determine if the object is a match for "foreach" operations
 *
 *-----------------------------------------------------------------*/
bool OS_ForEachFilterCreator(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    OS_creator_filter_t *filter = ref;

    /*
     * Check if the obj_id is both valid and matches
     * the specified creator_id
     */
    return (OS_ObjectIdIsValid(obj->active_id) && (OS_ObjectIdEqual(filter->creator_id, OS_OBJECT_CREATOR_ANY) ||
                                                   OS_ObjectIdEqual(obj->creator, filter->creator_id)));
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Invoke the user-specified callback routine
 *
 *-----------------------------------------------------------------*/
int32 OS_ForEachDoCallback(osal_id_t obj_id, void *ref)
{
    OS_creator_filter_t *filter = ref;

    /* Just invoke the user callback */
    filter->user_callback(obj_id, filter->user_arg);
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Gets the global/common record associated with the token
 *
 *  returns: pointer to record (never NULL - token MUST be valid)
 *
 *-----------------------------------------------------------------*/
OS_common_record_t *OS_ObjectIdGlobalFromToken(const OS_object_token_t *token)
{
    uint32 base_idx = OS_GetBaseForObjectType(token->obj_type);
    return &OS_common_table[base_idx + token->obj_idx];
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           A matching function to compare the name of the record against
 *           a reference value (which must be a const char* string).
 *
 *           This allows OS_ObjectIdFindByName() to be implemented using the
 *           generic OS_ObjectIdFindNextMatch() routine.
 *
 *  returns: true if match, false otherwise
 *
 *-----------------------------------------------------------------*/
bool OS_ObjectNameMatch(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    return (obj->name_entry != NULL && strcmp((const char *)ref, obj->name_entry) == 0);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *   Initiate the locking process for the given mode and ID type, prior
 *   to looking up a specific object.
 *
 *   For any lock_mode other than OS_LOCK_MODE_NONE, this acquires the
 *   global table lock for that ID type.
 *
 *   Once the lookup operation is completed, the OS_ObjectIdConvertToken()
 *   routine should be used to convert this global lock into the actual
 *   lock type requested (lock_mode).
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdTransactionInit(OS_lock_mode_t lock_mode, osal_objtype_t idtype, OS_object_token_t *token)
{
    memset(token, 0, sizeof(*token));

    /*
     * Confirm that OSAL has been fully initialized before allowing any transactions
     */
    if (OS_SharedGlobalVars.GlobalState != OS_INIT_MAGIC_NUMBER &&
        OS_SharedGlobalVars.GlobalState != OS_SHUTDOWN_MAGIC_NUMBER)
    {
        return OS_ERROR;
    }

    /*
     * only "exclusive" locks allowed after shutdown request (this is mode used for delete).
     * All regular ops will be blocked.
     */
    if (OS_SharedGlobalVars.GlobalState == OS_SHUTDOWN_MAGIC_NUMBER && lock_mode != OS_LOCK_MODE_EXCLUSIVE)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    /*
     * Transactions cannot be started on an object type for which
     * there are no actual objects
     */
    if (OS_GetMaxForObjectType(idtype) == 0)
    {
        return OS_ERR_INVALID_ID;
    }

    token->lock_mode = lock_mode;
    token->obj_type  = idtype;
    token->obj_idx   = OSAL_INDEX_C(-1);

    if (lock_mode != OS_LOCK_MODE_NONE)
    {
        OS_Lock_Global(token);
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Cancels/aborts a previously initialized transaction
 *
 *-----------------------------------------------------------------*/
void OS_ObjectIdTransactionCancel(OS_object_token_t *token)
{
    if (token->lock_mode != OS_LOCK_MODE_NONE)
    {
        OS_Unlock_Global(token);
        token->lock_mode = OS_LOCK_MODE_NONE;
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *   Selectively convert the existing lock on a given resource, depending on the lock mode.
 *
 *   For any lock_mode other than OS_LOCK_MODE_NONE, the global table lock **must**
 *   already be held prior to entering this function.  This function may or may
 *   not unlock the global table, depending on the lock_mode and state of the entry.
 *
 *   For all modes, this verifies that the reference_id passed in and the active_id
 *   within the record are a match.  If they do not match, then OS_ERR_INVALID_ID
 *   is returned.
 *
 *   If lock_mode is set to either OS_LOCK_MODE_NONE or OS_LOCK_MODE_GLOBAL,
 *   no additional operation is performed, as the existing lock (if any) is
 *   sufficient and no conversion is necessary.
 *
 *   If lock_mode is set to OS_LOCK_MODE_REFCOUNT, then this increments
 *   the reference count within the object itself and releases the table lock,
 *   so long as there is no "exclusive" request already pending.
 *
 *   If lock_mode is set to OS_LOCK_MODE_EXCLUSIVE, then this verifies
 *   that the refcount is zero, but also keeps the global lock held.
 *
 *   For EXCLUSIVE and REFCOUNT style locks, if the state is not appropriate,
 *   this may unlock the global table and re-lock it several times
 *   while waiting for the state to change.
 *
 *   Returns: OS_SUCCESS if operation was successful,
 *            or suitable error code if operation was not successful.
 *
 *   NOTE: Upon failure, the global table lock is always released for
 *         all lock modes other than OS_LOCK_MODE_NONE.
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdConvertToken(OS_object_token_t *token)
{
    int32               return_code = OS_ERROR;
    uint32              attempts    = 0;
    OS_common_record_t *obj;
    osal_id_t           expected_id;

    obj         = OS_ObjectIdGlobalFromToken(token);
    expected_id = OS_ObjectIdFromToken(token);

    /*
     * Upon entry the ID from the token must be valid
     */
    if (!OS_ObjectIdIsValid(expected_id))
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    /*
     * If lock mode is RESERVED, then the ID in the record should
     * already be set to OS_OBJECT_ID_RESERVED.  This is for very
     * specific use cases where a secondary task needs to access an
     * object during its creation/deletion.
     *
     * For all typical modes the ID in the record should be equal
     * to the token ID.
     */
    if (token->lock_mode == OS_LOCK_MODE_RESERVED)
    {
        expected_id = OS_OBJECT_ID_RESERVED;
    }

    while (true)
    {
        /* Validate the integrity of the ID.  As the "active_id" is a single
         * integer, we can do this check regardless of whether global is locked or not. */
        if (OS_ObjectIdEqual(obj->active_id, expected_id))
        {
            /*
             * Got an ID match...
             */
            if (token->lock_mode == OS_LOCK_MODE_EXCLUSIVE)
            {
                /*
                 * For EXCLUSIVE mode, overwrite the ID to be RESERVED now -- this
                 * makes any future ID checks or lock attempts in other tasks fail to match.
                 */
                if (!OS_ObjectIdEqual(expected_id, OS_OBJECT_ID_RESERVED))
                {
                    expected_id    = OS_OBJECT_ID_RESERVED;
                    obj->active_id = expected_id;
                }

                /*
                 * Also confirm that reference count is zero
                 * If not zero, will need to wait for other tasks to release.
                 */
                if (obj->refcount == 0)
                {
                    return_code = OS_SUCCESS;
                    break;
                }
            }
            else
            {
                /*
                 * Nothing else to test for this lock type
                 */
                return_code = OS_SUCCESS;
                break;
            }
        }
        else if (token->lock_mode == OS_LOCK_MODE_NONE || !OS_ObjectIdEqual(obj->active_id, OS_OBJECT_ID_RESERVED))
        {
            /* Not an ID match and not RESERVED - fail out */
            return_code = OS_ERR_INVALID_ID;
            break;
        }

        /*
         * If we get this far, it means there is contention for access to the object.
         *  a) we want to some type of lock but the ID is currently RESERVED
         *  b) the refcount is too high - need to wait for release
         *
         * In this case we will UNLOCK the global object again so that the holder
         * can relinquish it.  We'll try again a few times before giving up hope.
         */
        ++attempts;
        if (attempts >= 5)
        {
            return_code = OS_ERR_OBJECT_IN_USE;
            break;
        }

        /*
         * Call the impl layer to wait for some sort of change to occur.
         */
        OS_WaitForStateChange(token, attempts);
    }

    /*
     * Determine if the global table needs to be unlocked now.
     *
     * If lock_mode is OS_LOCK_MODE_NONE, then the table was never locked
     * to begin with, and therefore never needs to be unlocked.
     */
    if (token->lock_mode != OS_LOCK_MODE_NONE)
    {
        if (return_code == OS_SUCCESS)
        {
            /* always increment the refcount, which means a task is actively
             * using or modifying this record. */
            ++obj->refcount;

            /*
             * On a successful operation, the global is unlocked if it is
             * a REFCOUNT or EXCLUSIVE lock.  Note for EXCLUSIVE, because the ID
             * was overwritten to OS_OBJECT_ID_RESERVED, other tasks will not be
             * able to access the object because the ID will not match, so the
             * table can be unlocked while the remainder of the create/delete process
             * continues.
             *
             * For OS_LOCK_MODE_GLOBAL the global lock should be maintained and
             * returned to the caller.
             */
            if (token->lock_mode == OS_LOCK_MODE_REFCOUNT || token->lock_mode == OS_LOCK_MODE_EXCLUSIVE)
            {
                OS_Unlock_Global(token);
            }
        }
        else if (token->lock_mode == OS_LOCK_MODE_EXCLUSIVE && OS_ObjectIdEqual(expected_id, OS_OBJECT_ID_RESERVED))
        {
            /*
             * On failure, if the active_id was overwritten, then set
             * it back to the original value which is in the token.
             * (note it had to match initially before overwrite)
             */
            obj->active_id = OS_ObjectIdFromToken(token);
        }
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Locate an existing object using the supplied Match function.
 *           Matching object ID is stored in the object_id pointer
 *
 *           This is an internal function and no table locking is performed here.
 *           Locking must be done by the calling function.
 *
 *  returns: OS_ERR_NAME_NOT_FOUND if not found, OS_SUCCESS if match is found
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdFindNextMatch(OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_object_token_t *token)
{
    int32               return_code;
    uint32              obj_count;
    OS_common_record_t *base;
    OS_common_record_t *record;

    return_code   = OS_ERR_NAME_NOT_FOUND;
    base          = &OS_common_table[OS_GetBaseForObjectType(token->obj_type)];
    obj_count     = OS_GetMaxForObjectType(token->obj_type);
    token->obj_id = OS_OBJECT_ID_UNDEFINED;

    while (true)
    {
        ++token->obj_idx;

        if (token->obj_idx >= obj_count)
        {
            break;
        }

        record = OS_OBJECT_TABLE_GET(base, *token);

        if (OS_ObjectIdDefined(record->active_id) && MatchFunc(arg, token, record))
        {
            return_code   = OS_SUCCESS;
            token->obj_id = record->active_id;
            break;
        }
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Find the next available Object ID of the given type
 *           Searches the global name/id table for an open entry of the given type.
 *           The search will start at the location of the last-issued ID.
 *
 *           Note: This is an internal helper function and no locking is performed.
 *           The appropriate global table lock must be held prior to calling this.
 *
 *  Outputs: *record is set to point to the global entry and active_id member is set
 *           *array_index updated to the offset of the found entry (local_id)
 *
 *  returns: OS_SUCCESS if an empty location was found.
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdFindNextFree(OS_object_token_t *token)
{
    uint32              max_id;
    uint32              base_id;
    uint32              local_id = 0;
    uint32              serial;
    uint32              i;
    int32               return_code;
    OS_common_record_t *obj = NULL;
    OS_objtype_state_t *objtype_state;

    base_id       = OS_GetBaseForObjectType(token->obj_type);
    max_id        = OS_GetMaxForObjectType(token->obj_type);
    objtype_state = &OS_objtype_state[token->obj_type];

    if (max_id == 0)
    {
        /* if the max id is zero, then this build of OSAL
         * does not include any support for that object type.
         * Return the "not implemented" to differentiate between
         * this case vs. running out of valid slots  */
        return_code = OS_ERR_NOT_IMPLEMENTED;
        serial      = 0;
    }
    else
    {
        return_code = OS_ERR_NO_FREE_IDS;
        serial      = OS_ObjectIdToSerialNumber_Impl(objtype_state->last_id_issued);
    }

    for (i = 0; i < max_id; ++i)
    {
        local_id = (++serial) % max_id;
        if (serial >= OS_OBJECT_INDEX_MASK)
        {
            /* reset to beginning of ID space */
            serial = local_id;
        }
        obj = &OS_common_table[local_id + base_id];
        if (!OS_ObjectIdDefined(obj->active_id))
        {
            return_code = OS_SUCCESS;
            break;
        }
    }

    if (return_code == OS_SUCCESS)
    {
        token->obj_idx = OSAL_INDEX_C(local_id);
        OS_ObjectIdCompose_Impl(token->obj_type, serial, &token->obj_id);

        /* Ensure any data in the record has been cleared */
        obj->active_id  = token->obj_id;
        obj->name_entry = NULL;
        obj->creator    = OS_TaskGetId();
        obj->refcount   = 0;

        /* preemptively update the last id issued */
        objtype_state->last_id_issued = token->obj_id;
    }

    if (return_code != OS_SUCCESS)
    {
        token->obj_idx = OSAL_INDEX_C(-1);
        token->obj_id  = OS_OBJECT_ID_UNDEFINED;
    }

    return return_code;
}

/*
 *********************************************************************************
 *          OSAL INTERNAL FUNCTIONS
 *
 * These functions are invoked by other units within OSAL,
 *  but are NOT directly invoked by applications
 *********************************************************************************
 */

/*----------------------------------------------------------------

    Purpose: Locks the global table identified by "idtype"
 ------------------------------------------------------------------*/
void OS_Lock_Global(OS_object_token_t *token)
{
    osal_id_t           self_task_id;
    OS_objtype_state_t *objtype;

    if (token->obj_type < OS_OBJECT_TYPE_USER && token->lock_mode != OS_LOCK_MODE_NONE)
    {
        objtype      = &OS_objtype_state[token->obj_type];
        self_task_id = OS_TaskGetId_Impl();

        OS_Lock_Global_Impl(token->obj_type);

        /*
         * Track ownership of this table.  It should only be owned by one
         * task at a time, and this aids in recovery if the owning task is
         * deleted or experiences an exception causing it to not be freed.
         *
         * This is done after successfully locking, so this has exclusive access
         * to the state object.
         */
        if (!OS_ObjectIdIsValid(self_task_id))
        {
            /*
             * This just means the calling context is not an OSAL-created task.
             * This is not necessarily an error, but it should be tracked.
             * Also note that the root/initial task also does not have an ID.
             */
            self_task_id = OS_OBJECT_ID_RESERVED; /* nonzero, but also won't alias a known task */
        }

        /*
         * The key value is computed with fixed/nonzero flag bits combined
         * with the lower 24 bits of the task ID xor'ed with transaction id.
         * This makes it different for every operation, and different depending
         * on what task is calling the function.
         */
        token->lock_key.key_value =
            OS_LOCK_KEY_FIXED_VALUE | ((OS_ObjectIdToInteger(self_task_id) ^ objtype->transaction_count) & 0xFFFFFF);

        ++objtype->transaction_count;

        if (objtype->owner_key.key_value != 0)
        {
            /* this is almost certainly a bug */
            OS_DEBUG("ERROR: global %u acquired by task 0x%lx when already assigned key 0x%lx\n",
                     (unsigned int)token->obj_type, OS_ObjectIdToInteger(self_task_id),
                     (unsigned long)objtype->owner_key.key_value);
        }
        else
        {
            objtype->owner_key = token->lock_key;
        }
    }
    else
    {
        OS_DEBUG("ERROR: cannot lock global %u for mode %u\n", (unsigned int)token->obj_type,
                 (unsigned int)token->lock_mode);
    }
}

/*----------------------------------------------------------------

    Purpose: Unlocks the global table identified by "idtype"
 ------------------------------------------------------------------*/
void OS_Unlock_Global(OS_object_token_t *token)
{
    OS_objtype_state_t *objtype;

    if (token->obj_type < OS_OBJECT_TYPE_USER && token->lock_mode != OS_LOCK_MODE_NONE)
    {
        objtype = &OS_objtype_state[token->obj_type];

        /*
         * Un-track ownership of this table.  It should only be owned by one
         * task at a time, and this aids in recovery if the owning task is
         * deleted or experiences an exception causing it to not be freed.
         *
         * This is done before unlocking, while this has exclusive access
         * to the state object.
         */
        if ((objtype->owner_key.key_value & 0xFF000000) != OS_LOCK_KEY_FIXED_VALUE ||
            objtype->owner_key.key_value != token->lock_key.key_value)
        {
            /* this is almost certainly a bug */
            OS_DEBUG("ERROR: global %u released using mismatched key=0x%lx expected=0x%lx\n",
                     (unsigned int)token->obj_type, (unsigned long)token->lock_key.key_value,
                     (unsigned long)objtype->owner_key.key_value);
        }

        objtype->owner_key = OS_LOCK_KEY_INVALID;
        token->lock_key    = OS_LOCK_KEY_INVALID;

        OS_Unlock_Global_Impl(token->obj_type);
    }
    else
    {
        OS_DEBUG("ERROR: cannot unlock global %u for mode %u\n", (unsigned int)token->obj_type,
                 (unsigned int)token->lock_mode);
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *  Waits for a change in the global table identified by "idtype"
 *
 *  NOTE: this must be called while the table is _LOCKED_
 *  The "OS_WaitForStateChange_Impl" function should unlock + relock
 *
 *-----------------------------------------------------------------*/
void OS_WaitForStateChange(OS_object_token_t *token, uint32 attempts)
{
    osal_key_t          saved_unlock_key;
    OS_objtype_state_t *objtype;

    /*
     * This needs to release the lock, to allow other
     * tasks to make a change to the table.  But to avoid
     * ownership warnings the key must also be temporarily
     * cleared too, and restored after waiting.
     */

    objtype          = &OS_objtype_state[token->obj_type];
    saved_unlock_key = objtype->owner_key;

    /* temporarily release the table */
    objtype->owner_key = OS_LOCK_KEY_INVALID;

    /*
     * The implementation layer takes care of the actual unlock + wait.
     * This permits use of condition variables where these two actions
     * are done atomically.
     */
    OS_WaitForStateChange_Impl(token->obj_type, attempts);

    /*
     * After return, this task owns the table again
     */
    /* cppcheck-suppress redundantAssignment */
    objtype->owner_key = saved_unlock_key;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Called when the initialization of a newly-issued object ID is fully complete,
 *           to perform finalization of the object and record state.
 *
 *           If the operation_status was successful (OS_SUCCESS) then the ID is exported
 *           to the caller through the "outid" pointer.
 *
 *           If the operation_status is unsuccessful, then the temporary id in the record
 *           is cleared and an ID value of 0 is exported to the caller.
 *
 *  returns: The same operation_status value passed-in, or OS_ERR_INVALID_ID if problems
 *           were detected while validating the ID.
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdFinalizeNew(int32 operation_status, OS_object_token_t *token, osal_id_t *outid)
{
    osal_id_t final_id;

    /* if operation was unsuccessful, then clear
     * the active_id field within the record, so
     * the record can be re-used later.
     *
     * Otherwise, ensure that the record_id to be
     * exported is sane (it always should be)
     */
    if (operation_status == OS_SUCCESS)
    {
        final_id = token->obj_id;
    }
    else
    {
        final_id = OS_OBJECT_ID_UNDEFINED;
    }

    /* Either way we must unlock the object type */
    OS_ObjectIdTransactionFinish(token, &final_id);

    /* Give event callback to the application */
    if (operation_status == OS_SUCCESS)
    {
        OS_NotifyEvent(OS_EVENT_RESOURCE_CREATED, token->obj_id, NULL);
    }

    if (outid != NULL)
    {
        /* always write the final value to the output buffer */
        *outid = final_id;
    }

    return operation_status;
}

/*----------------------------------------------------------------

    Purpose: Helper routine, not part of OSAL public API.
             See description in prototype
 ------------------------------------------------------------------*/
int32 OS_ObjectIdFinalizeDelete(int32 operation_status, OS_object_token_t *token)
{
    osal_id_t final_id;

    /* Clear the OSAL ID if successful - this returns the record to the pool */
    if (operation_status == OS_SUCCESS)
    {
        final_id = OS_OBJECT_ID_UNDEFINED;
    }
    else
    {
        /* this restores the original ID */
        final_id = token->obj_id;
    }

    /* Either way we must unlock the object type */
    OS_ObjectIdTransactionFinish(token, &final_id);

    /* Give event callback to the application */
    if (operation_status == OS_SUCCESS)
    {
        OS_NotifyEvent(OS_EVENT_RESOURCE_DELETED, token->obj_id, NULL);
    }

    return operation_status;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Locate an existing object using the supplied Match function.
 *           Matching object ID is stored in the object_id pointer
 *
 *           Global locking is performed according to the lock_mode
 *           parameter.
 *
 *  returns: OS_ERR_NAME_NOT_FOUND if not found, OS_SUCCESS if match is found
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, osal_objtype_t idtype, OS_ObjectMatchFunc_t MatchFunc, void *arg,
                             OS_object_token_t *token)
{
    int32 return_code;

    OS_ObjectIdTransactionInit(lock_mode, idtype, token);

    return_code = OS_ObjectIdFindNextMatch(MatchFunc, arg, token);

    if (return_code == OS_SUCCESS)
    {
        /*
         * The "ConvertToken" routine will return with the global lock
         * in a state appropriate for returning to the caller, as indicated
         * by the "lock_mode" parameter.
         */
        return_code = OS_ObjectIdConvertToken(token);
    }
    else
    {
        OS_ObjectIdTransactionCancel(token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Locate an existing object with matching name and type
 *           Matching record is stored in the record pointer
 *
 *           Global locking is performed according to the lock_mode
 *           parameter.
 *
 *  returns: OS_ERR_NAME_NOT_FOUND if not found, OS_SUCCESS if match is found
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdGetByName(OS_lock_mode_t lock_mode, osal_objtype_t idtype, const char *name, OS_object_token_t *token)
{
    return OS_ObjectIdGetBySearch(lock_mode, idtype, OS_ObjectNameMatch, (void *)name, token);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Locate an existing object with matching name and type
 *           Matching object ID is stored in the object_id pointer
 *
 *  returns: OS_ERR_NAME_NOT_FOUND if not found, OS_SUCCESS if match is found
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdFindByName(osal_objtype_t idtype, const char *name, osal_id_t *object_id)
{
    int32             return_code;
    OS_object_token_t token;

    /*
     * As this is an internal-only function, calling it with NULL is allowed.
     * This is required by the file/dir/socket API since these DO allow multiple
     * instances of the same name.
     */
    ARGCHECK(name, OS_ERR_NAME_NOT_FOUND);
    LENGTHCHECK(name, OS_MAX_API_NAME, OS_ERR_NAME_TOO_LONG);

    return_code = OS_ObjectIdGetByName(OS_LOCK_MODE_GLOBAL, idtype, name, &token);
    if (return_code == OS_SUCCESS)
    {
        *object_id = token.obj_id;

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Gets the resource record pointer and index associated with the given resource ID.
 *           If successful, this returns with the item locked according to "lock_mode".
 *
 *           IMPORTANT: when this function returns OS_SUCCESS with lock_mode something
 *           other than NONE, then the caller must take appropriate action to UNLOCK
 *           after completing the respective operation.  The OS_ObjectIdRelease()
 *           function may be used to release the lock appropriately for the lock_mode.
 *
 *           If this returns something other than OS_SUCCESS then the global is NOT locked.
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdGetById(OS_lock_mode_t lock_mode, osal_objtype_t idtype, osal_id_t id, OS_object_token_t *token)
{
    int32 return_code;

    return_code = OS_ObjectIdTransactionInit(lock_mode, idtype, token);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    return_code = OS_ObjectIdToArrayIndex(idtype, id, &token->obj_idx);
    if (return_code == OS_SUCCESS)
    {
        token->obj_id = id;

        /*
         * The "ConvertToken" routine will return with the global lock
         * in a state appropriate for returning to the caller, as indicated
         * by the "check_mode" parameter.
         *
         * Note If this operation fails, then it always unlocks the global for
         * all check_mode's other than NONE.
         */
        return_code = OS_ObjectIdConvertToken(token);
    }

    if (return_code != OS_SUCCESS)
    {
        OS_ObjectIdTransactionCancel(token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Complete a transaction which was previously obtained via
 *           OS_ObjectIdGetById() or OS_ObjectIdGetBySearch().
 *
 * This also updates the ID from the value in the final_id parameter, which
 * is used for create/delete.
 *
 * If no ID update is pending, then NULL may be passed and the ID will not
 * be changed.
 *
 *-----------------------------------------------------------------*/
void OS_ObjectIdTransactionFinish(OS_object_token_t *token, const osal_id_t *final_id)
{
    OS_common_record_t *record;

    if (token->lock_mode == OS_LOCK_MODE_NONE)
    {
        /* nothing to do */
        return;
    }

    record = OS_ObjectIdGlobalFromToken(token);

    /* re-acquire global table lock to adjust refcount */
    if (token->lock_mode == OS_LOCK_MODE_EXCLUSIVE || token->lock_mode == OS_LOCK_MODE_REFCOUNT)
    {
        OS_Lock_Global(token);
    }

    if (record->refcount > 0)
    {
        --record->refcount;
    }

    /*
     * at this point the global mutex is always held, either
     * from re-acquiring it above or it is still held from
     * the original lock when using OS_LOCK_MODE_GLOBAL.
     *
     * If an ID update was pending (i.e. for a create/delete op)
     * then do the ID update now while holding the mutex.
     */
    if (final_id != NULL)
    {
        record->active_id = *final_id;
    }
    else if (token->lock_mode == OS_LOCK_MODE_EXCLUSIVE)
    {
        /*
         * If the lock type was EXCLUSIVE, it means that the ID in the record
         * was reset to OS_OBJECT_ID_RESERVED.  This must restore the original
         * object ID from the token.
         */
        record->active_id = token->obj_id;
    }

    /* always unlock (this also covers OS_LOCK_MODE_GLOBAL case) */
    OS_Unlock_Global(token);

    /*
     * Setting to "NONE" indicates that this token has been
     * released, and should not be released again.
     */
    token->lock_mode = OS_LOCK_MODE_NONE;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Release/Unlock a transaction token which was previously obtained via
 *           OS_ObjectIdGetById() or OS_ObjectIdGetBySearch().
 *
 * This is used for completing normal operations other than create/delete -
 * that is where the same ID exists before and after the transaction without
 * change.
 *
 * (There is a dedicated routine for finalization of create and delete ops)
 *
 *-----------------------------------------------------------------*/
void OS_ObjectIdRelease(OS_object_token_t *token)
{
    OS_ObjectIdTransactionFinish(token, NULL);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           Locks the global table for the indicated ID type and allocates a
 *           new object of the given type with the given name.
 *
 *   Inputs: last_alloc_id represents the previously issued ID of this type.
 *              (The search for a free entry will start here +1 to avoid repeats).
 *
 *  Outputs: *record is set to point to the global entry and active_id member is set
 *
 *  returns: OS_SUCCESS if a NEW object was allocated and the table remains locked.
 *
 *  IMPORTANT: The global table is remains in a locked state if this returns OS_SUCCESS,
 *             so that additional initialization can be performed in an atomic manner.
 *
 *             If this fails for any reason (i.e. a duplicate name or no free slots)
 *             then the global table is unlocked inside this function prior to
 *             returning to the caller.
 *
 *             If OS_SUCCESS is returned, then the global lock MUST be either unlocked
 *             or converted to a different style lock (see OS_ObjectIdConvertLock) once
 *             the initialization of the new object is completed.
 *
 *             For any return code other than OS_SUCCESS, the caller must NOT
 *             manipulate the global lock at all.
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdAllocateNew(osal_objtype_t idtype, const char *name, OS_object_token_t *token)
{
    int32 return_code;

    /*
     * No new objects can be created after Shutdown request
     */
    if (OS_SharedGlobalVars.GlobalState == OS_SHUTDOWN_MAGIC_NUMBER)
    {
        return OS_ERR_INCORRECT_OBJ_STATE;
    }

    return_code = OS_ObjectIdTransactionInit(OS_LOCK_MODE_EXCLUSIVE, idtype, token);
    if (return_code != OS_SUCCESS)
    {
        return return_code;
    }

    /*
     * Check if an object of the same name already exists.
     * If so, a new object cannot be allocated.
     */
    if (name != NULL)
    {
        return_code = OS_ObjectIdFindNextMatch(OS_ObjectNameMatch, (void *)name, token);
    }
    else
    {
        return_code = OS_ERR_NAME_NOT_FOUND;
    }

    if (return_code == OS_SUCCESS)
    {
        return_code = OS_ERR_NAME_TAKEN;
    }
    else
    {
        return_code = OS_ObjectIdFindNextFree(token);
    }

    /* If allocation failed, abort the operation now - no ID was allocated.
     * After this point, if a future step fails, the allocated ID must be
     * released. */
    if (return_code != OS_SUCCESS)
    {
        OS_ObjectIdTransactionCancel(token);
        return return_code;
    }
    else
    {
        return_code = OS_NotifyEvent(OS_EVENT_RESOURCE_ALLOCATED, token->obj_id, NULL);
    }

    if (return_code == OS_SUCCESS)
    {
        return_code = OS_ObjectIdConvertToken(token);
    }

    if (return_code != OS_SUCCESS)
    {
        return_code = OS_ObjectIdFinalizeNew(return_code, token, NULL);
    }

    return return_code;
}

/*----------------------------------------------------------------

    Purpose: Transfer ownership of a token to another buffer
 ------------------------------------------------------------------*/
void OS_ObjectIdTransferToken(OS_object_token_t *token_from, OS_object_token_t *token_to)
{
    /* start with a simple copy */
    *token_to = *token_from;

    /*
     * nullify the old token, such that if release/cancel
     * is invoked it will have no effect (the real lock is
     * now on token_to).
     */
    token_from->lock_mode = OS_LOCK_MODE_NONE;
}

/*----------------------------------------------------------------

    Purpose: Start the process of iterating through OSAL objects
 ------------------------------------------------------------------*/
int32 OS_ObjectIdIteratorInit(OS_ObjectMatchFunc_t matchfunc, void *matcharg, osal_objtype_t objtype,
                              OS_object_iter_t *iter)
{
    iter->match = matchfunc;
    iter->arg   = matcharg;
    iter->limit = OS_GetMaxForObjectType(objtype);
    iter->base  = &OS_common_table[OS_GetBaseForObjectType(objtype)];

    return OS_ObjectIdTransactionInit(OS_LOCK_MODE_GLOBAL, objtype, &iter->token);
}

/*----------------------------------------------------------------

    Purpose: Match function to iterate only active objects
 ------------------------------------------------------------------*/
bool OS_ObjectFilterActive(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    return OS_ObjectIdDefined(obj->active_id);
}

/*----------------------------------------------------------------

    Purpose: Start the process of iterating through OSAL objects
 ------------------------------------------------------------------*/
int32 OS_ObjectIdIterateActive(osal_objtype_t objtype, OS_object_iter_t *iter)
{
    return OS_ObjectIdIteratorInit(OS_ObjectFilterActive, NULL, objtype, iter);
}

/*----------------------------------------------------------------

    Purpose: Move iterator to the next entry
 ------------------------------------------------------------------*/
bool OS_ObjectIdIteratorGetNext(OS_object_iter_t *iter)
{
    OS_common_record_t *record;
    bool                got_next;

    got_next           = false;
    iter->token.obj_id = OS_OBJECT_ID_UNDEFINED;

    do
    {
        ++iter->token.obj_idx;
        if (iter->token.obj_idx >= iter->limit)
        {
            break;
        }

        record = OS_OBJECT_TABLE_GET(iter->base, iter->token);
        if (iter->match == NULL || iter->match(iter->arg, &iter->token, record))
        {
            iter->token.obj_id = record->active_id;
            got_next           = true;
        }
    } while (!got_next);

    return got_next;
}

/*----------------------------------------------------------------

    Purpose: Release iterator resources
 ------------------------------------------------------------------*/
void OS_ObjectIdIteratorDestroy(OS_object_iter_t *iter)
{
    OS_ObjectIdTransactionCancel(&iter->token);
}

/*----------------------------------------------------------------

    Purpose: Call a handler function on an iterator object ID
 ------------------------------------------------------------------*/
int32 OS_ObjectIdIteratorProcessEntry(OS_object_iter_t *iter, int32 (*func)(osal_id_t, void *))
{
    int32 status;

    /*
     * This needs to temporarily unlock the global,
     * call the handler function, then re-lock.
     */
    OS_Unlock_Global(&iter->token);
    status = func(OS_ObjectIdFromToken(&iter->token), iter->arg);
    OS_Lock_Global(&iter->token);

    return status;
}

/*
 *********************************************************************************
 *          PUBLIC API (these functions may be called externally)
 *********************************************************************************
 */

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ConvertToArrayIndex(osal_id_t object_id, osal_index_t *ArrayIndex)
{
    /* pass to conversion routine with undefined type */
    return OS_ObjectIdToArrayIndex(OS_OBJECT_TYPE_UNDEFINED, object_id, ArrayIndex);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_ForEachObject(osal_id_t creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg)
{
    osal_objtype_t idtype;

    for (idtype = 0; idtype < OS_OBJECT_TYPE_USER; ++idtype)
    {
        OS_ForEachObjectOfType(idtype, creator_id, callback_ptr, callback_arg);
    }
}

/*-----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
void OS_ForEachObjectOfType(osal_objtype_t idtype, osal_id_t creator_id, OS_ArgCallback_t callback_ptr,
                            void *callback_arg)
{
    OS_object_iter_t    iter;
    OS_creator_filter_t filter;

    filter.creator_id    = creator_id;
    filter.user_callback = callback_ptr;
    filter.user_arg      = callback_arg;

    if (OS_ObjectIdIteratorInit(OS_ForEachFilterCreator, &filter, idtype, &iter) == OS_SUCCESS)
    {
        while (OS_ObjectIdIteratorGetNext(&iter))
        {
            OS_ObjectIdIteratorProcessEntry(&iter, OS_ForEachDoCallback);
        }

        OS_ObjectIdIteratorDestroy(&iter);
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
osal_objtype_t OS_IdentifyObject(osal_id_t object_id)
{
    return OS_ObjectIdToType_Impl(object_id);
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_GetResourceName(osal_id_t object_id, char *buffer, size_t buffer_size)
{
    OS_common_record_t *record;
    int32               return_code;
    size_t              name_len;
    OS_object_token_t   token;

    /* sanity check the passed-in buffer and size */
    OS_CHECK_POINTER(buffer);
    OS_CHECK_SIZE(buffer_size);

    /*
     * Initially set the output string to empty.
     * This avoids undefined behavior in case the function fails
     * and the caller does not check the return code.
     */
    buffer[0] = 0;

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, OS_ObjectIdToType_Impl(object_id), object_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_ObjectIdGlobalFromToken(&token);

        if (record->name_entry != NULL)
        {
            name_len = OS_strnlen(record->name_entry, buffer_size);
            if (buffer_size <= name_len)
            {
                /* indicates the name does not fit into supplied buffer */
                return_code = OS_ERR_NAME_TOO_LONG;
                name_len    = buffer_size - 1;
            }
            memcpy(buffer, record->name_entry, name_len);
            buffer[name_len] = 0;
        }

        OS_ObjectIdRelease(&token);
    }

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ObjectIdToArrayIndex(osal_objtype_t idtype, osal_id_t object_id, osal_index_t *ArrayIndex)
{
    uint32         max_id;
    uint32         obj_index;
    osal_objtype_t actual_type;
    int32          return_code;

    /* Check Parameters */
    OS_CHECK_POINTER(ArrayIndex);

    obj_index   = OS_ObjectIdToSerialNumber_Impl(object_id);
    actual_type = OS_ObjectIdToType_Impl(object_id);

    /*
     * If requested by the caller, enforce that the ID is of the correct type.
     * If the caller passed OS_OBJECT_TYPE_UNDEFINED, then anything is allowed.
     */
    if (idtype != OS_OBJECT_TYPE_UNDEFINED && actual_type != idtype)
    {
        return_code = OS_ERR_INVALID_ID;
    }
    else
    {
        max_id = OS_GetMaxForObjectType(actual_type);
        if (max_id == 0)
        {
            return_code = OS_ERR_INVALID_ID;
        }
        else
        {
            return_code = OS_SUCCESS;
            *ArrayIndex = OSAL_INDEX_C(obj_index % max_id);
        }
    }

    return return_code;
}
