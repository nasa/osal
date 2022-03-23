/**
 * \file
 * \ingroup ut-stubs
 *
 * Declarations and prototypes for symLib
 */

#ifndef OVERRIDE_SYMLIB_H
#define OVERRIDE_SYMLIB_H

#include "OCS_symLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in symLib.h */
/* ----------------------------------------- */
#define LOAD_ALL_SYMBOLS OCS_LOAD_ALL_SYMBOLS
#define SYM_FIND_BY_NAME OCS_SYM_FIND_BY_NAME

#define SYM_TYPE    OCS_SYM_TYPE
#define SYMTAB      OCS_SYMTAB
#define SYMBOL      OCS_SYMBOL
#define SYMTAB_ID   OCS_SYMTAB_ID
#define SYMBOL_ID   OCS_SYMBOL_ID
#define SYMBOL_DESC OCS_SYMBOL_DESC
#define SYM_GROUP   OCS_SYM_GROUP
#define SYM_VALUE   OCS_SYM_VALUE

#define sysSymTbl     OCS_sysSymTbl
#define symFindByName OCS_symFindByName
#define symEach       OCS_symEach
#define symFind       OCS_symFind

#endif /* OVERRIDE_SYMLIB_H */
