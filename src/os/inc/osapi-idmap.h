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
 * Declarations and prototypes for object IDs
 */

#ifndef OSAPI_IDMAP_H
#define OSAPI_IDMAP_H

#include "osconfig.h"
#include "common_types.h"

/* Defines constants for making object ID's unique */
#define OS_OBJECT_INDEX_MASK 0xFFFF /**< @brief Object index mask */
#define OS_OBJECT_TYPE_SHIFT 16     /**< @brief Object type shift */

/** @defgroup OSObjectTypes OSAL Object Type Defines
 * @{
 */
#define OS_OBJECT_TYPE_UNDEFINED   0x00 /**< @brief Object type undefined */
#define OS_OBJECT_TYPE_OS_TASK     0x01 /**< @brief Object task type */
#define OS_OBJECT_TYPE_OS_QUEUE    0x02 /**< @brief Object queue type */
#define OS_OBJECT_TYPE_OS_COUNTSEM 0x03 /**< @brief Object counting semaphore type */
#define OS_OBJECT_TYPE_OS_BINSEM   0x04 /**< @brief Object binary semaphore type */
#define OS_OBJECT_TYPE_OS_MUTEX    0x05 /**< @brief Object mutex type */
#define OS_OBJECT_TYPE_OS_STREAM   0x06 /**< @brief Object stream type */
#define OS_OBJECT_TYPE_OS_DIR      0x07 /**< @brief Object directory type */
#define OS_OBJECT_TYPE_OS_TIMEBASE 0x08 /**< @brief Object timebase type */
#define OS_OBJECT_TYPE_OS_TIMECB   0x09 /**< @brief Object timer callback type */
#define OS_OBJECT_TYPE_OS_MODULE   0x0A /**< @brief Object module type */
#define OS_OBJECT_TYPE_OS_FILESYS  0x0B /**< @brief Object file system type */
#define OS_OBJECT_TYPE_OS_CONSOLE  0x0C /**< @brief Object console type */
#define OS_OBJECT_TYPE_USER        0x10 /**< @brief Object user type */
/**@}*/

/** @defgroup OSAPIObjUtil OSAL Object ID Utility APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain an integer value corresponding to an object ID
 *
 * Obtains an integer representation of an object id, generally
 * for the purpose of printing to the console or system logs.
 *
 * The returned value is of the type "unsigned long" for direct use with
 * printf-style functions. It is recommended to use the "%lx" conversion
 * specifier as the hexidecimal encoding clearly delineates the internal fields.
 *
 * @note This provides the raw integer value and is _not_ suitable for use
 * as an array index, as the result is not zero-based.  See the
 * OS_ConvertToArrayIndex() to obtain a zero-based index value.
 *
 * @param[in]   object_id The object ID
 * @returns     integer value representation of object ID
 *
 * @hidecallgraph
 * @hidecallergraph
 */
static inline unsigned long OS_ObjectIdToInteger(osal_id_t object_id)
{
    return object_id;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain an osal ID corresponding to an integer value
 *
 * Provides the inverse of OS_ObjectIdToInteger().  Reconstitutes the original
 * osal_id_t type from an integer representation.
 *
 * @param[in]   value   The integer representation of an OSAL ID
 * @returns     The ID value converted to an osal_id_t
 *
 * @hidecallgraph
 * @hidecallergraph
 */
static inline osal_id_t OS_ObjectIdFromInteger(unsigned long value)
{
    return (osal_id_t)value;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Check two OSAL object ID values for equality
 *
 * The OSAL ID values should be treated as abstract values by applications, and not
 * directly manipulated using standard C operators.
 *
 * This checks two values for equality, replacing the "==" operator.
 *
 * @param[in]   object_id1 The first object ID
 * @param[in]   object_id2 The second object ID
 * @returns     true if the object IDs are equal
 *
 * @hidecallgraph
 * @hidecallergraph
 */
static inline bool OS_ObjectIdEqual(osal_id_t object_id1, osal_id_t object_id2)
{
    return (object_id1 == object_id2);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Check if an object ID is defined.
 *
 * The OSAL ID values should be treated as abstract values by applications, and not
 * directly manipulated using standard C operators.
 *
 * This returns false if the ID is NOT a defined resource (i.e. free/empty/invalid).
 *
 * @note OS_ObjectIdDefined(OS_OBJECT_ID_UNDEFINED) is always guaranteed to be false.
 *
 * @param[in]   object_id The first object ID
 *
 * @hidecallgraph
 * @hidecallergraph
 */
static inline bool OS_ObjectIdDefined(osal_id_t object_id)
{
    return (object_id != 0);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain the name of an object given an arbitrary object ID
 *
 * All OSAL resources generally have a name associated with them.  This
 * allows application code to retrieve the name of any valid OSAL object ID.
 *
 * @param[in]  object_id The object ID to operate on
 * @param[out] buffer Buffer in which to store the name
 * @param[in]  buffer_size Size of the output storage buffer
 *
 * @returns #OS_SUCCESS if successful
 *          #OS_ERR_INVALID_ID if the passed-in ID is not a valid OSAL ID
 *          #OS_INVALID_POINTER if the passed-in buffer is invalid
 *          #OS_ERR_NAME_TOO_LONG if the name will not fit in the buffer provided
 */
int32 OS_GetResourceName(osal_id_t object_id, char *buffer, size_t buffer_size);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Obtain the type of an object given an arbitrary object ID
 *
 * Given an arbitrary object ID, get the type of the object
 *
 * @param[in] object_id The object ID to operate on
 *
 * @return The object type portion of the object_id, see @ref OSObjectTypes for
 *         expected values
 */
osal_objtype_t OS_IdentifyObject(osal_id_t object_id);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Converts an abstract ID into a number suitable for use as an array index.
 *
 * This will return a unique zero-based integer number in the range of [0,MAX) for
 * any valid object ID.  This may be used by application code as an array index
 * for indexing into local tables.
 *
 * @note This does NOT verify the validity of the ID, that is left to the caller.
 * This is only the conversion logic.
 *
 * This routine accepts any object type, and returns a value based on the
 * maximum number of objects for that type.  This is equivalent to invoking
 * OS_ObjectIdToArrayIndex() with the idtype set to OS_OBJECT_TYPE_UNDEFINED.
 *
 * @sa OS_ObjectIdToArrayIndex
 *
 * @param[in]  object_id    The object ID to operate on
 * @param[out] *ArrayIndex  The Index to return
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS                 @copybrief OS_SUCCESS
 * @retval #OS_ERR_INCORRECT_OBJ_TYPE  @copybrief OS_ERR_INCORRECT_OBJ_TYPE
 */
int32 OS_ConvertToArrayIndex(osal_id_t object_id, osal_index_t *ArrayIndex);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Converts an abstract ID into a number suitable for use as an array index.
 *
 * This will return a unique zero-based integer number in the range of [0,MAX) for
 * any valid object ID.  This may be used by application code as an array index
 * for indexing into local tables.
 *
 * This routine operates on a specific object type, and returns a value based on the
 * maximum number of objects for that type.
 *
 * If the idtype is passed as #OS_OBJECT_TYPE_UNDEFINED, then object type verification
 * is skipped and any object ID will be accepted and converted to an index.  In this
 * mode, the range of the output depends on the actual passed-in object type.
 *
 * If the idtype is passed as any other value, the passed-in ID value is first
 * confirmed to be the correct type.  This check will guarantee that the output
 * is within an expected range; for instance, if the type is passed as
 * #OS_OBJECT_TYPE_OS_TASK, then the output index is guaranteed to be between 0 and
 * #OS_MAX_TASKS-1 after successful conversion.
 *
 * @param[in]  idtype       The object type to convert
 * @param[in]  object_id    The object ID to operate on
 * @param[out] *ArrayIndex  The Index to return
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS                 @copybrief OS_SUCCESS
 * @retval #OS_ERR_INCORRECT_OBJ_TYPE  @copybrief OS_ERR_INCORRECT_OBJ_TYPE
 * */
int32 OS_ObjectIdToArrayIndex(osal_objtype_t idtype, osal_id_t object_id, osal_index_t *ArrayIndex);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief call the supplied callback function for all valid object IDs
 *
 * Loops through all defined OSAL objects of all types and calls callback_ptr on each one
 * If creator_id is nonzero then only objects with matching creator id are processed.
 *
 * @param[in]  creator_id   Filter objects to those created by a specific task
 *                          This may be passed as OS_OBJECT_CREATOR_ANY to return all objects
 * @param[in]  callback_ptr Function to invoke for each matching object ID
 * @param[in]  callback_arg Opaque Argument to pass to callback function
 */
void OS_ForEachObject(osal_id_t creator_id, OS_ArgCallback_t callback_ptr, void *callback_arg);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief call the supplied callback function for valid object IDs of a specific type
 *
 * Loops through all defined OSAL objects of a specific type and calls callback_ptr on each one
 * If creator_id is nonzero then only objects with matching creator id are processed.
 *
 * @param[in]  objtype      The type of objects to iterate
 * @param[in]  creator_id   Filter objects to those created by a specific task
 *                          This may be passed as OS_OBJECT_CREATOR_ANY to return all objects
 * @param[in]  callback_ptr Function to invoke for each matching object ID
 * @param[in]  callback_arg Opaque Argument to pass to callback function
 */
void OS_ForEachObjectOfType(osal_objtype_t objtype, osal_id_t creator_id, OS_ArgCallback_t callback_ptr,
                            void *callback_arg);

/**@}*/

#endif /* OSAPI_IDMAP_H */
