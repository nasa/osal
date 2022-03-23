/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-console
 */

#ifndef UT_ADAPTOR_CONSOLE_H
#define UT_ADAPTOR_CONSOLE_H

#include "common_types.h"
#include "ut-adaptor-common.h"

extern void *const  UT_Ref_OS_impl_console_table;
extern size_t const UT_Ref_OS_impl_console_table_SIZE;

/**
 * Invokes the console helper task entry point
 */
extern int UT_ConsoleTest_TaskEntry(int arg);

#endif /* UT_ADAPTOR_CONSOLE_H */
