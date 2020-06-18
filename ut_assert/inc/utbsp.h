/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file utbsp.h
 *
 *  Created on: Feb 25, 2015
 *      Author: joseph.p.hickey@nasa.gov
 *
 * Types and definitions for the generic Unit Test BSP interface implemented
 * as part of OSAL.
 *
 * When the OSAL BSP is used to start the tests, the same BSP can also be used to output
 * test status, this way alternate output resources can be used whenever needed.
 *
 * Provides a means to abstract the test start/stop and pass/fail reporting
 * to a board-specific function.  This allows tests to be better utilized on
 * embedded targets that may not have a visible printf() function.
 *
 * (We should not assume a working "printf" anywhere in UT-assert)
 *
 * It also provides for BSP customizations to the way tests are launched
 * and the way pass/fail determinations are made.
 */


#ifndef _UTBSP_H_
#define _UTBSP_H_

#include "common_types.h"
#include "utassert.h"

/**************************************************************
 * Functions implemented by the UT-specific BSP
 **************************************************************/

/**
 * Gets the number of additional user-supplied options
 *
 * For instance, this would be command line parameters on platforms that have a command line.
 */
int32 UT_BSP_GetTotalOptions(void);

/**
 * Gets the text string associated with a specific user-supplied option
 *
 * For instance, this would be command line parameters on platforms that have a command line.
 */
const char * UT_BSP_GetOptionString(int32 OptionNum);

/*
 * Note - functions here are not typically directly called by UT code, they
 * are used by the framework to implement the common functions.
 *
 * In general, these should all be stateless in order to keep the implementation
 * on the BSP side as simple as possible.
 */

/**
 * Initialize the BSP for unit test.
 *
 * This is just a hook for the BSP to be informed of the start-of-test event and may be a no-op.
 *
 * \param Name Name of application under test
 */
void UT_BSP_Setup(const char *Name);

/**
 * Initialize the unit test at the BSP layer.
 *
 * This is just a hook for the BSP to be informed of the start-of-test event and may be a no-op.
 *
 * \param Appname Name of current test segment
 */
void UT_BSP_StartTestSegment(uint32 SegmentNumber, const char *SegmentName);

/**
 * The BSP text-output function for unit test messages.
 *
 * This is used by the unit test code, typically for progress messages.  The destination is BSP-dependent
 * and may or may not go to the same place that OS_printf() does.  The BSP may choose to send this
 * to a special/separate log file to separate the test progress messages from the OS_printf() messages
 * produced by the application/function under test.
 *
 * \param MessageType  The nature (verbosity) of the message.  The BSP may allow the operator to select
 *    the maximum message verbosity at runtime to select what should go into the log file.
 *
 * \param OutputMessage The message text.
 */
void UT_BSP_DoText(uint8 MessageType, const char *OutputMessage);


/**
 * The BSP single test case reporting function.
 *
 * Invokes the BSP-specific pass/fail reporting mechanism based on the MessageType.
 *
 * This is typically output as a message to the test log but may be fancier if the BSP requires it.
 * One example might be to toggle a GPIO bit or LED if the test is running on a separate processor board.
 *
 * \param File         File containing the test case
 * \param LineNum      Line number containing the test case
 * \param MessageType  Should be set to either UT_MESSAGE_PASS or UT_MESSAGE_FAILURE.
 * \param SubsysName   The subsystem under test (abbreviated name)
 * \param ShortDesc    Short description of the test case
 * \param SegmentNum   Sequence among the overall/global test Segments
 * \param TestDescr    Sequence within the current test Segment
 */
void UT_BSP_DoReport(const char *File, uint32 LineNum, uint32 SegmentNum, uint32 SegmentSeq, uint8 MessageType, const char *SubsysName, const char *ShortDesc);

/**
 * The BSP overall test reporting function.
 *
 * Invokes the BSP-specific overall pass/fail reporting mechanism based the subsystem pass/fail counters.
 *
 * Like the UT_BSP_DoReport() function, this is typically done as a message on the console/log however
 * it might be different for embedded targets.
 *
 * \param Appname       The application under test
 * \param TestCounters  Counter object for the completed test
 */
void UT_BSP_DoTestSegmentReport(const char *SegmentName, const UtAssert_TestCounter_t *TestCounters);

/**
 * The BSP overall test end function.
 *
 * Invokes the BSP-specific global pass/fail reporting mechanism based the global overall pass/fail counters.
 *
 * This function ends the current test process and returns to the controlling process.
 *
 * \param TestCounters  Counter object for the completed test
 */
void UT_BSP_EndTest(const UtAssert_TestCounter_t *TestCounters);


#endif /* _UTBSP_H_ */
