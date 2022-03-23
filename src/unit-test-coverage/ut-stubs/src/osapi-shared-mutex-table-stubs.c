/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "os-shared-mutex.h"

OS_mutex_internal_record_t OS_mutex_table[OS_MAX_MUTEXES];
