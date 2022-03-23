/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for symLib.h
 */

#ifndef OCS_SYMLIB_H
#define OCS_SYMLIB_H

#include "OCS_basetypes.h"
#include "OCS_vxWorks.h"

/* ----------------------------------------- */
/* constants normally defined in symLib.h */
/* ----------------------------------------- */
#define OCS_LOAD_ALL_SYMBOLS 0x1A00
#define OCS_SYM_FIND_BY_NAME 0x1A01

/* ----------------------------------------- */
/* types normally defined in symLib.h */
/* ----------------------------------------- */
typedef int       OCS_SYM_TYPE;
typedef uintptr_t OCS_SYM_VALUE;
typedef int       OCS_SYM_GROUP;

typedef struct OCS_SYMBOL OCS_SYMBOL;
typedef struct OCS_SYMTAB OCS_SYMTAB;

typedef OCS_SYMTAB *OCS_SYMTAB_ID;
typedef OCS_SYMBOL *OCS_SYMBOL_ID;

typedef struct OCS_SYMBOL_DESC
{
    unsigned int  mask;
    char *        name;
    OCS_SYM_VALUE value;
} OCS_SYMBOL_DESC;

typedef OCS_BOOL (*OCS_symEach_Routine_t)(char *, OCS_SYM_VALUE, OCS_SYM_TYPE, OCS_Vx_usr_arg_t, OCS_SYM_GROUP);

/* ----------------------------------------- */
/* prototypes normally declared in symLib.h */
/* ----------------------------------------- */

extern OCS_SYMTAB_ID OCS_sysSymTbl;

extern OCS_STATUS  OCS_symFindByName(OCS_SYMTAB_ID symTblId, char *name, char **pValue, OCS_SYM_TYPE *pType);
extern OCS_SYMBOL *OCS_symEach(OCS_SYMTAB_ID symTblId, OCS_symEach_Routine_t routine, int routineArg);
extern OCS_STATUS  OCS_symFind(OCS_SYMTAB_ID symTblId, OCS_SYMBOL_DESC *pSymbol);

#endif /* OCS_SYMLIB_H */
