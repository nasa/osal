/**
 * \file
 * \ingroup portable
 *
 * Declarations and prototypes for os-portable-coveragetest
 */

#ifndef OS_PORTABLE_COVERAGETEST_H
#define OS_PORTABLE_COVERAGETEST_H

/*
 * Includes
 */

#include "utassert.h"
#include "uttest.h"
#include "utstubs.h"

#include "os-shared-globaldefs.h"

#define OSAPI_TEST_FUNCTION_RC(func, args, exp)                                                                \
    {                                                                                                          \
        int32 rcexp = exp;                                                                                     \
        int32 rcact = func args;                                                                               \
        UtAssert_True(rcact == rcexp, "%s%s (%ld) == %s (%ld)", #func, #args, (long)rcact, #exp, (long)rcexp); \
    }

#define ADD_TEST(test) UtTest_Add((Test_##test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

/*
 * The default/primary table index used by most coverage tests.
 */
#define UT_INDEX_0 OSAL_INDEX_C(0)

/*
 * A secondary table index for coverage tests which require
 * more than one entry
 */
#define UT_INDEX_1 OSAL_INDEX_C(1)

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void);
void Osapi_Test_Teardown(void);

#endif /* OS_PORTABLE_COVERAGETEST_H */
