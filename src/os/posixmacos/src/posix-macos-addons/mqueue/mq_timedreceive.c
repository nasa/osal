#include "mqueue.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>

ssize_t mq_timedreceive(mqd_t mqd, char *ptr, size_t maxlen, unsigned *priop, const struct timespec *abs_timeout)
{
    int               n;
    long              index;
    int8_t *          mptr;
    ssize_t           len;
    struct mq_hdr *   mqhdr;
    struct mq_attr *  attr;
    struct mymsg_hdr *msghdr;
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

    if (maxlen < attr->mq_msgsize)
    {
        errno = EMSGSIZE;
        goto err;
    }
    if (attr->mq_curmsgs == 0)
    { /* queue is empty */
        if (mqinfo->mqi_flags & O_NONBLOCK)
        {
            errno = EAGAIN;
            goto err;
        }
        /* wait for a message to be placed onto queue */
        mqhdr->mqh_nwait++;
        while (attr->mq_curmsgs == 0)
        {
            int wait_result = pthread_cond_timedwait(&mqhdr->mqh_wait, &mqhdr->mqh_lock, abs_timeout);
            if (wait_result == ETIMEDOUT)
            {
                errno = ETIMEDOUT;
                goto err;
            }
            assert(wait_result == 0);
        }
        mqhdr->mqh_nwait--;
    }

    if ((index = mqhdr->mqh_head) == 0)
    {
        printf("mq_receive: curmsgs = %ld; head = 0\n", attr->mq_curmsgs);
    }

    msghdr          = (struct mymsg_hdr *)&mptr[index];
    mqhdr->mqh_head = msghdr->msg_next; /* new head of list */
    len             = msghdr->msg_len;
    memcpy(ptr, msghdr + 1, len); /* copy the message itself */
    if (priop != NULL)
        *priop = msghdr->msg_prio;

    /* just-read message goes to front of free list */
    msghdr->msg_next = mqhdr->mqh_free;
    mqhdr->mqh_free  = index;

    /* wake up anyone blocked in mq_send waiting for room */
    if (attr->mq_curmsgs == attr->mq_maxmsg)
    {
        assert(pthread_cond_signal(&mqhdr->mqh_wait) == 0);
    }
    attr->mq_curmsgs--;

    assert(pthread_mutex_unlock(&mqhdr->mqh_lock) == 0);
    return (len);

err:
    pthread_mutex_unlock(&mqhdr->mqh_lock);
    return (-1);
}
