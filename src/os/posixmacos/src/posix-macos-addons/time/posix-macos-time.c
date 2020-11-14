#include "posix-macos-time.h"

#include <assert.h>
#include <errno.h>
#include <mach/clock_types.h>

void __timespec_diff(const struct timespec *lhs, const struct timespec *rhs, struct timespec *out)
{
    assert(lhs->tv_sec <= rhs->tv_sec);

    out->tv_sec  = rhs->tv_sec - lhs->tv_sec;
    out->tv_nsec = rhs->tv_nsec - lhs->tv_nsec;

    if (out->tv_sec < 0)
    {
        out->tv_sec  = 0;
        out->tv_nsec = 0;
        return;
    }

    if (out->tv_nsec < 0)
    {
        if (out->tv_sec == 0)
        {
            out->tv_sec  = 0;
            out->tv_nsec = 0;
            return;
        }

        out->tv_sec  = out->tv_sec - 1;
        out->tv_nsec = out->tv_nsec + NSEC_PER_SEC;
    }
}

int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *req, struct timespec *rem)
{

    assert(clock_id == CLOCK_REALTIME || clock_id == CLOCK_MONOTONIC);
    assert(0 <= req->tv_nsec && req->tv_nsec <= NSEC_PER_SEC);
    assert(flags == 0 || flags == TIMER_ABSTIME);
    assert(flags != TIMER_ABSTIME || clock_id == CLOCK_MONOTONIC);

    if (flags == TIMER_ABSTIME)
    {
        struct timespec now;
        struct timespec diff;

        if (clock_gettime(clock_id, &now) != 0)
        {
            return errno;
        }

        __timespec_diff(&now, req, &diff);

        return nanosleep(&diff, rem);
    }

    if (nanosleep(req, rem) != 0)
    {
        return errno;
    }

    return 0;
}
