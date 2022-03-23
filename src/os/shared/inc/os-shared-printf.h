/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_PRINTF_H
#define OS_SHARED_PRINTF_H

#include "osapi-printf.h"
#include "os-shared-console.h"
#include "os-shared-globaldefs.h"

/****************************************************************************************
                 CONSOLE / DEBUG API LOW-LEVEL IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

/*----------------------------------------------------------------
   Function: OS_ConsoleOutput_Impl

    Purpose: Basic Console output implementation

   This function forwards the data from the console
   ring buffer into the actual output device/descriptor

   The data is already formatted, this just writes the characters.
 ------------------------------------------------------------------*/
void OS_ConsoleOutput_Impl(const OS_object_token_t *token);

#endif /* OS_SHARED_PRINTF_H */
