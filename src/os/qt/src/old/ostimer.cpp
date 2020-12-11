#include "osconfig.h"
#include "osapi.h"
#include "osapi-os-timer.h"
#include <QtCore>
#include "osconfig.h"


class OSALTimer : public QObject{
public:
    std::string name;
    uint32 id;
    int startID;
    OS_TimerCallback_t callback_ptr;
    uint32 data;
    void timerEvent(QTimerEvent *event){
        if(callback_ptr != 0x0)
            callback_ptr(data);
    }
};

OSALTimer *timer_table[OS_MAX_TIMERS];
/*
** Timer API
*/
int32  OS_TimerAPIInit          (void){
    memset(&timer_table,0x0,sizeof(timer_table));
}

int32 OS_GetTimer(uint32 id, OSALTimer ** timer)
{
    if(id > OS_MAX_TIMERS)
        return OS_ERROR;
    *timer = timer_table[id];
    if(*timer == 0x0)
        return OS_ERROR;
    return OS_SUCCESS;
}

int32 OS_TimerCreate            (uint32 *timer_id, const char *timer_name, uint32 *clock_accuracy, OS_TimerCallback_t callback_ptr){
    int i = 0;
    OSALTimer *timer = 0x0;
    for(i=0;i<OS_MAX_TIMERS;i++)
    {
        if(timer_table[i] == 0x0)
            break;
    }
    if(timer_table[i] != 0x0 || i == OS_MAX_TIMERS)
    {
        return OS_ERROR;
    }
    timer_table[i] = timer = new OSALTimer();
    timer->id = i;
    timer->callback_ptr = callback_ptr;
    timer->name = timer_name;
    *clock_accuracy = 1000; /* TODO What is clock accuracy Number of microseconds ???*/

}
int32 OS_TimerSet               (uint32 timer_id, uint32 start_time, uint32 interval_time){
    OSALTimer * timer;
    if(OS_GetTimer(timer_id, &timer) != OS_SUCCESS)
        return OS_ERROR;
    timer->startID = timer->startTimer(interval_time/1000);

}
int32 OS_TimerDelete            (uint32 timer_id)
{
    OSALTimer * timer;
    if(OS_GetTimer(timer_id, &timer) != OS_SUCCESS)
        return OS_ERROR;
    timer->killTimer(timer->startID);
    delete timer;
    timer_table[timer_id] = 0x0;

}
int32 OS_TimerGetIdByName       (uint32 *timer_id, const char *timer_name){
    return OS_ERR_NOT_IMPLEMENTED;
}
int32 OS_TimerGetInfo           (uint32  timer_id, OS_timer_prop_t *timer_prop){
    return OS_ERR_NOT_IMPLEMENTED;
}


/****************************************************************
 * TIME BASE API
 *
 * This is not implemented by this OSAL, so return "OS_ERR_NOT_IMPLEMENTED"
 * for all calls defined by this API.  This is necessary for forward
 * compatibility (runtime code can check for this return code and use
 * an alternative API where appropriate).
 */

int32 OS_TimeBaseCreate(uint32 *timer_id, const char *timebase_name, OS_TimerSync_t external_sync)
{

    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseSet(uint32 timer_id, uint32 start_time, uint32 interval_time)
{

    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseDelete(uint32 timer_id)
{

    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimeBaseGetIdByName (uint32 *timer_id, const char *timebase_name)
{

    return OS_ERR_NOT_IMPLEMENTED;
}

int32 OS_TimerAdd(uint32 *timer_id, const char *timer_name, uint32 timebase_id, OS_ArgCallback_t  callback_ptr, void *callback_arg)
{

    return OS_ERR_NOT_IMPLEMENTED;
}
