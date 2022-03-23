/**
 * \file
 *
 * General constants for OSAL that are shared across subsystems
 */

#ifndef OSAPI_CONSTANTS_H
#define OSAPI_CONSTANTS_H

#include "osconfig.h"
#include "common_types.h"

/*
** Defines for Queue Timeout parameters
*/
#define OS_PEND  (-1)
#define OS_CHECK (0)

/**
 * @brief Initializer for the osal_id_t type which will not match any valid value
 */
#define OS_OBJECT_ID_UNDEFINED ((osal_id_t) {0})

/**
 * @brief Constant that may be passed to OS_ForEachObject()/OS_ForEachObjectOfType() to match any
 * creator (i.e. get all objects)
 */
#define OS_OBJECT_CREATOR_ANY OS_OBJECT_ID_UNDEFINED

/**
 * @brief Maximum length of a local/native path name string
 *
 * This is a concatenation of the OSAL virtual path with the system
 * mount point or device name
 */
#define OS_MAX_LOCAL_PATH_LEN (OS_MAX_PATH_LEN + OS_FS_PHYS_NAME_LEN)

#endif /* OSAPI_CONSTANTS_H */
