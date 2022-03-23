/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_BINSEM_H
#define OS_IMPL_BINSEM_H

#include "osconfig.h"
#include <semLib.h>

/* Binary Semaphores */
typedef struct
{
    VX_BINARY_SEMAPHORE(bmem);
    SEM_ID vxid;
} OS_impl_binsem_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_binsem_internal_record_t OS_impl_bin_sem_table[OS_MAX_BIN_SEMAPHORES];

#endif /* OS_IMPL_BINSEM_H */
