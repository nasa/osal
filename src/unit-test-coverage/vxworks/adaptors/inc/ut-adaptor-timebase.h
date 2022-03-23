/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-timebase
 */

#ifndef UT_ADAPTOR_TIMEBASE_H
#define UT_ADAPTOR_TIMEBASE_H

#include "common_types.h"
#include "OCS_symLib.h"
#include "OCS_time.h"

extern void *const  UT_Ref_OS_impl_timebase_table;
extern size_t const UT_Ref_OS_impl_timebase_table_SIZE;

int32 UT_Call_OS_VxWorks_TimeBaseAPI_Impl_Init(void);

void UT_TimeBaseTest_Setup(osal_index_t local_id, int signo, bool reset_flag);

/**
 * Invokes OS_VxWorks_SigWait() with the given arguments.
 * This is normally a static function but exposed via a non-static wrapper for UT purposes.
 */
int32 UT_TimeBaseTest_CallSigWaitFunc(osal_id_t timebase_id);

/* Invokes the static OS_VxWorks_TimeBaseTask() function with given argument */
int UT_TimeBaseTest_CallHelperTaskFunc(int arg);

/* Invokes the static OS_VxWorks_RegisterTimer() function with given argument */
void UT_TimeBaseTest_CallRegisterTimer(osal_id_t obj_id);

/* Hook functions which set the timer registration state */
void UT_TimeBaseTest_SetTimeBaseRegState(osal_index_t local_id, bool is_success);
void UT_TimeBaseTest_ClearTimeBaseRegState(osal_index_t local_id);

/* Hook functions which test the timer registration state */
bool UT_TimeBaseTest_CheckTimeBaseRegisteredState(osal_index_t local_id);
bool UT_TimeBaseTest_CheckTimeBaseErrorState(osal_index_t local_id);

/* Invoke the internal UsecToTimespec API */
void UT_TimeBaseTest_UsecToTimespec(uint32 usecs, struct OCS_timespec *time_spec);

#endif /* UT_ADAPTOR_TIMEBASE_H */
