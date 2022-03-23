/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_SYMTAB_H
#define OS_IMPL_SYMTAB_H

#include "osconfig.h"
#include <symLib.h>

typedef struct
{
    size_t Sizelimit;
    size_t CurrSize;
    int32  StatusCode;
    int    fd;
} SymbolDumpState_t;

/* A global for storing the state in a SymbolDump call */
extern SymbolDumpState_t OS_VxWorks_SymbolDumpState;

BOOL OS_SymTableIterator_Impl(char *name, SYM_VALUE val, SYM_TYPE type, _Vx_usr_arg_t arg, SYM_GROUP group);

#endif /* OS_IMPL_SYMTAB_H */
