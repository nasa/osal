#include "mqueue.h"

#include <assert.h>
#include <time.h>
#include <stdint.h>

int mq_send(mqd_t mqd, const char *ptr, size_t len, unsigned int prio)
{
    assert(sizeof(((struct timespec *)0)->tv_sec) == 8);

    struct timespec distant_future;
    distant_future.tv_nsec = 0;
    distant_future.tv_sec  = (int64_t)(UINT64_MAX / 2);

    return mq_timedsend(mqd, ptr, len, prio, &distant_future);
}
