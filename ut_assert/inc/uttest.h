/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 *
 * Purpose: This file contains functions to implement a standard way to execute unit tests.
 *
 * Design Notes:
 *    By default the only output that is printed to the console is assert failures
 *    and a summary of the test results after all tests have executed.  To enable additional
 *    test output define the macro UT_VERBOSE.
 */

#ifndef UTTEST_H
#define UTTEST_H

#include <stdbool.h>

/*
 * Exported Functions
 */

/**
 * \brief Adds a new unit test to the test database.
 *
 * Called by the user to register a new test case with the library.
 *
 * Note: Nested addition of tests is not supported.  Calling
 *       UtTest_Add from within a test function added using UtTest_Add
 *       will not cause the nested test to execute, and will be
 *       silently ignored.
 *
 * \param Test     Main test function to call.
 * \param Setup    Setup function, called before the test function
 * \param Teardown Cleanup function, called after the test function
 * \param TestName Name of test for logging purposes
 */
void UtTest_Add(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *TestName);

/**
 * \brief Registers a setup function
 *
 * This group of functions are invoked BEFORE normal test routines added with UtTest_Add.
 * Within the group, functions are executed in the order registered.
 *
 * \param Setup        Setup function, called before the test function
 * \param SequenceName Name of sequence for logging purposes
 */
void UtTest_AddSetup(void (*Setup)(void), const char *SequenceName);

/**
 * \brief Registers a teardown function
 *
 * This group of functions is invoked AFTER normal test routines added with UtTest_Add.
 * Within the group, functions are executed in the order registered.
 *
 * \param Teardown     Teardown function, called before the test function
 * \param SequenceName Name of sequence for logging purposes
 */
void UtTest_AddTeardown(void (*Teardown)(void), const char *SequenceName);

/**
 * \brief Add a test as a member of a subgroup.
 *
 * Allow tests to be grouped together
 *
 * This is just a wrapper around UtTest_Add() that registers
 * a test with a "GroupName.TestName" convention.  Purely an
 * organizational/identification helper for units which have
 * lots of tests.
 *
 * \param Test     Main test function to call.
 * \param Setup    Setup function, called before the test function
 * \param Teardown Cleanup function, called after the test function
 * \param GroupName Name of group for logging purposes
 * \param TestName Name of test for logging purposes
 */
void UtTest_AddSubTest(void (*Test)(void), void (*Setup)(void), void (*Teardown)(void), const char *GroupName,
                       const char *TestName);

/**
 * \brief Early initialization function
 *
 * Reset the global data to a safe state for initial start-up.
 * This should be called before any other API.
 */
void UtTest_EarlyInit(void);

/**
 * \brief Execute all registered tests
 *
 * All test functions that were registered with UtTest_Add will be executed in order.
 */
void UtTest_Run(void);

/*
 * \brief Set up function for UT-Assert based test routines
 *
 * This function must be provided by the user to set up test cases.
 * This should call UtTest_Add() for each test case.
 */
void UtTest_Setup(void);

#endif /* UTTEST_H */
