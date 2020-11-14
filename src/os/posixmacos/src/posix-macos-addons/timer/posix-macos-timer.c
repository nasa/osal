#include "posix-macos-timer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>

typedef enum
{
    STOPPED = 0,
    STARTED = 1
} timer_state_t;

typedef struct timer_instance_t
{
    dispatch_source_t    source;
    dispatch_semaphore_t semaphore;
    timer_state_t        state;
} timer_instance_t;

static long timespec_to_ns(const struct timespec *value)
{
    return value->tv_sec * NSEC_PER_SEC + value->tv_nsec;
}

int timer_create(clockid_t clockid, struct sigevent *sevp, timer_t *timerid)
{
    assert(clockid == CLOCK_DUMMY && "Passing CLOCK_REALTIME or CLOCK_MONOTONIC has no effect. Use CLOCK_DUMMY.");
    if (timerid == NULL)
    {
        return EINVAL;
    }

    timer_instance_t *timer_entry = malloc(sizeof(struct timer_instance_t));

    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);

    // Configure the timer queue to have the highest priority.
    dispatch_queue_attr_t priorityAttribute =
        dispatch_queue_attr_make_with_qos_class(DISPATCH_QUEUE_SERIAL, QOS_CLASS_USER_INTERACTIVE, -1);
    dispatch_queue_t queue = dispatch_queue_create("timerQueue", priorityAttribute);

    dispatch_source_t new_timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);

    dispatch_source_set_event_handler(new_timer, ^{ dispatch_semaphore_signal(semaphore); });

    dispatch_source_set_cancel_handler(new_timer, ^{
      /// BUG IN CLIENT OF LIBDISPATCH: Semaphore object deallocated while in use
      /// This should never happen but it does in CFS OSAL tests for POSIX.
      /// Putting this additional dispatch_semaphore_signal for now because it
      /// makes the tests pass.
      /// https://stackoverflow.com/questions/8287621/why-does-this-code-cause-exc-bad-instruction
      dispatch_semaphore_signal(semaphore);

      dispatch_release(new_timer);
      dispatch_release(queue);
      dispatch_release(semaphore);

      timer_entry->state = STOPPED;
    });

    timer_entry->source    = new_timer;
    timer_entry->semaphore = semaphore;

    *timerid = timer_entry;

    return 0;
}

int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value, struct itimerspec *old_value)
{
    assert(timerid);
    assert(flags == 0 && "No flags are supported");

    int64_t value_ns    = timespec_to_ns(&new_value->it_value);
    int64_t interval_ns = timespec_to_ns(&new_value->it_interval);

    dispatch_time_t start = dispatch_time(DISPATCH_TIME_NOW, value_ns);
    dispatch_source_set_timer(timerid->source, start, interval_ns, 0);

    /// Setting up a timer that is already set happens in NASA cFS tests and this
    /// results in:
    /// 	movq   %rax, 0x368ad889(%rip)
    ///	ud2
    ///	leaq   0x28695(%rip), %rcx        ; "BUG IN CLIENT OF LIBDISPATCH: Over-resume of an inactive object"
    ///
    ///  * Dispatch objects can be suspended with dispatch_suspend(), which increments
    ///  * an internal suspension count. dispatch_resume() is the inverse operation,
    ///  * and consumes suspension counts. When the last suspension count is consumed,
    ///  * blocks associated with the object will be invoked again.
    ///  The not so nice solution is to introduce a state flag and use it for
    ///  doing a suspend when the timer is already running.
    if (timerid->state != STARTED)
    {
        timerid->state = STARTED;
    }
    else
    {
        dispatch_suspend(timerid->source);
    }
    dispatch_resume(timerid->source);

    return 0;
}

int timer_delete(timer_t timerid)
{
    assert(timerid);
    dispatch_source_cancel(timerid->source);
    free(timerid);
    return 0;
}

int timer_poll(timer_t timerid)
{
    assert(timerid);
    dispatch_semaphore_wait(timerid->semaphore, DISPATCH_TIME_FOREVER);
    return 0;
}
