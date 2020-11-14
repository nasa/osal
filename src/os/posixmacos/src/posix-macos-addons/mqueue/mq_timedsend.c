#include "mqueue.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>

int mq_timedsend(mqd_t mqd, const char *ptr, size_t len, unsigned prio, const struct timespec *abs_timeout)
{
    int               n;
    long              index, freeindex;
    int8_t *          mptr;
    struct sigevent * sigev;
    struct mq_hdr *   mqhdr;
    struct mq_attr *  attr;
    struct mymsg_hdr *msghdr, *nmsghdr, *pmsghdr;
    struct mq_info *  mqinfo;

    mqinfo = mqd;
    if (mqinfo->mqi_magic != MQI_MAGIC)
    {
        errno = EBADF;
        return (-1);
    }
    mqhdr = mqinfo->mqi_hdr; /* struct pointer */
    mptr  = (int8_t *)mqhdr; /* byte pointer */
    attr  = &mqhdr->mqh_attr;

    if ((n = pthread_mutex_lock(&mqhdr->mqh_lock)) != 0)
    {
        errno = n;
        return (-1);
    }

    if (len > attr->mq_msgsize)
    {
        errno = EMSGSIZE;
        goto err;
    }
    if (attr->mq_curmsgs == 0)
    {
        if (mqhdr->mqh_pid != 0 && mqhdr->mqh_nwait == 0)
        {
            sigev = &mqhdr->mqh_event;
            if (sigev->sigev_notify == SIGEV_SIGNAL)
            {
                /// sigqueue does not exit on macOS but it looks like it is enough if we
                /// just send a signal with kill to make simple tests pass.
                /// If a user does not use mq_notify this can be considered an unused
                /// branch.
                /// sigqueue(mqhdr->mqh_pid, sigev->sigev_signo, sigev->sigev_value);
                kill(mqhdr->mqh_pid, sigev->sigev_signo);
            }
            mqhdr->mqh_pid = 0; /* unregister */
        }
    }
    else if (attr->mq_curmsgs >= attr->mq_maxmsg)
    {
        /* queue is full */
        if (mqinfo->mqi_flags & O_NONBLOCK)
        {
            errno = EAGAIN;
            goto err;
        }
        /* wait for room for one message on the queue */
        while (attr->mq_curmsgs >= attr->mq_maxmsg)
        {
            int wait_result = pthread_cond_timedwait(&mqhdr->mqh_wait, &mqhdr->mqh_lock, abs_timeout);

            if (wait_result == ETIMEDOUT)
            {
                errno = ETIMEDOUT;
                goto err;
            }
        }
    }

    /* nmsghdr will point to new message */
    if ((freeindex = mqhdr->mqh_free) == 0)
    {
        printf("mq_send: curmsgs = %ld; free = 0\n", attr->mq_curmsgs);
    }
    nmsghdr           = (struct mymsg_hdr *)&mptr[freeindex];
    nmsghdr->msg_prio = prio;
    nmsghdr->msg_len  = len;
    memcpy(nmsghdr + 1, ptr, len);       /* copy message from caller */
    mqhdr->mqh_free = nmsghdr->msg_next; /* new freelist head */

    /* find right place for message in linked list */
    index   = mqhdr->mqh_head;
    pmsghdr = (struct mymsg_hdr *)&(mqhdr->mqh_head);
    while (index != 0)
    {
        msghdr = (struct mymsg_hdr *)&mptr[index];
        if (prio > msghdr->msg_prio)
        {
            nmsghdr->msg_next = index;
            pmsghdr->msg_next = freeindex;
            break;
        }
        index   = msghdr->msg_next;
        pmsghdr = msghdr;
    }
    if (index == 0)
    {
        /* queue was empty or new goes at end of list */
        pmsghdr->msg_next = freeindex;
        nmsghdr->msg_next = 0;
    }
    /* wake up anyone blocked in mq_receive waiting for a message */
    if (attr->mq_curmsgs == 0)
    {
        pthread_cond_signal(&mqhdr->mqh_wait);
    }
    attr->mq_curmsgs++;

    pthread_mutex_unlock(&mqhdr->mqh_lock);
    return (0);

err:
    pthread_mutex_unlock(&mqhdr->mqh_lock);
    return (-1);
}
