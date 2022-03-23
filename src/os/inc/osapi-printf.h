/**
 * \file
 *
 * Declarations and prototypes for printf/console output
 */

#ifndef OSAPI_PRINTF_H
#define OSAPI_PRINTF_H

#include "osconfig.h"
#include "common_types.h"

/** @defgroup OSAPIPrintf OSAL Printf APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Abstraction for the system printf() call
 *
 * This function abstracts out the printf type statements. This is
 * useful for using OS- specific thats that will allow non-polled
 * print statements for the real time systems.
 *
 * Operates in a manner similar to the printf() call defined by the standard C
 * library and takes all the parameters and formatting options of printf.
 * This abstraction may implement additional buffering, if necessary,
 * to improve the real-time performance of the call.
 *
 * Strings (including terminator) longer than #OS_BUFFER_SIZE will be truncated.
 *
 * The output of this routine also may be dynamically enabled or disabled by
 * the OS_printf_enable() and OS_printf_disable() calls, respectively.
 *
 * @param[in] string Format string, followed by additional arguments
 */
void OS_printf(const char *string, ...) OS_PRINTF(1, 2);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief This function disables the output from OS_printf.
 */
void OS_printf_disable(void);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief This function enables the output from OS_printf.
 *
 */
void OS_printf_enable(void);
/**@}*/

#endif /* OSAPI_PRINTF_H */
