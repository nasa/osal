#include "posix-macos-semaphore2.h"

#include <assert.h>
#include <errno.h>
#include <time.h>

static void timespec_diff(const struct timespec *lhs, const struct timespec *rhs, struct timespec *out)
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

int mac_sem2_init(mac_sem2_t *psem, int flags, unsigned count)
{
    // TODO: handle return value
    *psem = dispatch_semaphore_create(count);
    return 0;
}

int mac_sem2_destroy(mac_sem2_t *psem)
{
    dispatch_release(*psem);
    return 0;
}

int mac_sem2_post(mac_sem2_t *psem)
{
    // TODO: handle return value
    dispatch_semaphore_signal(*psem);
    return 0;
}

int mac_sem2_trywait(mac_sem2_t *psem)
{
    int result;
    result = dispatch_semaphore_wait(*psem, DISPATCH_TIME_NOW);
    if (result != 0)
    {
        errno = ETIMEDOUT;
        return -1;
    }
    return 0;
}

int mac_sem2_wait(mac_sem2_t *psem)
{
    int result = dispatch_semaphore_wait(*psem, DISPATCH_TIME_FOREVER);
    if (result != 0)
    {
        errno = ETIMEDOUT;
        return -1;
    }
    return 0;
}

int mac_sem2_timedwait(mac_sem2_t *psem, const struct timespec *abstim)
{
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    struct timespec diff;
    timespec_diff(&now, abstim, &diff);

    long            diff_ns = diff.tv_sec * NSEC_PER_SEC + diff.tv_nsec;
    dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, diff_ns);

    int result = dispatch_semaphore_wait(*psem, timeout);
    if (result != 0)
    {
        errno = ETIMEDOUT;
        return -1;
    }
    return 0;
}

int mac_sem2_getvalue(mac_sem2_t *sem, int *sval)
{
    assert(0 && "Not implemented");
    return 0;
}
