#include "mqueue.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>

int mq_notify(mqd_t mqd, const struct sigevent *notification)
{
    int             n;
    pid_t           pid;
    struct mq_hdr * mqhdr;
    struct mq_info *mqinfo;

    mqinfo = mqd;
    if (mqinfo->mqi_magic != MQI_MAGIC)
    {
        errno = EBADF;
        return (-1);
    }
    mqhdr = mqinfo->mqi_hdr;

    {
        // TODO: queue-test deadlocks because there a queue is closed while still being used.
        // queue-test: a message queue gets closed while still being used #1164
        // https://github.com/nasa/osal/issues/1164
        if ((n = pthread_mutex_trylock(&mqhdr->mqh_lock)) == EBUSY)
        {
            (void)pthread_mutex_unlock(&mqhdr->mqh_lock);
        }
        pthread_mutex_unlock(&mqhdr->mqh_lock);
    }

    if ((n = pthread_mutex_lock(&mqhdr->mqh_lock)) != 0)
    {
        errno = n;
        return (-1);
    }

    pid = getpid();
    if (notification == NULL)
    {
        if (mqhdr->mqh_pid == pid)
        {
            mqhdr->mqh_pid = 0; /* unregister calling process */
        }                       /* no error if caller not registered */
    }
    else
    {
        if (mqhdr->mqh_pid != 0)
        {
            if (kill(mqhdr->mqh_pid, 0) != -1 || errno != ESRCH)
            {
                errno = EBUSY;
                goto err;
            }
        }
        mqhdr->mqh_pid   = pid;
        mqhdr->mqh_event = *notification;
    }
    pthread_mutex_unlock(&mqhdr->mqh_lock);
    return (0);

err:
    pthread_mutex_unlock(&mqhdr->mqh_lock);
    return (-1);
}
