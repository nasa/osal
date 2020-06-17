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

#include "os-rtems.h"

#include <sys/select.h>


/****************************************************************************************
                                     COMMON ROUTINES
 After including the OS/C-library specific include files, the basic UNIX select API is used
 ****************************************************************************************/

/*
 * NOTE: the RTEMS select() implementation is very limited and only works on
 * network sockets (at least for release 4.11.2 this is the case).
 */
#include "../portable/os-impl-bsd-select.c"

