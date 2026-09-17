/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
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
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_MUTEX_H
#define OS_IMPL_MUTEX_H

#include "osconfig.h"
#include <pthread.h>

/* Mutexes */
typedef struct
{
    pthread_mutex_t id;
} OS_impl_mutex_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_mutex_internal_record_t OS_impl_mutex_table[OS_MAX_MUTEXES];

/*
 * Robust mutex support, selected by OSAL_CONFIG_POSIX_ROBUST_MUTEX:
 * os-impl-posix-mutex-robust.c uses the POSIX.1-2008 robust mutex API,
 * os-impl-posix-mutex-no-robust.c leaves the mutex as it was.
 */

/**
 * Request the robust attribute on a mutex being created
 *
 * \param attr  The initialized attribute set the mutex will be created with
 * \return 0 on success, otherwise the pthread error code
 */
int OS_Posix_MutexAttrSetRobust(pthread_mutexattr_t *attr);

/**
 * Recover a mutex whose previous owner was deleted while holding it
 *
 * \param mutex        The mutex just passed to pthread_mutex_lock()
 * \param lock_status  The value pthread_mutex_lock() returned
 * \return lock_status unchanged unless it was EOWNERDEAD, in which case the
 *         result of restoring the mutex to a consistent state (0 on success)
 */
int OS_Posix_MutexRecoverOwnerDead(pthread_mutex_t *mutex, int lock_status);

#endif /* OS_IMPL_MUTEX_H */
