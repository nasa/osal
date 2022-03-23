/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for stdio.h
 */

#ifndef OVERRIDE_STDIO_H
#define OVERRIDE_STDIO_H

#include "OCS_stdio.h"

/* ----------------------------------------- */
/* mappings for declarations in stdio.h */
/* ----------------------------------------- */

#define FILE         OCS_FILE
#define fclose       OCS_fclose
#define fgets        OCS_fgets
#define fopen        OCS_fopen
#define fputs        OCS_fputs
#define remove       OCS_remove
#define rename       OCS_rename
#define snprintf     OCS_snprintf
#define vsnprintf    OCS_vsnprintf
#define printf(...)  OCS_printf(__VA_ARGS__)
#define fprintf(...) OCS_fprintf(__VA_ARGS__)
#define putchar      OCS_putchar

#define stdin  OCS_stdin
#define stdout OCS_stdout
#define stderr OCS_stderr

#endif /* OVERRIDE_STDIO_H */
