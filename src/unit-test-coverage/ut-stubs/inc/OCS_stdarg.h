/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for stdarg.h
 */

#ifndef OCS_STDARG_H
#define OCS_STDARG_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in stdarg.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in stdarg.h */
/* ----------------------------------------- */
typedef struct
{
    void *p;
} OCS_va_list;

/* ----------------------------------------- */
/* prototypes normally declared in stdarg.h */
/* ----------------------------------------- */

#define OCS_va_start(ap, last) ap.p = &last
#define OCS_va_end(ap)

#endif /* OCS_STDARG_H */
