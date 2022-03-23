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
 *
 * Declarations and prototypes for select abstraction
 */

#ifndef OSAPI_SELECT_H
#define OSAPI_SELECT_H

#include "osconfig.h"
#include "common_types.h"

/**
 * @brief An abstract structure capable of holding several OSAL IDs
 *
 * This is part of the select API and is manipulated using the
 * related API calls.  It should not be modified directly by applications.
 *
 * Note: Math is to determine uint8 array size needed to represent
 *       single bit OS_MAX_NUM_OPEN_FILES objects, + 7 rounds up
 *       and 8 is the size of uint8.
 *
 * @sa OS_SelectFdZero(), OS_SelectFdAdd(), OS_SelectFdClear(), OS_SelectFdIsSet()
 */
typedef struct
{
    uint8 object_ids[(OS_MAX_NUM_OPEN_FILES + 7) / 8];
} OS_FdSet;

/**
 * @brief For the OS_SelectSingle() function's in/out StateFlags parameter,
 * the state(s) of the stream and the result of the select is a combination
 * of one or more of these states.
 *
 * @sa OS_SelectSingle()
 */
typedef enum
{
    OS_STREAM_STATE_BOUND     = 0x01, /**< @brief whether the stream is bound     */
    OS_STREAM_STATE_CONNECTED = 0x02, /**< @brief whether the stream is connected */
    OS_STREAM_STATE_READABLE  = 0x04, /**< @brief whether the stream is readable  */
    OS_STREAM_STATE_WRITABLE  = 0x08, /**< @brief whether the stream is writable  */
} OS_StreamState_t;

/** @defgroup OSAPISelect OSAL Select APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Wait for events across multiple file handles
 *
 * Wait for any of the given sets of IDs to become readable or writable
 *
 * This function will block until any of the following occurs:
 *  - At least one OSAL ID in the ReadSet is readable
 *  - At least one OSAL ID in the WriteSet is writable
 *  - The timeout has elapsed
 *
 * The sets are input/output parameters.  On entry, these indicate the
 * file handle(s) to wait for.  On exit, these are set to the actual
 * file handle(s) that have activity.
 *
 * If the timeout occurs this returns an error code and all output sets
 * should be empty.
 *
 * @note This does not lock or otherwise protect the file handles in the
 * given sets.  If a filehandle supplied via one of the FdSet arguments
 * is closed or modified by another while this function is in progress,
 * the results are undefined.  Because of this limitation, it is recommended
 * to use OS_SelectSingle() whenever possible.
 *
 * @param[in,out] ReadSet  Set of handles to check/wait to become readable
 * @param[in,out] WriteSet Set of handles to check/wait to become writable
 * @param[in] msecs Indicates the timeout. Positive values will wait up to that many milliseconds. Zero will not wait
 * (poll). Negative values will wait forever (pend)
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS If any handle in the ReadSet or WriteSet is readable or writable, respectively
 * @retval #OS_ERROR_TIMEOUT If no handles in the ReadSet or WriteSet became readable or writable within the timeout
 * @retval #OS_ERR_OPERATION_NOT_SUPPORTED if a specified handle does not support select
 * @retval #OS_ERR_INVALID_ID if no valid handles were contained in the ReadSet/WriteSet
 */
int32 OS_SelectMultiple(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Wait for events on a single file handle
 *
 * Wait for a single OSAL filehandle to change state
 *
 * This function can be used to wait for a single OSAL stream ID
 * to become readable or writable.   On entry, the "StateFlags"
 * parameter should be set to the desired state (OS_STREAM_STATE_READABLE
 * and/or OS_STREAM_STATE_WRITABLE) and upon return the flags
 * will be set to the state actually detected.
 *
 * As this operates on a single ID, the filehandle is protected
 * during this call, such that another thread accessing the same
 * handle will return an error.  However, it is important to note that
 * once the call returns then other threads may then also read/write
 * and affect the state before the current thread can service it.
 *
 * To mitigate this risk the application may prefer to use
 * the OS_TimedRead/OS_TimedWrite calls.
 *
 * @param[in] objid The handle ID to select on
 * @param[in,out] StateFlags State flag(s) (readable or writable) @nonnull
 * @param[in] msecs Indicates the timeout. Positive values will wait up to that many milliseconds. Zero will not wait
 * (poll). Negative values will wait forever (pend)
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS If the handle is readable and/or writable, as requested
 * @retval #OS_ERROR_TIMEOUT If the handle did not become readable or writable within the timeout
 * @retval #OS_INVALID_POINTER if argument is NULL
 * @retval #OS_ERR_INVALID_ID if the objid is not a valid handle
 */
int32 OS_SelectSingle(osal_id_t objid, uint32 *StateFlags, int32 msecs);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear a FdSet structure
 *
 * After this call the set will contain no OSAL IDs
 *
 * @param[out] Set Pointer to OS_FdSet object to clear @nonnull
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief #OS_SUCCESS
 * @retval #OS_INVALID_POINTER if argument is NULL
 */
int32 OS_SelectFdZero(OS_FdSet *Set);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Add an ID to an FdSet structure
 *
 * After this call the set will contain the given OSAL ID
 *
 * @param[in,out] Set Pointer to OS_FdSet object to operate on @nonnull
 * @param[in] objid The handle ID to add to the set
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief #OS_SUCCESS
 * @retval #OS_INVALID_POINTER if argument is NULL
 * @retval #OS_ERR_INVALID_ID if the objid is not a valid handle
 */
int32 OS_SelectFdAdd(OS_FdSet *Set, osal_id_t objid);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Clear an ID from an FdSet structure
 *
 * After this call the set will no longer contain the given OSAL ID
 *
 * @param[in,out] Set Pointer to OS_FdSet object to operate on @nonnull
 * @param[in] objid The handle ID to remove from the set
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief #OS_SUCCESS
 * @retval #OS_INVALID_POINTER if argument is NULL
 * @retval #OS_ERR_INVALID_ID if the objid is not a valid handle
 */
int32 OS_SelectFdClear(OS_FdSet *Set, osal_id_t objid);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Check if an FdSet structure contains a given ID
 *
 * @param[in] Set Pointer to OS_FdSet object to operate on @nonnull
 * @param[in] objid The handle ID to check for in the set
 *
 * @return Boolean set status
 * @retval true  FdSet structure contains ID
 * @retval false FDSet structure does not contain ID
 */
bool OS_SelectFdIsSet(const OS_FdSet *Set, osal_id_t objid);
/**@}*/

#endif /* OSAPI_SELECT_H */
