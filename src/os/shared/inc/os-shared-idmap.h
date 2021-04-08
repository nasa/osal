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
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_IDMAP_H
#define OS_SHARED_IDMAP_H

#include "osapi-idmap.h"
#include "os-shared-globaldefs.h"

#define OS_OBJECT_ID_RESERVED ((osal_id_t) {0xFFFFFFFF})

/*
 * This supplies a non-abstract definition of "OS_common_record_t"
 */
struct OS_common_record
{
    const char *name_entry;
    osal_id_t   active_id;
    osal_id_t   creator;
    uint16      refcount;
};

/*
 * Type of locking that should occur when checking IDs.
 */
typedef enum
{
    OS_LOCK_MODE_NONE,      /**< Quick ID validity check, does not lock global table at all (use with caution) */
    OS_LOCK_MODE_GLOBAL,    /**< Confirm ID match, and if successful, leave global table locked */
    OS_LOCK_MODE_REFCOUNT,  /**< Confirm ID match, increment refcount, and unlock global table.  ID is not changed. */
    OS_LOCK_MODE_EXCLUSIVE, /**< Confirm ID match AND refcount equal zero, then change ID to RESERVED value and unlock
                               global. */
    OS_LOCK_MODE_RESERVED   /**< Confirm ID is already set to RESERVED, otherwise like OS_LOCK_MODE_GLOBAL. */
} OS_lock_mode_t;

/*
 * A unique key value issued when obtaining a table lock, based on a
 * the a combination of the requesting task ID and a transaction ID
 */
typedef struct
{
    uint32 key_value;
} osal_key_t;

/*
 * Actual (non-abstract) definition of "OS_object_token_t"
 */
struct OS_object_token
{
    OS_lock_mode_t lock_mode;
    osal_key_t     lock_key;
    osal_objtype_t obj_type;
    osal_index_t   obj_idx;
    osal_id_t      obj_id;
};

/*
 * Macro to retrieve an entry from an object table, based on a token
 */
#define OS_OBJECT_TABLE_GET(tbl, tok) (&tbl[OS_ObjectIndexFromToken(&(tok))])

/*
 * Macro to clear a table entry and reset its name
 */
#define OS_OBJECT_INIT(tok, ref, namefield, nameval)                   \
    {                                                                  \
        memset(ref, 0, sizeof(*ref));                                  \
        strncpy(ref->namefield, nameval, sizeof(ref->namefield) - 1);  \
        OS_ObjectIdGlobalFromToken(&tok)->name_entry = ref->namefield; \
    }

/*
 * A function to perform arbitrary record matching.
 *
 * This can be used to find a record based on criteria other than the ID,
 * such as the name or any other record within the structure.
 *
 * Returns true if the id/obj matches the reference, false otherwise.
 */
typedef bool (*OS_ObjectMatchFunc_t)(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj);

/*
 * State object associated with an object iterator
 */
typedef struct
{
    OS_common_record_t * base;
    OS_ObjectMatchFunc_t match;
    void *               arg;
    osal_index_t         limit;
    OS_object_token_t    token;
} OS_object_iter_t;

/*
 * Global instantiations
 */
/* The following are quick-access pointers to the various sections of the common table */
extern OS_common_record_t *const OS_global_task_table;
extern OS_common_record_t *const OS_global_queue_table;
extern OS_common_record_t *const OS_global_bin_sem_table;
extern OS_common_record_t *const OS_global_count_sem_table;
extern OS_common_record_t *const OS_global_mutex_table;
extern OS_common_record_t *const OS_global_stream_table;
extern OS_common_record_t *const OS_global_dir_table;
extern OS_common_record_t *const OS_global_timebase_table;
extern OS_common_record_t *const OS_global_timecb_table;
extern OS_common_record_t *const OS_global_module_table;
extern OS_common_record_t *const OS_global_filesys_table;
extern OS_common_record_t *const OS_global_console_table;

/****************************************************************************************
                                ID MAPPING FUNCTIONS
  ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_ObjectIdInit

   Purpose: Initialize the OS-independent layer for object ID management

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_ObjectIdInit(void);

/*
 * Table locking and unlocking for global objects can be done at the shared code
 * layer but the actual implementation is OS-specific
 */

/*----------------------------------------------------------------
   Function: OS_Lock_Global

    Purpose: Locks the global table identified by "idtype"

   Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
void OS_Lock_Global(OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_Lock_Global

    Purpose: Locks the global table identified by "idtype"

   Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
void OS_Lock_Global_Impl(osal_objtype_t idtype);

/*----------------------------------------------------------------
   Function: OS_Unlock_Global

    Purpose: Unlocks the global table identified by "idtype"

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
void OS_Unlock_Global(OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_Unlock_Global

    Purpose: Unlocks the global table identified by "idtype"

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
void OS_Unlock_Global_Impl(osal_objtype_t idtype);

/*----------------------------------------------------------------

   Function: OS_WaitForStateChange

    Purpose: Waits for a change in the global table identified by "idtype"

   NOTE: The table must be already "owned" (via OS_Lock_Global) by the calling
   at the time this function is invoked.  The lock is released and re-acquired
   before returning from this function.

  -----------------------------------------------------------------*/
void OS_WaitForStateChange(OS_object_token_t *token, uint32 attempts);

/*----------------------------------------------------------------

   Function: OS_WaitForStateChange_Impl

   Purpose: Block the caller until some sort of change event
   has occurred for the given object type, such as a record changing
   state i.e. the acquisition or release of a lock/refcount from
   another thread.

   It is not guaranteed what, if any, state change has actually
   occured when this function returns.  This may be implement as
   a simple OS_TaskDelay().

 ------------------------------------------------------------------*/
void OS_WaitForStateChange_Impl(osal_objtype_t objtype, uint32 attempts);

/*
   Function prototypes for routines implemented in common layers but private to OSAL

   These implement the basic OSAL ObjectID patterns - that is a 32-bit number that
   is opaque externally, but internally identifies a specific type of object and
   corresponding index within the local tables.
 */

/*----------------------------------------------------------------
   Function: OS_ObjectIdToSerialNumber

    Purpose: Obtain the serial number component of a generic OSAL Object ID
 ------------------------------------------------------------------*/
static inline uint32 OS_ObjectIdToSerialNumber_Impl(osal_id_t id)
{
    return (OS_ObjectIdToInteger(id) & OS_OBJECT_INDEX_MASK);
}

/*----------------------------------------------------------------
   Function: OS_ObjectIdToType

    Purpose: Obtain the object type component of a generic OSAL Object ID
 ------------------------------------------------------------------*/
static inline osal_objtype_t OS_ObjectIdToType_Impl(osal_id_t id)
{
    return (OS_ObjectIdToInteger(id) >> OS_OBJECT_TYPE_SHIFT);
}

/*----------------------------------------------------------------
   Function: OS_ObjectIdCompose

    Purpose: Convert an object serial number and resource type into an external 32-bit OSAL ID
 ------------------------------------------------------------------*/
static inline void OS_ObjectIdCompose_Impl(osal_objtype_t idtype, uint32 idserial, osal_id_t *result)
{
    *result = OS_ObjectIdFromInteger((idtype << OS_OBJECT_TYPE_SHIFT) | idserial);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Check if an object ID represents a valid/active value.
 *
 * This tests that the ID value is within the range specifically used by
 * valid OSAL IDs. This is smaller than the set of defined IDs.
 *
 * For example, the value of OS_OBJECT_ID_RESERVED is defined but not valid.
 * So while OS_ObjectIdDefined() will match entries being actively created or
 * deleted, OS_ObjectIdIsValid() will not.
 *
 * @param[in]   object_id The object ID
 * @returns     true if table entry is valid
 */
static inline bool OS_ObjectIdIsValid(osal_id_t object_id)
{
    osal_objtype_t objtype = OS_ObjectIdToType_Impl(object_id);
    return (objtype > OS_OBJECT_TYPE_UNDEFINED && objtype < OS_OBJECT_TYPE_USER);
}

/*----------------------------------------------------------------
   Function: OS_GetMaxForObjectType

    Purpose: Obtains the maximum number of objects for "idtype" in the global table

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
uint32 OS_GetMaxForObjectType(osal_objtype_t idtype);

/*----------------------------------------------------------------
   Function: OS_GetBaseForObjectType

    Purpose: Obtains the base object number for "idtype" in the global table

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
uint32 OS_GetBaseForObjectType(osal_objtype_t idtype);

/*----------------------------------------------------------------
   Function: OS_ObjectIndexFromToken

    Purpose: Gets the index referenced by the token

    Returns: None
 ------------------------------------------------------------------*/
static inline osal_objtype_t OS_ObjectTypeFromToken(const OS_object_token_t *token)
{
    return token->obj_type;
}

/*----------------------------------------------------------------
   Function: OS_ObjectIndexFromToken

    Purpose: Gets the index referenced by the token

    Returns: None
 ------------------------------------------------------------------*/
static inline osal_index_t OS_ObjectIndexFromToken(const OS_object_token_t *token)
{
    return token->obj_idx;
}

/*----------------------------------------------------------------
   Function: OS_ObjectIdFromToken

    Purpose: Gets the object ID referenced by the token

    Returns: None
 ------------------------------------------------------------------*/
static inline osal_id_t OS_ObjectIdFromToken(const OS_object_token_t *token)
{
    return token->obj_id;
}

/*----------------------------------------------------------------
   Function: OS_ObjectIdGlobalFromToken

    Purpose: Obtains the global record corresponding to the token

    Returns: Pointer to global object
 ------------------------------------------------------------------*/
OS_common_record_t *OS_ObjectIdGlobalFromToken(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdTransactionInit

    Purpose: Initiates a transaction by obtaining the global table lock
             and preparing the object token value

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdTransactionInit(OS_lock_mode_t lock_mode, osal_objtype_t idtype, OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdTransactionCancel

    Purpose: Cancels/Releases the lock obtained by OS_ObjectIdTransactionInit()
             without making any modification to global IDs.

    Returns: None
 ------------------------------------------------------------------*/
void OS_ObjectIdTransactionCancel(OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdTransactionFinish

    Purpose: Releases the lock obtained by OS_ObjectIdTransactionInit()
             with an optional synchronized ID update for new/deleted IDs.

    Returns: None
 ------------------------------------------------------------------*/
void OS_ObjectIdTransactionFinish(OS_object_token_t *token, const osal_id_t *final_id);

/*----------------------------------------------------------------
   Function: OS_ObjectIdConvertToken

    Purpose: Converts a token from OS_ObjectIdTransactionInit() to the
             type that was requested by the user.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdConvertToken(OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdFindByName

    Purpose: Finds an entry in the global resource table matching the given name

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdFindByName(osal_objtype_t idtype, const char *name, osal_id_t *object_id);

/*----------------------------------------------------------------
   Function: OS_ObjectIdGetBySearch

    Purpose: Find and lock an entry in the global resource table
             Search is performed using a user-specified match function
             (Allows searching for items by arbitrary keys)

   Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdGetBySearch(OS_lock_mode_t lock_mode, osal_objtype_t idtype, OS_ObjectMatchFunc_t MatchFunc, void *arg,
                             OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdGetByName

    Purpose: Find and lock an entry in the global resource table
             Search is performed using a name match function

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdGetByName(OS_lock_mode_t lock_mode, osal_objtype_t idtype, const char *name, OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdGetById

    Purpose: Find and lock an entry in the global resource table
             Lookup is performed by ID value (no searching required)

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdGetById(OS_lock_mode_t lock_mode, osal_objtype_t idtype, osal_id_t id, OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdRelease

    Purpose: Releases (unlocks) the object token previously obtained using
             OS_ObjectIdGetById() or OS_ObjectIdGetBySearch().

    Returns: none
 ------------------------------------------------------------------*/
void OS_ObjectIdRelease(OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdTransferToken

    Purpose: Transfers ownership of a object token without unlocking/releasing.
             The original token will become benign and the new token becomes active.

    Returns: none
 ------------------------------------------------------------------*/
void OS_ObjectIdTransferToken(OS_object_token_t *token_from, OS_object_token_t *token_to);

/*----------------------------------------------------------------
   Function: OS_ObjectIdAllocateNew

    Purpose: Issue a new object ID of the given type and associate with the given name
             The array index (0-based) and global record pointers are output back to the caller
             The table will be left in a "locked" state to allow further initialization
             The OS_ObjectIdFinalizeNew() function must be called to complete the operation

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdAllocateNew(osal_objtype_t idtype, const char *name, OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdFinalizeNew

    Purpose: Completes the operation initiated by OS_ObjectIdAllocateNew()
             If the operation was successful, the final OSAL ID is returned
             If the operation was unsuccessful, the ID is deleted and returned to the pool.
             The global table is unlocked for future operations

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdFinalizeNew(int32 operation_status, OS_object_token_t *token, osal_id_t *outid);

/*----------------------------------------------------------------
   Function: OS_ObjectIdFinalizeDelete

    Purpose: Completes a delete operation
             If the operation was successful, the OSAL ID is deleted and returned to the pool
             If the operation was unsuccessful, no operation is performed.
             The global table is unlocked for future operations

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdFinalizeDelete(int32 operation_status, OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_ObjectIdIteratorInit

    Purpose: Initialize a generic object iterator of the given type.
             Note This obtains and holds a global lock on the internal table, so
             this call must be followed by a call to OS_ObjectIdIteratorDestroy()

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdIteratorInit(OS_ObjectMatchFunc_t matchfunc, void *matcharg, osal_objtype_t objtype,
                              OS_object_iter_t *iter);

/*----------------------------------------------------------------
   Function: OS_ObjectIdIterateActive

    Purpose: Initialize a object iterator of the given type that will
             return only active/valid OSAL objects.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_ObjectIdIterateActive(osal_objtype_t objtype, OS_object_iter_t *iter);

/*----------------------------------------------------------------
   Function: OS_ObjectIdIteratorGetNext

    Purpose: Move then token to the next matching iterator entry

    Returns: true if successful, false if at last entry/end of table
 ------------------------------------------------------------------*/
bool OS_ObjectIdIteratorGetNext(OS_object_iter_t *iter);

/*----------------------------------------------------------------
   Function: OS_ObjectIdIteratorDestroy

    Purpose: Releases an iterator from OS_ObjectIdIteratorInit()

    Returns: None
 ------------------------------------------------------------------*/
void OS_ObjectIdIteratorDestroy(OS_object_iter_t *iter);

/*----------------------------------------------------------------
   Function: OS_ObjectIdIteratorRef

    Purpose: Gets the token indicating current iterator position
             The returned token can be used to access the relevant entry

    Returns: None
 ------------------------------------------------------------------*/
static inline const OS_object_token_t *OS_ObjectIdIteratorRef(OS_object_iter_t *iter)
{
    return &iter->token;
}

/*----------------------------------------------------------------
   Function: OS_ObjectIdIteratorProcessEntry

    Purpose: Calls a function using the ID of the entry from the iterator

    Returns: None
 ------------------------------------------------------------------*/
int32 OS_ObjectIdIteratorProcessEntry(OS_object_iter_t *iter, int32 (*func)(osal_id_t, void *));

/*
 * Internal helper functions
 * These are not normally called outside this unit, but need
 * to be exposed for unit testing.
 */
bool  OS_ObjectFilterActive(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj);
bool  OS_ObjectNameMatch(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj);
int32 OS_ObjectIdFindNextMatch(OS_ObjectMatchFunc_t MatchFunc, void *arg, OS_object_token_t *token);
int32 OS_ObjectIdFindNextFree(OS_object_token_t *token);

#endif /* OS_SHARED_IDMAP_H */
