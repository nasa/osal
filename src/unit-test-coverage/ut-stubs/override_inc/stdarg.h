/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for stdarg.h
 */

#ifndef OVERRIDE_STDARG_H
#define OVERRIDE_STDARG_H

#include "OCS_stdarg.h"

/* ----------------------------------------- */
/* mappings for declarations in stdarg.h */
/* ----------------------------------------- */

#define va_list            OCS_va_list
#define va_start(ap, last) OCS_va_start(ap, last)
#define va_end(ap)         OCS_va_end(ap)

#endif /* OVERRIDE_STDARG_H */
