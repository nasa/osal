/**
 * \file
 *
 * \ingroup  shared
 *
 */

#ifndef OS_SHARED_ERRORS_H
#define OS_SHARED_ERRORS_H

#include "os-shared-globaldefs.h"

/* Mapping of integer error number to name */
typedef struct
{
    int32       Number;
    const char *Name;
} OS_ErrorTable_Entry_t;

extern const OS_ErrorTable_Entry_t OS_IMPL_ERROR_NAME_TABLE[];

#endif /* OS_SHARED_ERRORS_H */
