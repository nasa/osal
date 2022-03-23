/**
 * \file
 * \ingroup vxworks
 *
 * Declarations and prototypes for os-vxworks-coveragetest
 */

#ifndef OS_VXWORKS_COVERAGETEST_H
#define OS_VXWORKS_COVERAGETEST_H

#include "utassert.h"
#include "uttest.h"
#include "utstubs.h"

#include "os-shared-idmap.h"

#define OSAPI_TEST_FUNCTION_RC(func, exp)                                                             \
    {                                                                                                 \
        int32 rcexp = exp;                                                                            \
        int32 rcact = func;                                                                           \
        UtAssert_True(rcact == rcexp, "%s (%ld) == %s (%ld)", #func, (long)rcact, #exp, (long)rcexp); \
    }

#define ADD_TEST(test) UtTest_Add((Test_##test), Osapi_Test_Setup, Osapi_Test_Teardown, #test)

#define UT_INDEX_0 OSAL_INDEX_C(0)
#define UT_INDEX_1 OSAL_INDEX_C(1)
#define UT_INDEX_2 OSAL_INDEX_C(2)

#define UT_TOKEN_0                                    \
    (OS_object_token_t)                               \
    {                                                 \
        .obj_id = (osal_id_t) {0x10000}, .obj_idx = 0 \
    }
#define UT_TOKEN_1                                    \
    (OS_object_token_t)                               \
    {                                                 \
        .obj_id = (osal_id_t) {0x10001}, .obj_idx = 1 \
    }
#define UT_TOKEN_2                                    \
    (OS_object_token_t)                               \
    {                                                 \
        .obj_id = (osal_id_t) {0x10002}, .obj_idx = 2 \
    }

/* Osapi_Test_Setup
 *
 * Purpose:
 *   Called by the unit test tool to set up the app prior to each test
 */
void Osapi_Test_Setup(void);
void Osapi_Test_Teardown(void);

#endif /* OS_VXWORKS_COVERAGETEST_H */
