/**
 * \file
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-rtems.h"
#include "os-shared-errors.h"

const OS_ErrorTable_Entry_t OS_IMPL_ERROR_NAME_TABLE[] = {{0, NULL}};
