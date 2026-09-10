
#include "os-shared-idmap.h"
#include "utassert.h"
#include "uttest.h"
#include "utstubs.h"
#include "OCS_pthread.h"
#include "OCS_errno.h"
#include "OCS_time.h"

extern int32 OS_Posix_TableMutex_Init(osal_objtype_t idtype);
extern void  OS_Posix_ReleaseTableMutex(void *mut);

static struct OCS_timespec Deadline;
static osal_objtype_t      WakeType;
static uint32              WakeCall;
static uint32              WaitCalls;
static uint32              WaitLimit;
static int32               WaitError;
static void               *Condition;
static void               *Mutex;

static int32 WaitHook(void *UserObj, int32 StubRetcode, uint32 CallCount, const UT_StubContext_t *Context)
{
    const struct OCS_timespec *limit = UT_Hook_GetArgValueByName(Context, "abstime", const struct OCS_timespec *);
    void                      *cond  = UT_Hook_GetArgValueByName(Context, "cond", OCS_pthread_cond_t *);
    void                      *mutex = UT_Hook_GetArgValueByName(Context, "mutex", OCS_pthread_mutex_t *);

    UtAssert_True(limit->tv_sec == Deadline.tv_sec && limit->tv_nsec == Deadline.tv_nsec,
                  "Every wait uses the original absolute deadline");
    Condition = cond;
    Mutex     = mutex;
    ++WaitCalls;
    if (WaitCalls == WakeCall)
    {
        /* Model a table owner releasing the mutex while this waiter sleeps. */
        OS_Unlock_Global_Impl(WakeType);
        OS_Lock_Global_Impl(WakeType);
    }
    return WaitCalls < WaitLimit ? 0 : WaitError;
}

static void Setup(void)
{
    UT_ResetState(0);
    Deadline.tv_sec  = 0;
    Deadline.tv_nsec = 10000000;
    WakeType         = OS_OBJECT_TYPE_OS_TASK;
    WakeCall         = 0;
    WaitCalls        = 0;
    WaitLimit        = 1;
    WaitError        = OCS_ETIMEDOUT;
    UT_SetHookFunction(UT_KEY(OCS_pthread_cond_timedwait), WaitHook, NULL);
    UT_SetDefaultReturnValue(UT_KEY(OCS_pthread_cond_timedwait), OCS_ETIMEDOUT);
}

static void TestSpuriousWakeups(void)
{
    WaitLimit = 3;
    OS_WaitForStateChange_Impl(OS_OBJECT_TYPE_OS_TASK, 1);
    UtAssert_STUB_COUNT(OCS_pthread_cond_timedwait, 3);
    UtAssert_STUB_COUNT(OCS_clock_gettime, 1);
    UtAssert_STUB_COUNT(OCS_pthread_cleanup_push, 1);
    UtAssert_STUB_COUNT(OCS_pthread_cleanup_pop, 1);
}

static void TestStateChange(void)
{
    WakeCall  = 2;
    WaitLimit = 3;
    OS_WaitForStateChange_Impl(OS_OBJECT_TYPE_OS_TASK, 1);
    UtAssert_STUB_COUNT(OCS_pthread_cond_timedwait, 2);
    UtAssert_STUB_COUNT(OCS_pthread_cond_broadcast, 1);
}

static void TestOtherTableChange(void)
{
    WakeCall  = 1;
    WakeType  = OS_OBJECT_TYPE_OS_QUEUE;
    WaitLimit = 2;
    OS_WaitForStateChange_Impl(OS_OBJECT_TYPE_OS_TASK, 1);
    UtAssert_STUB_COUNT(OCS_pthread_cond_timedwait, 2);
}

static void TestTimeoutAndError(void)
{
    OS_WaitForStateChange_Impl(OS_OBJECT_TYPE_OS_TASK, 1);
    UtAssert_STUB_COUNT(OCS_pthread_cond_timedwait, 1);
    WaitError = OCS_EINVAL;
    OS_WaitForStateChange_Impl(OS_OBJECT_TYPE_OS_TASK, 1);
    UtAssert_STUB_COUNT(OCS_pthread_cond_timedwait, 2);
}

static void TestDeadlineAndIndependentTables(void)
{
    void               *task_cond;
    void               *task_mutex;
    struct OCS_timespec now = { 10, 999000000 };
    UT_SetDataBuffer(UT_KEY(OCS_clock_gettime), &now, sizeof(now), false);
    Deadline.tv_sec  = 11;
    Deadline.tv_nsec = 9000000;
    OS_WaitForStateChange_Impl(OS_OBJECT_TYPE_OS_TASK, 1);
    task_cond        = Condition;
    task_mutex       = Mutex;
    Deadline.tv_sec  = 1;
    Deadline.tv_nsec = 0;
    OS_WaitForStateChange_Impl(OS_OBJECT_TYPE_OS_QUEUE, 11);
    UtAssert_True(Condition != task_cond && Mutex != task_mutex, "Object types have distinct synchronization objects");
    Deadline.tv_sec = 0;
    OS_WaitForStateChange_Impl(OS_OBJECT_TYPE_OS_TASK, 0);
}

static void TestMutexOperations(void)
{
    OS_Lock_Global_Impl(OS_OBJECT_TYPE_UNDEFINED);
    OS_Unlock_Global_Impl(OS_OBJECT_TYPE_UNDEFINED);
    UtAssert_STUB_COUNT(OCS_pthread_mutex_lock, 0);
    UtAssert_STUB_COUNT(OCS_pthread_mutex_unlock, 0);
    OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_TASK);
    OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TASK);
    UtAssert_STUB_COUNT(OCS_pthread_mutex_lock, 1);
    UtAssert_STUB_COUNT(OCS_pthread_cond_broadcast, 1);
    UT_SetDefaultReturnValue(UT_KEY(OCS_pthread_mutex_lock), OCS_EINVAL);
    UT_SetDefaultReturnValue(UT_KEY(OCS_pthread_mutex_unlock), OCS_EINVAL);
    UT_SetDefaultReturnValue(UT_KEY(OCS_pthread_cond_broadcast), OCS_EINVAL);
    OS_Lock_Global_Impl(OS_OBJECT_TYPE_OS_TASK);
    OS_Unlock_Global_Impl(OS_OBJECT_TYPE_OS_TASK);
    OS_Posix_ReleaseTableMutex(NULL);
    UtAssert_STUB_COUNT(OCS_pthread_mutex_unlock, 3);
}

static void TestInitialization(void)
{
    UT_EntryKey_t failures[] = { UT_KEY(OCS_pthread_mutexattr_init),
                                 UT_KEY(OCS_pthread_mutexattr_setprotocol),
                                 UT_KEY(OCS_pthread_mutexattr_settype),
                                 UT_KEY(OCS_pthread_mutex_init),
                                 UT_KEY(OCS_pthread_cond_init) };
    size_t        i;
    UtAssert_INT32_EQ(OS_Posix_TableMutex_Init(OS_OBJECT_TYPE_UNDEFINED), OS_SUCCESS);
    UtAssert_INT32_EQ(OS_Posix_TableMutex_Init(OS_OBJECT_TYPE_OS_TASK), OS_SUCCESS);
    for (i = 0; i < sizeof(failures) / sizeof(failures[0]); ++i)
    {
        UT_SetDefaultReturnValue(failures[i], OCS_EINVAL);
        UtAssert_INT32_EQ(OS_Posix_TableMutex_Init(OS_OBJECT_TYPE_OS_TASK), OS_ERROR);
        UT_ClearDefaultReturnValue(failures[i]);
    }
}

void UtTest_Setup(void)
{
    UtTest_Add(TestSpuriousWakeups, Setup, NULL, "Spurious wakeups retain the original wait");
    UtTest_Add(TestStateChange, Setup, NULL, "A notified state change releases the wait");
    UtTest_Add(TestOtherTableChange, Setup, NULL, "Other object types cannot release the wait");
    UtTest_Add(TestTimeoutAndError, Setup, NULL, "Timeouts and errors stop waiting");
    UtTest_Add(TestDeadlineAndIndependentTables, Setup, NULL, "Deadlines and per-type isolation");
    UtTest_Add(TestMutexOperations, Setup, NULL, "Mutex and cleanup paths");
    UtTest_Add(TestInitialization, Setup, NULL, "Table initialization");
}
