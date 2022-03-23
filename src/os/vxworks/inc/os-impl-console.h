/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_CONSOLE_H
#define OS_IMPL_CONSOLE_H

#include <stdbool.h>
#include "osconfig.h"
#include <semLib.h>
#include <taskLib.h>

/* Console device */
typedef struct
{
    VX_COUNTING_SEMAPHORE(cmem);
    SEM_ID  datasem;
    TASK_ID taskid;
} OS_impl_console_internal_record_t;

extern OS_impl_console_internal_record_t OS_impl_console_table[OS_MAX_CONSOLES];

#endif /* OS_IMPL_CONSOLE_H */
