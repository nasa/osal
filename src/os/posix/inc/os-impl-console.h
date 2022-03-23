/**
 * \file
 *
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_CONSOLE_H
#define OS_IMPL_CONSOLE_H

#include <stdbool.h>
#include "osconfig.h"
#include <unistd.h>
#include <semaphore.h>

/* Console device */
typedef struct
{
    sem_t data_sem;
} OS_impl_console_internal_record_t;

extern OS_impl_console_internal_record_t OS_impl_console_table[OS_MAX_CONSOLES];

#endif /* OS_IMPL_CONSOLE_H */
