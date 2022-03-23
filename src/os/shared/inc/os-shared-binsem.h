/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_BINSEM_H
#define OS_SHARED_BINSEM_H

#include "osapi-binsem.h"
#include "os-shared-globaldefs.h"

/* other objects that have only an API name and no other data */
typedef struct
{
    char obj_name[OS_MAX_API_NAME];
} OS_bin_sem_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_bin_sem_internal_record_t OS_bin_sem_table[OS_MAX_BIN_SEMAPHORES];

/****************************************************************************************
                 SEMAPHORE API LOW-LEVEL IMPLEMENTATION FUNCTIONS
  ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_BinSemAPI_Init

   Purpose: Initialize the OS-independent layer for binary semaphores

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_BinSemAPI_Init(void);

/*----------------------------------------------------------------
   Function: OS_BinSemCreate_Impl

    Purpose: Prepare/allocate OS resources for a binary semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_BinSemCreate_Impl(const OS_object_token_t *token, uint32 sem_initial_value, uint32 options);

/*----------------------------------------------------------------
   Function: OS_BinSemFlush_Impl

    Purpose: Unblock all tasks waiting on the binary semaphore.
             Does not change the semaphore value.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_BinSemFlush_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_BinSemGive_Impl

    Purpose: Release the semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_BinSemGive_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_BinSemTake_Impl

    Purpose: Acquire the semaphore
             Block the calling task if the semaphore is 0.

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_BinSemTake_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_BinSemTimedWait_Impl

    Purpose: Acquire the semaphore, with a time limit

    Returns: OS_SUCCESS on success, or relevant error code
             OS_SEM_TIMEOUT must be returned if the time limit was reached
 ------------------------------------------------------------------*/
int32 OS_BinSemTimedWait_Impl(const OS_object_token_t *token, uint32 msecs);

/*----------------------------------------------------------------
   Function: OS_BinSemDelete_Impl

    Purpose: Free the OS resources associated with the binary semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_BinSemDelete_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_BinSemGetInfo_Impl

    Purpose: Obtain OS-specific information about the semaphore

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_BinSemGetInfo_Impl(const OS_object_token_t *token, OS_bin_sem_prop_t *bin_prop);

#endif /* OS_SHARED_BINSEM_H */
