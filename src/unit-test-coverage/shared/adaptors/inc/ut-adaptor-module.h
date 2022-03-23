/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-module
 */

#ifndef UT_ADAPTOR_MODULE_H
#define UT_ADAPTOR_MODULE_H

#include "common_types.h"
#include "osapi-module.h"

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

/**
 * Purges all state tables and resets back to initial conditions
 * Helps avoid cross-test dependencies
 */
void Osapi_Internal_ResetState(void);

/* A dummy function for the static symbol lookup test.  Not called */
void Test_DummyFunc(void);

int32 Osapi_Call_SymbolLookup_Static(cpuaddr *SymbolAddress, const char *SymbolName, const char *ModuleName);
int32 Osapi_Call_ModuleLoad_Static(const char *ModuleName);

#endif /* UT_ADAPTOR_MODULE_H */
