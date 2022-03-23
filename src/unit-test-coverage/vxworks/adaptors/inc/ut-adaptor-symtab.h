/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-symtab
 */

#ifndef UT_ADAPTOR_SYMTAB_H
#define UT_ADAPTOR_SYMTAB_H

#include "common_types.h"

int32 UT_SymTabTest_CallIteratorFunc(const char *name, void *val, size_t TestSize, size_t SizeLimit);
int32 UT_SymTabTest_GetIteratorStatus(void);

#endif /* UT_ADAPTOR_SYMTAB_H */
