/**
 * \file   osselect.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains wrappers around the select() system call
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-posix.h"

/* Include the OS-specific header files necessary for select() operations */
#include <sys/select.h>


/****************************************************************************************
                                     COMMON ROUTINES
 After including the OS/C-library specific include files, the basic UNIX select API is used
 ****************************************************************************************/

/* Leverage the portable UNIX-style Select API for POSIX */
#include "../portable/os-impl-bsd-select.c"

