/**
 * \file
 *
 * Purpose: This file contains functions to implement a standard way to execute unit tests.
 */

#ifndef UTGLOBAL_H
#define UTGLOBAL_H

/*
 * Includes
 */
#include "osapi.h"
#include "utassert.h"
#include "utlist.h"
#include "utbsp.h"
#include "uttest.h"
#include "utstubs.h"

/*
 * Type Definitions
 */

typedef struct
{
    void (*Test)(void);
    void (*Setup)(void);
    void (*Teardown)(void);

    /* Note - the name entry should be long enough to support a GroupName.TestName pattern,
     * hence why it uses double the OS_MAX_API_NAME length */
    char TestName[OS_MAX_API_NAME * 2];
} UtTestDataBaseEntry_t;

typedef struct
{
    UtListHead_t *DataBasePtr;
    uint32        ExecutedCount;
} UtAssert_Global_t;

/*
 * Global Test Data
 */
extern UtAssert_Global_t UtAssert_Global;

#endif /* UTGLOBAL_H */
