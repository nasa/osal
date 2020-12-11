#include <QtCore>
//#include "osconfig.h"
#include "osapi.h"
class OSALThread: public QThread
{
    public:
        osal_task_entry function_pointer;
        void * data;
        osal_task_entry delete_handler;
        uint32 task_id;
        uint32 creator;
        /*!
         * Origianl priority. Qt squashes to 0 to 6
         */
        uint32 priority;
        std::string task_name;
        uint32 flags;
        /* Unused stack pointer */
        const uint32 *stack_pointer;

        void run() override {
            if(function_pointer != 0x0)
            {
                function_pointer( );
            }
        }

};


OSALThread  *OS_task_table [OS_MAX_TASKS] = {0x0};



/*
** Task API
*/

int32 OS_TaskCreate            (uint32 *task_id, const char *task_name,
                                osal_task_entry function_pointer,
                                const uint32 *stack_pointer,
                                uint32 stack_size,
                                uint32 priority, uint32 flags)
{
    OSALThread * new_task;
    int i;
    for(i=0;i<OS_MAX_TASKS;i++)
    {
        new_task = OS_task_table[i];
        if(new_task == 0x0)
            break;
    }

    OS_task_table[i] = new_task = new OSALThread();
    if(new_task == 0x0)
        return OS_ERROR;
    *task_id = new_task->task_id = i;
    new_task->priority = priority;
    new_task->task_name = task_name;
    new_task->flags = flags;
    new_task->data = 0x0;
    OSALThread * creator = dynamic_cast< OSALThread *>(QThread::currentThread() );
    if(creator == 0x0)
        new_task->creator = -1;
    new_task->creator = creator->task_id;


    OS_TaskSetPriority(i,priority);


    /*!
     * Warning: Most operating systems place minimum and maximum limits on thread stack sizes.
     * The thread will fail to start if the stack size is outside these limits.
     */
    new_task->setStackSize(stack_size);
    new_task->stack_pointer = stack_pointer;
    new_task->function_pointer = function_pointer;
    new_task->start();
    return OS_SUCCESS;
}


void OS_TaskExit               (void){
    OSALThread * task =  (OSALThread*)QThread::currentThread();
    if(!task)
        return;

    task->quit();
    task->delete_handler();

}
int32 OS_TaskInstallDeleteHandler(osal_task_entry function_pointer){
    OSALThread * task =  (OSALThread*)QThread::currentThread();
    if(!task)
        return OS_ERROR;

    task->delete_handler = function_pointer;
    return OS_SUCCESS;
}
int32 OS_TaskDelay             (uint32 millisecond){
    bool status = false;
    status = QThread::currentThread()->wait(millisecond);
    if(status == false)
        return OS_ERROR_TIMEOUT;

    return OS_SUCCESS;
}
uint32 OS_FindCreator(void){

    OSALThread * task = dynamic_cast< OSALThread *>(QThread::currentThread() );
    if(task == 0x0)
        return -1;

    return task->creator;
}
static int32 OS_GetTask(uint32 task_id, OSALThread ** task){
//    if(task_id < 0)
//        return OS_ERROR;
    if(task_id >= OS_MAX_TASKS)
        return OS_ERROR;
    *task = OS_task_table[task_id];
    if(*task == 0x0)
        return OS_ERROR;
    return OS_SUCCESS;
}

int32 OS_TaskDelete            (uint32 task_id){
    OSALThread * task;
    if(OS_GetTask(task_id, &task) != OS_SUCCESS)
        return OS_ERROR;
    delete OS_task_table[task_id];
    OS_task_table[task_id] = 0x0;
    return OS_SUCCESS;
}

int32 OS_TaskSetPriority       (uint32 task_id, uint32 priority){
    OSALThread * new_task;

    if(OS_GetTask(task_id, &new_task) != OS_SUCCESS)
        return OS_ERROR;

    new_task->priority = priority;
    if(priority <= 0)
        priority = 0;
    if(priority > 6)
        priority = 6;
    QThread::Priority priorities[7] = {
            QThread::IdlePriority   ,
            QThread::LowestPriority ,
            QThread::LowPriority    ,
            QThread::NormalPriority ,
            QThread::HighPriority   ,
            QThread::HighestPriority    ,
            QThread::TimeCriticalPriority
    };
    new_task->setPriority(priorities[priority]);
    return OS_SUCCESS;
}
int32 OS_TaskRegister          (void){
    return OS_SUCCESS;
}
uint32 OS_TaskGetId            (void){
    OSALThread * task = (OSALThread*) QThread::currentThread();
    if(task == 0x0)
        return -1;
    return task->task_id;
}
int32 OS_TaskGetIdByName       (uint32 *task_id, const char *task_name){
    OSALThread * task;
    for(int i =0;i< OS_MAX_TASKS;i++){
        task = OS_task_table[i];
        if(task == 0x0)
            continue;
        if(task->task_name == task_name)
            break;
    }

    if(task->task_name != task_name)
        return OS_ERROR;

    *task_id = task->task_id;

    return OS_SUCCESS;
}
int32 OS_TaskGetInfo           (uint32 task_id, OS_task_prop_t *task_prop){
    OSALThread * task;
    if(OS_GetTask(task_id, &task) != OS_SUCCESS)
        return OS_ERROR;

    task_prop->OStask_id = task->task_id;
    task_prop->priority = task->priority;
    task_prop->stack_size = task->stackSize();
    task_prop->creator = 0;
    return OS_SUCCESS;
}

class OSALSemaphore : public QSemaphore{
public:
    std::string name;
    uint32 id;
    uint32 options;
    OSALSemaphore(uint32 count) : QSemaphore(count){

    }
};

OSALSemaphore *OS_sema_table[OS_MAX_BIN_SEMAPHORES];

/*
** Semaphore API
*/
int32 OS_GetBinSem(uint32 sem_id, OSALSemaphore ** sema)
{
    if(sem_id > OS_MAX_BIN_SEMAPHORES)
        return OS_ERROR;
    *sema = OS_sema_table[sem_id];
    if(*sema == 0x0)
        return OS_ERROR;
    return OS_SUCCESS;
}
int32 OS_BinSemCreate          (uint32 *sem_id, const char *sem_name,
                                uint32 sem_initial_value, uint32 options){
    int i =0;
    OSALSemaphore * sema = 0x0;
    for(i=0;i< OS_MAX_BIN_SEMAPHORES; i++)
    {
        if(OS_sema_table[i] == 0x0)
            break;
    }
    if(OS_sema_table[i] != 0x0)
        return OS_ERROR;
    sema = OS_sema_table[i] = new OSALSemaphore(1);
    sema->name = sem_name;
    *sem_id = sema->id = i;
    sema->options = options;
    if(sem_initial_value == 0)
        sema->acquire(1);

    return OS_SUCCESS;
}
int32 OS_BinSemFlush            (uint32 sem_id){
    OSALSemaphore * sema;
    if(OS_GetBinSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    if(sema->available() > 0)
        sema->release(1);
    return OS_SUCCESS;
}
int32 OS_BinSemGive            (uint32 sem_id)
{
    OSALSemaphore * sema;
    if(OS_GetBinSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    if(sema->available() == 0)
        sema->release(1);
    else
        return OS_ERROR;
    return OS_SUCCESS;
}
int32 OS_BinSemTake            (uint32 sem_id){
    OSALSemaphore * sema;
    if(OS_GetBinSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    sema->acquire(1);
    return OS_SUCCESS;
}
int32 OS_BinSemTimedWait       (uint32 sem_id, uint32 msecs){
    OSALSemaphore * sema;
    if(OS_GetBinSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    if(sema->tryAcquire(1,msecs) == true)
        return OS_SUCCESS;
    return OS_ERROR_TIMEOUT;
}
int32 OS_BinSemDelete          (uint32 sem_id){
    OSALSemaphore * sema;
    if(OS_GetBinSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    OS_sema_table[sema->id] = 0x0;
    OS_BinSemGive(sem_id);
    delete sema;
    return OS_SUCCESS;
}
int32 OS_BinSemGetIdByName     (uint32 *sem_id, const char *sem_name){
    int i =0;
    OSALSemaphore * sema;

    for(i=0;i< OS_MAX_BIN_SEMAPHORES;i++)
    {
        sema = OS_sema_table[i];
        if(strcmp(sem_name, sema->name.c_str()) == 0)
        {

            *sem_id = i;
            return OS_SUCCESS;

        }
    }
    return OS_ERROR;

}
int32 OS_BinSemGetInfo         (uint32 sem_id, OS_bin_sem_prop_t *bin_prop){
    OSALSemaphore * sema;
    if(OS_GetBinSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;

    bin_prop->creator = 0x0;
    strncpy( bin_prop->name, sema->name.c_str(),sizeof(bin_prop->name));
    bin_prop->value = sema->available();
    return OS_SUCCESS;
}


OSALSemaphore *OS_Csema_table[OS_MAX_BIN_SEMAPHORES];


/*
** Semaphore API
*/
int32 OS_GetCountSem(uint32 sem_id, OSALSemaphore ** sema)
{
    if(sem_id > OS_MAX_BIN_SEMAPHORES)
        return OS_ERROR;
    *sema = OS_Csema_table[sem_id];
    if(*sema == 0x0)
        return OS_ERROR;
    return OS_SUCCESS;
}
int32 OS_CountSemCreate          (uint32 *sem_id, const char *sem_name,
                                uint32 sem_initial_value, uint32 options){
    int i =0;
    OSALSemaphore * sema = 0x0;
    for(i=0;i< OS_MAX_BIN_SEMAPHORES; i++)
    {
        if(OS_Csema_table[i] == 0x0)
            break;
    }
    if(OS_Csema_table[i] != 0x0)
        return OS_ERROR;
    sema = OS_Csema_table[i] = new OSALSemaphore(sem_initial_value);
    sema->name = sem_name;
    *sem_id = sema->id = i;
    sema->options = options;


    return OS_SUCCESS;
}
int32 OS_CountSemFlush            (uint32 sem_id){
    OSALSemaphore * sema;
    if(OS_GetCountSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    if(sema->available() > 0)
        sema->release(1);
    return OS_SUCCESS;
}
int32 OS_CountSemGive            (uint32 sem_id)
{
    OSALSemaphore * sema;
    if(OS_GetCountSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;

    sema->release(1);

    return OS_SUCCESS;
}
int32 OS_CountSemTake            (uint32 sem_id){
    OSALSemaphore * sema;
    if(OS_GetCountSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    sema->acquire(1);
    return OS_SUCCESS;
}
int32 OS_CountSemTimedWait       (uint32 sem_id, uint32 msecs){
    OSALSemaphore * sema;
    if(OS_GetCountSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    if(sema->tryAcquire(1,msecs) == true)
        return OS_SUCCESS;
    return OS_ERROR_TIMEOUT;
}
int32 OS_CountSemDelete          (uint32 sem_id){
    OSALSemaphore * sema;
    if(OS_GetCountSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;
    OS_Csema_table[sema->id] = 0x0;
    OS_CountSemGive(sem_id);
    delete sema;
    return OS_SUCCESS;
}
int32 OS_CountSemGetIdByName     (uint32 *sem_id, const char *sem_name){
    int i =0;
    OSALSemaphore * sema;

    for(i=0;i< OS_MAX_BIN_SEMAPHORES;i++)
    {
        sema = OS_Csema_table[i];
        if(strcmp(sem_name, sema->name.c_str()) == 0)
        {

            *sem_id = i;
            return OS_SUCCESS;

        }
    }
    return OS_ERROR;

}
int32 OS_CountSemGetInfo         (uint32 sem_id, OS_bin_sem_prop_t *bin_prop){
    OSALSemaphore * sema;
    if(OS_GetCountSem(sem_id, & sema) != OS_SUCCESS)
        return OS_ERROR;

    bin_prop->creator = 0x0;
    strncpy( bin_prop->name, sema->name.c_str(),sizeof(bin_prop->name));
    bin_prop->value = sema->available();
    return OS_SUCCESS;
}


class OSAL_Mutex : public QMutex{
public:
    uint32 id;
    std::string name;
    uint32 options;
};
OSAL_Mutex *mutex_table[ OS_MAX_MUTEXES] ={0};

int32 OS_GetMutex(uint32 sem_id, OSAL_Mutex ** sema)
{
    if(sem_id > OS_MAX_MUTEXES)
        return OS_ERROR;
    *sema = mutex_table[sem_id];
    if(*sema == 0x0)
        return OS_ERROR;
    return OS_SUCCESS;
}

int32 OS_MutSemCreate           (uint32 *sem_id, const char *sem_name, uint32 options){
    OSAL_Mutex * mutex = 0x0;
    int i = 0;
    for(i=0;i<OS_MAX_MUTEXES;i++)
    {
        if(mutex_table[i] == 0x0)
            break;
    }
    if(mutex_table[i] != 0x0)
        return OS_ERROR;

    mutex = mutex_table[i] = new OSAL_Mutex();
    *sem_id = mutex->id = i;
    mutex->name = sem_name;
    mutex->options = options;

    return OS_SUCCESS;
}
int32 OS_MutSemGive             (uint32 sem_id){
    OSAL_Mutex * mutex = 0x0;
    if(OS_GetMutex(sem_id, &mutex) != OS_SUCCESS)
        return OS_INVALID_SEM_VALUE;
    mutex->unlock();
    return OS_SUCCESS;
}
int32 OS_MutSemTake             (uint32 sem_id){
    OSAL_Mutex * mutex = 0x0;
    if(OS_GetMutex(sem_id, &mutex) != OS_SUCCESS)
        return OS_INVALID_SEM_VALUE;
    mutex->lock();
    return OS_SUCCESS;
}
int32 OS_MutSemDelete           (uint32 sem_id){
    OSAL_Mutex * mutex = 0x0;
    if(OS_GetMutex(sem_id, &mutex) != OS_SUCCESS)
        return OS_INVALID_SEM_VALUE;
    mutex->unlock();
    delete mutex;
    mutex_table[sem_id] = 0x0;
    return OS_SUCCESS;
}
int32 OS_MutSemGetIdByName      (uint32 *sem_id, const char *sem_name){
    int i =0;
    OSAL_Mutex * sema;

    for(i=0;i< OS_MAX_MUTEXES;i++)
    {
        sema = mutex_table[i];
        if(strcmp(sem_name, sema->name.c_str()) == 0)
        {
            *sem_id = i;
            return OS_SUCCESS;
        }
    }
    return OS_ERROR;
}
int32 OS_MutSemGetInfo          (uint32 sem_id, OS_mut_sem_prop_t *mut_prop){
    OSAL_Mutex * mutex = 0x0;
    if(OS_GetMutex(sem_id, &mutex) != OS_SUCCESS)
        return OS_INVALID_SEM_VALUE;
    mut_prop->creator = 0;
    strncpy(mut_prop->name, mutex->name.c_str(), sizeof(mut_prop->name));
    return OS_SUCCESS;

}


/*
** OS Time/Tick related API
*/
QTime system_timer;

int32 OS_Milli2Ticks(uint32 milli_seconds)
{
    uint32 num_of_ticks;
    uint32 tick_duration_usec;

    tick_duration_usec = OS_Tick2Micros();
    num_of_ticks = ((milli_seconds * 1000) + tick_duration_usec - 1)/tick_duration_usec;

    return(num_of_ticks);
}
int32 OS_Tick2Micros           (void){
    /*
     * Returns the number of milliseconds that have elapsed since the last time
     * TODO Replace with system_timer.elapsed();
     */
    return system_timer.elapsed() * 1000;


}
int32  OS_GetLocalTime         (OS_time_t *time_struct){
    QTime time = QTime::currentTime();

    time_struct->seconds = time.hour() * 60 * 60;
    time_struct->seconds += time.minute() * 60;
    time_struct->seconds += time.second();

    time_struct->microsecs = time.msec() * 1000;
    return OS_SUCCESS;
}
int32  OS_SetLocalTime         (OS_time_t *time_struct){
    system_timer.restart();
    system_timer = system_timer.addSecs(time_struct->seconds);
    system_timer = system_timer.addMSecs(time_struct->microsecs/1000);
    return OS_SUCCESS;
}



/*---------------------------------------------------------------------------------------
** Name: OS_IntAttachHandler
**
** Purpose:
**      The call associates a specified C routine to a specified interrupt
**      number.Upon occurring of the InterruptNumber , the InerruptHandler
**      routine will be called and passed the parameter.
**
** Assumptions and Notes:
**
** Parameters:
**      InterruptNumber : The Interrupt Number that will cause the start of
**                        the ISR
**      InerruptHandler : The ISR associatd with this interrupt
**      parameter :The parameter that is passed to the ISR
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**     OS_SUCCESS on success
**     OS_INVALID_POINTER if the Interrupt handler pointer is NULL
---------------------------------------------------------------------------------------*/
int32 OS_IntAttachHandler  (uint32 InterruptNumber, osal_task_entry InterruptHandler, int32 parameter)
{
    if (InterruptHandler == NULL)
    {
        return OS_INVALID_POINTER;
    }
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*---------------------------------------------------------------------------------------
** Name: OS_IntUnlock
** Purpose:
**      Enable the interrupts.
**
** Assumptions and Notes:
**
** Parameters:
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntUnlock (int32 IntLevel)
{
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*---------------------------------------------------------------------------------------
** Name: OS_Intlock
** Purpose:
**      Disable the interrupts.
**
** Assumptions and Notes:
**
** Parameters:
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntLock ( void )
{
    return(OS_ERR_NOT_IMPLEMENTED) ;
}
/*---------------------------------------------------------------------------------------
** Name: OS_IntEnable
** Purpose:
**      Enables interrupts through Level
**
** Assumptions and Notes:
**
** Parameters:
**              Level - the interrupts to enable
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**      OS_SUCCESS
---------------------------------------------------------------------------------------*/
int32 OS_IntEnable(int32 Level)
{
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*---------------------------------------------------------------------------------------
** Name: OS_IntDisable
** Purpose:
**      Disables interrupts through Level
**
** Assumptions and Notes:
**
** Parameters:
**              Level - the interrupts to disable
**
** Global Inputs: None
**
** Global Outputs: None
**
**
** Return Values:
**      OS_SUCCESS
---------------------------------------------------------------------------------------*/

int32 OS_IntDisable(int32 Level)
{
    return(OS_ERR_NOT_IMPLEMENTED);
}

/*---------------------------------------------------------------------------------------
   Name: OS_HeapGetInfo

   Purpose: Return current info on the heap

   Parameters:

---------------------------------------------------------------------------------------*/
int32 OS_HeapGetInfo       (OS_heap_prop_t *heap_prop)
{
    if (heap_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }
    /*
    ** Not implemented yet
    */
    return (OS_ERR_NOT_IMPLEMENTED);
}

void * OS_Malloc(uint32 size)
{
    return malloc(size);
}

void OS_Free(void * pointer)
{
    free(pointer);
}
void OS_ApplicationExit(int32 Status){

    exit(-1);
}



/*
** Exported Functions
*/

/*
** Initialization of API
*/
int32 OS_API_Init (void){
    memset(&mutex_table,0x0,sizeof(mutex_table));
    memset(&OS_task_table,0x0,sizeof(OS_task_table));
    memset(&OS_sema_table,0x0,sizeof(OS_sema_table));
    memset(&OS_Csema_table,0x0,sizeof(OS_Csema_table));
    system_timer = QTime::currentTime();
    return OS_SUCCESS;
}

/*
** OS-specific background thread implementation - waits forever for events to occur.
**
** This should be called from the BSP main routine / initial thread after all other
** board / application initialization has taken place and all other tasks are running.
*/
void OS_IdleLoop (void){
    /* TODO Do somethign ? */
}

/*
** OS_DeleteAllObjects() provides a means to clean up all resources allocated by this
** instance of OSAL.  It would typically be used during an orderly shutdown but may also
** be helpful for testing purposes.
*/
void OS_DeleteAllObjects       (void){
    for(int i =0;i< OS_MAX_BIN_SEMAPHORES; i++)
    {
        OS_BinSemDelete(i);
    }
    for(int i =0;i< OS_MAX_TASKS; i++)
    {
        OS_TaskDelete(i);
    }
    for(int i =0;i< OS_MAX_COUNT_SEMAPHORES; i++)
    {
        OS_CountSemDelete(i);
    }
    for(int i =0;i< OS_MAX_MUTEXES; i++)
    {
        OS_MutSemDelete(i);
    }

}

/*
** OS_ApplicationShutdown() provides a means for a user-created thread to request the orderly
** shutdown of the whole system, such as part of a user-commanded reset command.
** This is preferred over e.g. ApplicationExit() which exits immediately and does not
** provide for any means to clean up first.
*/
void OS_ApplicationShutdown         (uint8 flag){
    OSALThread * task;
    for(int i=0;i<OS_MAX_TASKS;i++)
    {
        task = OS_task_table[i];
        if(task){
            task->exit();
        }
        OS_task_table[i] = 0;

    }
}
