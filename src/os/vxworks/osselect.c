/*
 * File   : osselect.c
 *
 * Author : Joe Hickey
 *
 * Purpose: This file contains wrappers around the select() system call
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/

#include "os-vxworks.h"

#include <selectLib.h>

/*
 * Use the BSD-style select() API
 */
#include "../portable/os-impl-bsd-select.c"

