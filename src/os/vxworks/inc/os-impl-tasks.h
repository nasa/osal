/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_TASKS_H
#define OS_IMPL_TASKS_H

#include "osconfig.h"
#include <taskLib.h>

#if defined(VX_WIND_TCB_SIZE)
/* vxworks >= 7.0 should provide this symbol via taskLib.h. WIND_TCB is an opaque type */
typedef char OS_VxWorks_TCB_t[VX_WIND_TCB_SIZE];
#else
/* older vxworks expose the definition of VX_WIND_TCB_SIZE */
typedef WIND_TCB OS_VxWorks_TCB_t;
#endif /* !defined(VX_WIND_TCB_SIZE) */

/*tasks */
typedef struct
{
    OS_VxWorks_TCB_t tcb; /* Must be first */
    TASK_ID          vxid;
    void *           heap_block; /* set non-null if the stack was obtained with malloc() */
    size_t           heap_block_size;
} OS_impl_task_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

#endif /* OS_IMPL_TASKS_H */
