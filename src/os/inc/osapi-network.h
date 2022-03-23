/**
 * \file
 *
 * Declarations and prototypes for network subsystem
 */

#ifndef OSAPI_NETWORK_H
#define OSAPI_NETWORK_H

#include "osconfig.h"
#include "common_types.h"

/**
 * @defgroup OSAPINetwork OSAL Network ID APIs
 *
 * Provides some basic methods to query a network host name and ID
 *
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief  Gets the network ID of the local machine
 *
 * The ID is an implementation-defined value and may not be consistent
 * in meaning across different platform types.
 *
 * @note This API may be removed in a future version of OSAL due to
 *       inconsistencies between platforms.
 *
 * @return The ID or fixed value of -1 if the host id could not be found.
 *         Note it is not possible to differentiate between error codes and valid
 *         network IDs here. It is assumed, however, that -1 is never a valid ID.
 */
int32 OS_NetworkGetID(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Gets the local machine network host name
 *
 * If configured in the underlying network stack,
 * this function retrieves the local hostname of the system.
 *
 * @param[out]  host_name    Buffer to hold name information @nonnull
 * @param[in]   name_len     Maximum length of host name buffer @nonzero
 *
 * @return Execution status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_ERR_INVALID_SIZE if the name_len is zero
 * @retval #OS_INVALID_POINTER if the host_name is NULL
 */
int32 OS_NetworkGetHostName(char *host_name, size_t name_len);

/**@}*/

#endif /* OSAPI_NETWORK_H */
