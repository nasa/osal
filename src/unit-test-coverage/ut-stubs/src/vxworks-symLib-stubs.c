/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* OSAL coverage stub replacement for symLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "OCS_symLib.h"

struct OCS_SYMBOL
{
    int sym;
};

struct OCS_SYMTAB
{
    int symtab;
};

static OCS_SYMBOL LOCAL_SYMBOL = {1};
static OCS_SYMTAB LOCAL_SYMTAB = {2};

OCS_SYMTAB_ID OCS_sysSymTbl = &LOCAL_SYMTAB;

OCS_STATUS OCS_symFindByName(OCS_SYMTAB_ID symTblId, char *name, char **pValue, OCS_SYM_TYPE *pType)
{
    return (UT_DEFAULT_IMPL(OCS_symFindByName));
}

OCS_SYMBOL *OCS_symEach(OCS_SYMTAB_ID symTblId, OCS_symEach_Routine_t routine, int routineArg)
{
    int32       Status;
    OCS_SYMBOL *retval;

    Status = UT_DEFAULT_IMPL(OCS_symEach);
    if (Status == 0)
    {
        retval = &LOCAL_SYMBOL;
    }
    else
    {
        retval = NULL;
    }

    return retval;
}

OCS_STATUS OCS_symFind(OCS_SYMTAB_ID symTblId, OCS_SYMBOL_DESC *pSymbol)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_symFind);
    if (Status == 0 && UT_Stub_CopyToLocal(UT_KEY(OCS_symFind), pSymbol, sizeof(*pSymbol)) < sizeof(*pSymbol))
    {
        memset(pSymbol, 0, sizeof(*pSymbol));
    }

    return Status;
}
