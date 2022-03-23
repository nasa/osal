/**
 * \file
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

#ifndef UTBSP_H
#define UTBSP_H

#include "common_types.h"
#include "utassert.h"

/**************************************************************
 * Functions implemented by the UT-specific BSP
 **************************************************************/

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
 */
void UT_BSP_Setup(void);

/**
 * Initialize the unit test at the BSP layer.
 *
 * This is just a hook for the BSP to be informed of the start-of-test event and may be a no-op.
 *
 * \param[in] SegmentNumber Number of current test segment
 * \param[in] SegmentName   Name of current test segment
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
 * The BSP overall test end function.
 *
 * Invokes the BSP-specific global pass/fail reporting mechanism based on the global overall pass/fail counters.
 *
 * This function ends the current test process and returns to the controlling process.
 *
 * \param TestCounters  Counter object for the completed test
 */
void UT_BSP_EndTest(const UtAssert_TestCounter_t *TestCounters);

/**
 * UT mutex lock for multi-threaded test support
 *
 * Lock that should be acquired before modifying any global test state variables
 */
void UT_BSP_Lock(void);

/**
 * UT mutex unlock for multi-threaded test support
 *
 * Must be called after UT_BSP_Lock to allow other threads access to the global
 */
void UT_BSP_Unlock(void);

#endif /* UTBSP_H */
