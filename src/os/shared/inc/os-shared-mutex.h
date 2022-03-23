/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_MUTEX_H
#define OS_SHARED_MUTEX_H

#include "osapi-mutex.h"
#include "os-shared-globaldefs.h"

typedef struct
{
    char      obj_name[OS_MAX_API_NAME];
    osal_id_t last_owner;
} OS_mutex_internal_record_t;

/*
 * These record types have extra information with each entry.  These tables are used
 * to share extra data between the common layer and the OS-specific implementation.
 */
extern OS_mutex_internal_record_t OS_mutex_table[OS_MAX_MUTEXES];

/*---------------------------------------------------------------------------------------
   Name: OS_MutexAPI_Init

   Purpose: Initialize the OS-independent layer for mutex objects

   returns: OS_SUCCESS on success, or relevant error code
---------------------------------------------------------------------------------------*/
int32 OS_MutexAPI_Init(void);

/*----------------------------------------------------------------
   Function: OS_MutSemCreate_Impl

    Purpose: Prepare/allocate OS resources for a mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemCreate_Impl(const OS_object_token_t *token, uint32 options);

/*----------------------------------------------------------------
   Function: OS_MutSemGive_Impl

    Purpose: Release the mutex, which must be owned by the caller

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemGive_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_MutSemTake_Impl

    Purpose: Acquire the mutex, blocking the caller as necessary

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemTake_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_MutSemDelete_Impl

    Purpose: Free the OS resources associated with a mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemDelete_Impl(const OS_object_token_t *token);

/*----------------------------------------------------------------
   Function: OS_MutSemGetInfo_Impl

    Purpose: Obtain OS-specific information about the mutex object

    Returns: OS_SUCCESS on success, or relevant error code
 ------------------------------------------------------------------*/
int32 OS_MutSemGetInfo_Impl(const OS_object_token_t *token, OS_mut_sem_prop_t *mut_prop);

#endif /* OS_SHARED_MUTEX_H */
