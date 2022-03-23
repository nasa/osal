/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-errors.h"

/* Nonzero/NULL entry required for full branch coverage */
const OS_ErrorTable_Entry_t OS_IMPL_ERROR_NAME_TABLE[] = {{-4444, "UT_ERROR"}, {-4445, NULL}, {0, NULL}};
