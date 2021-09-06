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
 * \file     os-impl-tasks.c
 * \ingroup  qt
 * \author   samuel.r.price@nasa.gov
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include "os-qt.h"
#include "bsp-impl.h"
#include <sched.h>


extern "C" {
#include "os-impl-tasks.h"
#include "os-shared-task.h"
#include "os-shared-idmap.h"

}

/*
 * Defines
 */
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN (8 * 1024)
#endif

/* Tables where the OS object information is stored */
OS_impl_task_internal_record_t OS_impl_task_table[OS_MAX_TASKS];

/*
 * Local Function Prototypes
 */
extern "C" {

/*----------------------------------------------------------------------------
 * Name: OS_PriorityRemap
 *
 * Purpose: Remaps the OSAL priority into one that is viable for this OS
 *
 * Note: This implementation assumes that InputPri has already been verified
 * to be within the range of [0,OS_MAX_TASK_PRIORITY]
 *
----------------------------------------------------------------------------*/
static int  OS_PriorityRemap(osal_priority_t InputPri)
{
    int OutputPri;

    if (InputPri == 0)
    {
        /* use the "MAX" local priority only for OSAL tasks with priority=0 */
        OutputPri = QT_GlobalVars.PriLimits.PriorityMax;
    }
    else if (InputPri >= OS_MAX_TASK_PRIORITY)
    {
        /* use the "MIN" local priority only for OSAL tasks with priority=255 */
        OutputPri = QT_GlobalVars.PriLimits.PriorityMin;
    }
    else
    {
        /*
         * Spread the remainder of OSAL priorities over the remainder of local priorities
         *
         * Note OSAL priorities use the VxWorks style with zero being the
         * highest and OS_MAX_TASK_PRIORITY being the lowest, this inverts it
         */
        OutputPri = (OS_MAX_TASK_PRIORITY - 1) - (int)InputPri;

        OutputPri *= (QT_GlobalVars.PriLimits.PriorityMax - QT_GlobalVars.PriLimits.PriorityMin) - 2;
        OutputPri += OS_MAX_TASK_PRIORITY / 2;
        OutputPri /= (OS_MAX_TASK_PRIORITY - 2);
        OutputPri += QT_GlobalVars.PriLimits.PriorityMin + 1;
    }

    return OutputPri;
} /* end OS_PriorityRemap */

/*----------------------------------------------------------------
 *
 * Function: OS_NoopSigHandler
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *           A QT signal handler that does nothing
 *
 *-----------------------------------------------------------------*/
static void OS_NoopSigHandler(int signal) {} /* end OS_NoopSigHandler */

/*---------------------------------------------------------------------------------------
   Name: OS_PthreadEntry

   Purpose: A Simple pthread-compatible entry point that calls the real task function

   returns: NULL

    NOTES: This wrapper function is only used locally by OS_TaskCreate below

---------------------------------------------------------------------------------------*/
static void *OS_PthreadTaskEntry(void *arg)
{
    OS_VoidPtrValueWrapper_t local_arg;

    local_arg.opaque_arg = arg;
    OS_TaskEntryPoint(local_arg.id); /* Never returns */

    return NULL;
}


/*----------------------------------------------------------------
 *
 * Function: OS_TaskCreate_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    OS_VoidPtrValueWrapper_t        arg;
    int32                           return_code;
    OS_impl_task_internal_record_t *impl;
    OS_task_internal_record_t *     task;

    arg.opaque_arg = NULL;
    arg.id         = OS_ObjectIdFromToken(token);

    task = OS_OBJECT_TABLE_GET(OS_task_table, *token);
    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    return_code = OS_QT_InternalTaskCreate_Impl(impl, task->priority, task->stack_size, OS_PthreadTaskEntry,
                                                   arg.opaque_arg);

    return return_code;
} /* end OS_TaskCreate_Impl */


/*----------------------------------------------------------------
 *
 * Function: OS_TaskDetach_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDetach_Impl(const OS_object_token_t *token)
{
    OS_impl_task_internal_record_t *impl;
    int                             ret;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    return OS_ERR_NOT_IMPLEMENTED;
    // ret = pthread_detach(impl->id);

    // if (ret != 0)
    // {
    //     OS_DEBUG("pthread_detach: Failed on Task ID = %lu, err = %s\n",
    //              OS_ObjectIdToInteger(OS_ObjectIdFromToken(token)), strerror(ret));
    //     return OS_ERROR;
    // }

    // return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 * Function: OS_TaskMatch_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskMatch_Impl(const OS_object_token_t *token)
{
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    /* TODO */
    // if (pthread_equal(pthread_self(), impl->id) == 0)
    // {
    //     return OS_ERROR;
    // }
    return OS_ERR_NOT_IMPLEMENTED;

} /* end OS_TaskMatch_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskDelete_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    if(impl->thread == NULL){
        /* TODO Already deleted should this be an error? */
        return OS_SUCCESS;
    }

    /*
    ** Try to delete the task
    ** If this fails, not much recourse - the only potential cause of failure
    ** to cancel here is that the thread ID is invalid because it already exited itself,
    ** and if that is true there is nothing wrong - everything is OK to continue normally.
    */

    /* TODO ... Decide proper way to stop the task */
    impl->thread->quit();
    impl->thread->requestInterruption();
    impl->thread->wait(100);
    impl->thread->terminate();
    impl->thread->wait();

    delete impl->thread;


    return OS_SUCCESS;

} /* end OS_TaskDelete_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskExit_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_TaskExit_Impl()
{
    /* TODO .. How to get curent thread ... */
    // OS_impl_task_internal_record_t *impl;
    // impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    // if(impl->thread == NULL){
    //     /* TODO Already deleted should this be an error? */
    //     return;
    // }

    // impl->thread->quit();


} /* end OS_TaskExit_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskDelay_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskDelay_Impl(uint32 millisecond)
{
    QThread *thread = QThread::currentThread();
    if(thread == NULL)
        return OS_ERROR;

    thread->msleep(millisecond);
    return OS_SUCCESS;

} /* end OS_TaskDelay_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskSetPriority_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskSetPriority_Impl(const OS_object_token_t *token, osal_priority_t new_priority)
{
    int os_priority;
    int ret;

    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);
    if(impl->thread == NULL){
        return OS_ERROR;
    }
    if (QT_GlobalVars.EnableTaskPriorities)
    {
        /* Change OSAL priority into a priority that will work for this OS */
        os_priority = OS_PriorityRemap(new_priority);
        QThread::Priority priorty = (QThread::Priority)os_priority;
        /*
        ** Set priority
        */
        impl->thread->setPriority(priorty);
    }

    return OS_SUCCESS;
} /* end OS_TaskSetPriority_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskRegister_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskRegister_Impl(osal_id_t global_task_id)
{
    int32                return_code;
    // OS_U32ValueWrapper_t arg;

    // arg.opaque_arg = 0;
    // arg.id         = global_task_id;
    /* TODO */
    return OS_ERR_NOT_IMPLEMENTED;

    // return_code = pthread_setspecific(QT_GlobalVars.ThreadKey, arg.opaque_arg);
    // if (return_code == 0)
    // {
    //     return_code = OS_SUCCESS;
    // }
    // else
    // {
    //     OS_DEBUG("OS_TaskRegister_Impl failed during pthread_setspecific() error=%s\n", strerror(return_code));
    //     return_code = OS_ERROR;
    // }

    // return return_code;
} /* end OS_TaskRegister_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetId_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
osal_id_t OS_TaskGetId_Impl(void)
{
    /* TODO */
    return OS_ERR_NOT_IMPLEMENTED;
    // OS_U32ValueWrapper_t self_record;

    // self_record.opaque_arg = pthread_getspecific(QT_GlobalVars.ThreadKey);

    // return (self_record.id);
} /* end OS_TaskGetId_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskGetInfo_Impl(const OS_object_token_t *token, OS_task_prop_t *task_prop)
{
    OS_impl_task_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);
    if(impl->thread == NULL){
        return OS_ERROR;
    }
    memcpy(task_prop->name,impl->name, sizeof(task_prop->name));
    task_prop->creator = 0; /* TODO */
    task_prop->stack_size = impl->thread->stackSize();
    task_prop->priority   = impl->thread->priority(); /*TODO Map from QT to OSAL */

    return OS_SUCCESS;
} /* end OS_TaskGetInfo_Impl */

/*----------------------------------------------------------------
 *
 * Function: OS_TaskIdMatchSystemData_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
bool OS_TaskIdMatchSystemData_Impl(void *ref, const OS_object_token_t *token, const OS_common_record_t *obj)
{
    /* TODO */
    return OS_ERR_NOT_IMPLEMENTED;
    // const pthread_t *               target = (const pthread_t *)ref;
    // OS_impl_task_internal_record_t *impl;

    // impl = OS_OBJECT_TABLE_GET(OS_impl_task_table, *token);

    // return (pthread_equal(*target, impl->id) != 0);
}

/*----------------------------------------------------------------
 *
 * Function: OS_TaskValidateSystemData_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_TaskValidateSystemData_Impl(const void *sysdata, size_t sysdata_size)
{
    if (sysdata == NULL || sysdata_size != sizeof(OSALThread))
    {
        return OS_INVALID_POINTER;
    }
    return OS_SUCCESS;
}

}


/*---------------------------------------------------------------------------------------
   Name: OS_QT_GetSchedulerParams

   Purpose: Helper function to get the details of the given OS scheduling policy.
            Determines if the policy is usable by OSAL - namely, that it provides
            enough priority levels to be useful.

   returns: true if policy is suitable for use by OSAL

    NOTES: Only used locally by task API initialization

---------------------------------------------------------------------------------------*/
static bool OS_QT_GetSchedulerParams(int sched_policy, QT_PriorityLimits_t *PriLim)
{

    /*
     * Set up the local Min/Max priority levels (varies by OS and scheduler policy)
     */
    /* 
    QThread::IdlePriority	0	scheduled only when no other threads are running.
    QThread::LowestPriority	1	scheduled less often than LowPriority.
    QThread::LowPriority	2	scheduled less often than NormalPriority.
    QThread::NormalPriority	3	the default priority of the operating system.
    QThread::HighPriority	4	scheduled more often than NormalPriority.
    QThread::HighestPriority	5	scheduled more often than HighPriority.
    QThread::TimeCriticalPriority	6	scheduled as often as possible.
    QThread::InheritPriority	7	use the same priority as the creating thread. This is the default.
    */
    PriLim->PriorityMax = QThread::TimeCriticalPriority;
    PriLim->PriorityMin = QThread::IdlePriority;

    /*
     * For OSAL, the absolute minimum spread between min and max must be 4.
     *
     * Although QT stipulates 32, we don't necessarily need that many, but we
     * also want to confirm that there is an acceptable spread.
     *
     * - Highest is reserved for the root task
     * - Next highest is reserved for OSAL priority=0 task(s)
     * - Lowest is reserved for OSAL priority=255 tasks(s)
     * - Need at least 1 for everything else.
     */
    if ((PriLim->PriorityMax - PriLim->PriorityMin) < 4)
    {
        OS_DEBUG("Policy %d: Insufficient spread between priority min-max: %d-%d\n", sched_policy,
                 (int)PriLim->PriorityMin, (int)PriLim->PriorityMax);
        return false;
    }

    /* If we get here, then the sched_policy is potentially valid */
    OS_DEBUG("Policy %d: available, min-max: %d-%d\n", sched_policy, (int)PriLim->PriorityMin,
             (int)PriLim->PriorityMax);
    return true;
} /* end OS_QT_GetSchedulerParams */

/*
 *********************************************************************************
 *          TASK API
 *********************************************************************************
 */

/*---------------------------------------------------------------------------------------
   Name: OS_QT_TaskAPI_Impl_Init

   Purpose: Initialize the QT Task data structures

 ----------------------------------------------------------------------------------------*/
int32 OS_QT_TaskAPI_Impl_Init(void)
{
    int                    ret;
    int                    sig;
    struct sched_param     sched_param;
    int                    sched_policy;
    QT_PriorityLimits_t sched_fifo_limits;
    bool                   sched_fifo_valid;
    QT_PriorityLimits_t sched_rr_limits;
    bool                   sched_rr_valid;

    /* Initialize Local Tables */
    memset(OS_impl_task_table, 0, sizeof(OS_impl_task_table));

    /* Clear the "limits" structs otherwise the compiler may warn
     * about possibly being used uninitialized (false warning)
     */
    memset(&sched_fifo_limits, 0, sizeof(sched_fifo_limits));
    memset(&sched_rr_limits, 0, sizeof(sched_rr_limits));

    /*
     * Create the key used to store OSAL task IDs
     * TODO
     */
    // ret = pthread_key_create(&QT_GlobalVars.ThreadKey, NULL);
    // if (ret != 0)
    // {
    //     OS_DEBUG("Error creating thread key: %s (%d)\n", strerror(ret), ret);
    //     return OS_ERROR;
    // }

    /*
    ** Disable Signals to parent thread and therefore all
    ** child threads create will block all signals
    ** Note: Timers will not work in the application unless
    **       threads are spawned in OS_Application_Startup.
    ** TODO Fix for windows
    */
    sigfillset(&QT_GlobalVars.MaximumSigMask);

    /*
     * Keep these signals unblocked so the process can be interrupted
     */
    sigdelset(&QT_GlobalVars.MaximumSigMask, SIGINT);  /* CTRL+C */
    sigdelset(&QT_GlobalVars.MaximumSigMask, SIGABRT); /* Abort */

    /*
     * One should not typically block ANY of the synchronous error
     * signals, i.e. SIGSEGV, SIGFPE, SIGILL, SIGBUS
     *
     * The kernel generates these signals in response to hardware events
     * and they get routed to the _specific thread_ that was executing when
     * the problem occurred.
     *
     * While it is technically possible to block these signals, the result is
     * undefined, and it makes debugging _REALLY_ hard.  If the kernel ever does
     * send one it means there really is a major problem, best to listen to it,
     * and not ignore it.
     */
    sigdelset(&QT_GlobalVars.MaximumSigMask, SIGSEGV); /* Segfault */
    sigdelset(&QT_GlobalVars.MaximumSigMask, SIGILL);  /* Illegal instruction */
    sigdelset(&QT_GlobalVars.MaximumSigMask, SIGBUS);  /* Bus Error */
    sigdelset(&QT_GlobalVars.MaximumSigMask, SIGFPE);  /* Floating Point Exception */

    /*
     * Set the mask and store the original (default) mask in the QT_GlobalVars.NormalSigMask
     */
    sigprocmask(SIG_SETMASK, &QT_GlobalVars.MaximumSigMask, &QT_GlobalVars.NormalSigMask);

    /*
     * Add all "RT" signals into the QT_GlobalVars.NormalSigMask
     * This will be used for the signal mask of the main thread
     * (This way it will end up as the default/original signal mask plus all RT sigs)
     */
    #ifdef SIGRTMIN
    for (sig = SIGRTMIN; sig <= SIGRTMAX; ++sig)
    {
        sigaddset(&QT_GlobalVars.NormalSigMask, sig);
    }
    #endif

    /*
     * SIGHUP is used to wake up the main thread when necessary,
     * so make sure it is NOT in the set.
     */
    sigdelset(&QT_GlobalVars.NormalSigMask, SIGHUP);

    /*
    ** Install noop as the signal handler for SIGUP.
    */
    signal(SIGHUP, OS_NoopSigHandler);

    /*
    ** Raise the priority of the current (main) thread so that subsequent
    ** application initialization will complete.  This had previously been
    ** done by the BSP and but it is moved here.
    **
    ** This will only work if the user owning this process has permission
    ** to create real time threads.  Otherwise, the default priority will
    ** be retained.  Typically this is only the root user, but finer grained
    ** permission controls are out there.  So if it works, great, but if
    ** a permission denied error is generated, that is OK too - this allows
    ** easily debugging code as a normal user.
    ** TODO Ignoring for QT for now
    */
//    ret = pthread_getschedparam(pthread_self(), &sched_policy, &sched_param);
    ret = -1;
    if (ret == 0)
    {
        #if 0
        QT_GlobalVars.SelectedRtScheduler = sched_policy; /* Fallback/default */
        do
        {
            sched_fifo_valid = OS_QT_GetSchedulerParams(SCHED_FIFO, &sched_fifo_limits);
            sched_rr_valid   = OS_QT_GetSchedulerParams(SCHED_RR, &sched_rr_limits);

            /*
             * If both policies are valid, choose the best. In general, FIFO is preferred
             * since it is simpler.
             *
             * But, RR is preferred if mapping several OSAL priority levels into the
             * same local priority level. For instance, if 2 OSAL tasks are created at priorities
             * "2" and "1", both may get mapped to local priority 98, and if using FIFO then the
             * task at priority "2" could run indefinitely, never letting priority "1" execute.
             *
             * This violates the original intent, which would be to have priority "1" preempt
             * priority "2" tasks.  RR is less bad since it at least guarantees both tasks some
             * CPU time,
             */
            if (sched_fifo_valid && sched_rr_valid)
            {
                /*
                 * If the spread from min->max is greater than what OSAL actually needs,
                 * then FIFO is the preferred scheduler.  Must take into account one extra level
                 * for the root task.
                 */
                if ((sched_fifo_limits.PriorityMax - sched_fifo_limits.PriorityMin) > OS_MAX_TASK_PRIORITY)
                {
                    sched_policy               = SCHED_FIFO;
                    QT_GlobalVars.PriLimits = sched_fifo_limits;
                }
                else
                {
                    sched_policy               = SCHED_RR;
                    QT_GlobalVars.PriLimits = sched_rr_limits;
                }
            }
            else if (sched_fifo_valid)
            {
                /* only FIFO is available */
                sched_policy               = SCHED_FIFO;
                QT_GlobalVars.PriLimits = sched_fifo_limits;
            }
            else if (sched_rr_valid)
            {
                /* only RR is available */
                sched_policy               = SCHED_RR;
                QT_GlobalVars.PriLimits = sched_rr_limits;
            }
            else
            {
                /* Nothing is valid, use default */
                break;
            }

            /*
             * This OSAL QT implementation will reserve the absolute highest priority
             * for the root thread, which ultimately will just pend in sigsuspend() so
             * it will not actually DO anything, except if sent a signal.  This way,
             * that thread will still be able to preempt a high-priority user thread that
             * has gone awry (i.e. using 100% cpu in FIFO mode).
             */
            sched_param.sched_priority = QT_GlobalVars.PriLimits.PriorityMax;
            --QT_GlobalVars.PriLimits.PriorityMax;

            OS_DEBUG("Selected policy %d for RT tasks, root task = %d\n", sched_policy,
                     (int)sched_param.sched_priority);

            /*
             * If the spread from min->max is greater than what OSAL actually needs,
             * then truncate it at the number of OSAL priorities.  This will end up mapping 1:1.
             * and leaving the highest priority numbers unused.
             */
            if ((QT_GlobalVars.PriLimits.PriorityMax - QT_GlobalVars.PriLimits.PriorityMin) >
                OS_MAX_TASK_PRIORITY)
            {
                QT_GlobalVars.PriLimits.PriorityMax = QT_GlobalVars.PriLimits.PriorityMin + OS_MAX_TASK_PRIORITY;
            }

            ret = pthread_setschedparam(pthread_self(), sched_policy, &sched_param);
            if (ret != 0)
            {
                OS_DEBUG("Could not setschedparam in main thread: %s (%d)\n", strerror(ret), ret);
                break;
            }

            /*
             * Set the boolean to indicate that "setschedparam" worked --
             * This means that it is also expected to work for future calls.
             */
            QT_GlobalVars.SelectedRtScheduler  = sched_policy;
            QT_GlobalVars.EnableTaskPriorities = true;

        } while (0);
        #endif
    }
    else
    {
        OS_DEBUG("Could not getschedparam in main thread: %s (%d)\n", strerror(ret), ret);
    }

#if !defined(OSAL_CONFIG_DEBUG_PERMISSIVE_MODE)
    /*
     * In strict (non-permissive) mode, if the task priority setting did not work, fail with an error.
     * This would be used on a real target where it needs to be ensured that priorities are active
     * and the "silent fallback" of debug mode operation is not desired.
     */
    if (!QT_GlobalVars.EnableTaskPriorities)
    {
        return OS_ERROR;
    }
#endif

    QT_GlobalVars.PageSize = sysconf(_SC_PAGESIZE);

    return OS_SUCCESS;
} /* end OS_QT_TaskAPI_Impl_Init */

/*----------------------------------------------------------------
 *
 * Function: OS_QT_InternalTaskCreate_Impl
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_QT_InternalTaskCreate_Impl(OS_impl_task_internal_record_t *ost, osal_priority_t priority, size_t stacksz,
                                       PthreadFuncPtr_t entry, void *entry_arg)
{
    int  return_code = OS_SUCCESS;
    static int idCounter = 0;
    idCounter++;
    // pthread_attr_t     custom_attr;
    // struct sched_param priority_holder;

    /*
     ** Initialize the pthread_attr structure.
     ** The structure is used to set the stack and priority
     */
    // memset(&custom_attr, 0, sizeof(custom_attr));
    // return_code = pthread_attr_init(&custom_attr);
    // if (return_code != 0)
    // {
    //     OS_DEBUG("pthread_attr_init error in OS_TaskCreate: %s\n", strerror(return_code));
    //     return (OS_ERROR);
    // }

    /*
     * Adjust the stack size parameter.
     *
     * QT has additional restrictions/limitations on the stack size of tasks that
     * other RTOS environments may not have.  Specifically QT says that the stack
     * size must be at least PTHREAD_STACK_MIN and may also need to be a multiple of the
     * system page size.
     *
     * Rounding up means the user might get a bigger stack than they requested, but
     * that should not break anything aside from consuming extra memory.
     */
    if (stacksz < PTHREAD_STACK_MIN)
    {
        stacksz = PTHREAD_STACK_MIN;
    }

    stacksz += QT_GlobalVars.PageSize - 1;
    stacksz -= stacksz % QT_GlobalVars.PageSize;

    ost->thread = new OSALThread();
    if(ost->thread == 0x0)
    {
        return OS_ERR_BAD_ADDRESS;
    }
    /*
    ** Set the Stack Size
    */
   if(stacksz > 0)
        ost->thread->setStackSize(stacksz);

    /*
    ** Test to see if the original main task scheduling priority worked.
    ** If so, then also set the attributes for this task.  Otherwise attributes
    ** are left at default.
    */
    if (QT_GlobalVars.EnableTaskPriorities)
    {

        /*
        ** Set the scheduling inherit attribute to EXPLICIT
        */
        // return_code = pthread_attr_setinheritsched(&custom_attr, PTHREAD_EXPLICIT_SCHED);
        // if (return_code != 0)
        // {
        //     OS_DEBUG("pthread_attr_setinheritsched error in OS_TaskCreate, errno = %s\n", strerror(return_code));
        //     return (OS_ERROR);
        // }

        /*
        ** Set the scheduling policy
        ** The best policy is determined during initialization
        */
        // return_code = pthread_attr_setschedpolicy(&custom_attr, QT_GlobalVars.SelectedRtScheduler);
        // if (return_code != 0)
        // {
        //     OS_DEBUG("pthread_attr_setschedpolity error in OS_TaskCreate: %s\n", strerror(return_code));
        //     return (OS_ERROR);
        // }

        /*
        ** Set priority TODO ? 
        */
        // return_code = pthread_attr_getschedparam(&custom_attr, &priority_holder);
        // if (return_code != 0)
        // {
        //     OS_DEBUG("pthread_attr_getschedparam error in OS_TaskCreate: %s\n", strerror(return_code));
        //     return (OS_ERROR);
        // }

        // priority_holder.sched_priority = OS_PriorityRemap(priority);
        // return_code                    = pthread_attr_setschedparam(&custom_attr, &priority_holder);
        // if (return_code != 0)
        // {
        //     OS_DEBUG("pthread_attr_setschedparam error in OS_TaskCreate: %s\n", strerror(return_code));
        //     return (OS_ERROR);
        // }

    } /* End if user is root */

    /*
     ** Create thread
     */
    ost->thread->entry =entry;
    ost->thread->data = entry_arg;


    /*
     ** Free the resources that are no longer needed
     ** Since the task is now running - pthread_create() was successful -
     ** Do not treat anything bad that happens after this point as fatal.
     ** The task is running, after all - better to leave well enough alone.
     */
    // return_code = pthread_detach(*pthr);
    // if (return_code != 0)
    // {
    //     OS_DEBUG("pthread_detach error in OS_TaskCreate: %s\n", strerror(return_code));
    // }

    // return_code = pthread_attr_destroy(&custom_attr);
    // if (return_code != 0)
    // {
    //     OS_DEBUG("pthread_attr_destroy error in OS_TaskCreate: %s\n", strerror(return_code));
    // }

    return return_code;
} /* end OS_QT_InternalTaskCreate_Impl */
