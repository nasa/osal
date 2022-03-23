/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "os-shared-countsem.h"

OS_count_sem_internal_record_t OS_count_sem_table[OS_MAX_COUNT_SEMAPHORES];
