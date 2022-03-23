/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "os-shared-binsem.h"

OS_bin_sem_internal_record_t OS_bin_sem_table[OS_MAX_BIN_SEMAPHORES];
