/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub basic data types
 */

#ifndef OCS_BASETYPES_H
#define OCS_BASETYPES_H

/*
 * NOTE: These header files are intentionally _not_ overridden
 * in the replacement/override header directory, so this should
 * pull in the actual (native system) version of these files.
 *
 * It is important to pull in these definitions first before any
 * potential re-mapping (#define) statements are done.
 */

#include <stddef.h>  /* for correct size_t and ptrdiff_t types */
#include <stdint.h>  /* for correct fixed-width integer types */
#include <limits.h>  /* for correct INT_MAX, etc. */
#include <stdbool.h> /* for correct boolean semantics */

#endif /* OCS_BASETYPES_H */
