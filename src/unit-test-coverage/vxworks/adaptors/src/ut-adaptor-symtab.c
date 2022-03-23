/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-loader.h"

#include "os-vxworks.h"
#include "os-impl-symtab.h"

/*
 * A UT-specific wrapper function to invoke the Symbol Table Iterator.
 * This is normally static so it needs this wrapper to call it.
 */
int32 UT_SymTabTest_CallIteratorFunc(const char *name, void *val, size_t TestSize, size_t SizeLimit)
{
    OS_VxWorks_SymbolDumpState.Sizelimit = SizeLimit;
    OS_VxWorks_SymbolDumpState.CurrSize  = TestSize;
    /*
     * note the internal function is takes a name declared as "char*" to be
     * consistent with the VxWorks API, however the implementation does not
     * modify this argument.
     */
    return OS_SymTableIterator_Impl((char *)name, (OCS_SYM_VALUE)val, 0, 0, 0);
}

/*
 * Gets the current status of the iterator function
 */
int32 UT_SymTabTest_GetIteratorStatus(void)
{
    return OS_VxWorks_SymbolDumpState.StatusCode;
}
