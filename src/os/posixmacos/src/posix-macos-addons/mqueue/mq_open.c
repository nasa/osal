#include "mqueue.h"

#include "mqueue-internal.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_TRIES 10 /* for waiting for initialization */

/// TODO: CHECK THIS
#define va_mode_t int

struct mq_attr defattr = {0, 128, 1024, 0};

mqd_t mq_open(const char *pathname, int oflag, ...)
{
    int                 i, fd, nonblock, created, save_errno;
    long                msgsize, filesize, index;
    va_list             ap;
    mode_t              mode;
    int8_t *            mptr;
    struct stat         statbuff;
    struct mq_hdr *     mqhdr;
    struct mymsg_hdr *  msghdr;
    struct mq_attr *    attr;
    struct mq_info *    mqinfo;
    pthread_mutexattr_t mattr;
    pthread_condattr_t  cattr;

    created  = 0;
    nonblock = oflag & O_NONBLOCK;
    oflag &= ~O_NONBLOCK;
    mptr   = (int8_t *)MAP_FAILED;
    mqinfo = NULL;

    char fs_pathname[MQ_FS_NAME_MAX];
    if (mq_get_fs_pathname(pathname, fs_pathname) == EINVAL)
    {
        errno = EINVAL;
        return ((mqd_t)-1);
    };

again:
    if (oflag & O_CREAT)
    {
        va_start(ap, oflag); /* init ap to final named argument */
        mode = va_arg(ap, va_mode_t) & ~S_IXUSR;
        attr = va_arg(ap, struct mq_attr *);
        va_end(ap);

        /* open and specify O_EXCL and user-execute */
        fd = open(fs_pathname, oflag | O_EXCL | O_RDWR, mode | S_IXUSR);
        if (fd < 0)
        {
            if (errno == EEXIST && (oflag & O_EXCL) == 0)
                goto exists; /* already exists, OK */
            else
                return ((mqd_t)-1);
        }
        created = 1;
        /* first one to create the file initializes it */
        if (attr == NULL)
            attr = &defattr;
        else
        {
            if (attr->mq_maxmsg <= 0 || attr->mq_msgsize <= 0)
            {
                errno = EINVAL;
                goto err;
            }
        }

        /* calculate and set the file size */
        msgsize  = MSGSIZE(attr->mq_msgsize);
        filesize = sizeof(struct mq_hdr) + (attr->mq_maxmsg * (sizeof(struct mymsg_hdr) + msgsize));
        if (lseek(fd, filesize - 1, SEEK_SET) == -1)
            goto err;
        if (write(fd, "", 1) == -1)
            goto err;

        /* memory map the file */
        mptr = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mptr == MAP_FAILED)
            goto err;

        /* allocate one mq_info{} for the queue */
        /* *INDENT-OFF* */
        if ((mqinfo = malloc(sizeof(struct mq_info))) == NULL)
            goto err;
        /* *INDENT-ON* */
        mqinfo->mqi_hdr = mqhdr = (struct mq_hdr *)mptr;
        mqinfo->mqi_magic       = MQI_MAGIC;
        mqinfo->mqi_flags       = nonblock;

        /* initialize header at beginning of file */
        /* create free list with all messages on it */
        mqhdr->mqh_attr.mq_flags   = 0;
        mqhdr->mqh_attr.mq_maxmsg  = attr->mq_maxmsg;
        mqhdr->mqh_attr.mq_msgsize = attr->mq_msgsize;
        mqhdr->mqh_attr.mq_curmsgs = 0;
        mqhdr->mqh_nwait           = 0;
        mqhdr->mqh_pid             = 0;
        mqhdr->mqh_head            = 0;
        index                      = sizeof(struct mq_hdr);
        mqhdr->mqh_free            = index;
        for (i = 0; i < attr->mq_maxmsg - 1; i++)
        {
            msghdr = (struct mymsg_hdr *)&mptr[index];
            index += sizeof(struct mymsg_hdr) + msgsize;
            msghdr->msg_next = index;
        }
        msghdr           = (struct mymsg_hdr *)&mptr[index];
        msghdr->msg_next = 0; /* end of free list */

        /* initialize mutex & condition variable */
        if ((i = pthread_mutexattr_init(&mattr)) != 0)
            goto pthreaderr;
        pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
        i = pthread_mutex_init(&mqhdr->mqh_lock, &mattr);
        pthread_mutexattr_destroy(&mattr); /* be sure to destroy */
        if (i != 0)
            goto pthreaderr;

        if ((i = pthread_condattr_init(&cattr)) != 0)
            goto pthreaderr;
        pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
        i = pthread_cond_init(&mqhdr->mqh_wait, &cattr);
        pthread_condattr_destroy(&cattr); /* be sure to destroy */
        if (i != 0)
            goto pthreaderr;

        /* initialization complete, turn off user-execute bit */
        if (fchmod(fd, mode) == -1)
            goto err;
        close(fd);
        return ((mqd_t)mqinfo);
    }

exists:
    /* open the file then memory map */
    if ((fd = open(fs_pathname, O_RDWR)) < 0)
    {
        if (errno == ENOENT && (oflag & O_CREAT))
            goto again;
        goto err;
    }

    /* make certain initialization is complete */
    for (i = 0; i < MAX_TRIES; i++)
    {
        if (stat(fs_pathname, &statbuff) == -1)
        {
            if (errno == ENOENT && (oflag & O_CREAT))
            {
                close(fd);
                goto again;
            }
            goto err;
        }
        if ((statbuff.st_mode & S_IXUSR) == 0)
            break;
        sleep(1);
    }
    if (i == MAX_TRIES)
    {
        errno = ETIMEDOUT;
        goto err;
    }

    filesize = statbuff.st_size;
    mptr     = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mptr == MAP_FAILED)
        goto err;
    close(fd);

    /* allocate one mq_info{} for each open */
    if ((mqinfo = malloc(sizeof(struct mq_info))) == NULL)
        goto err;
    mqinfo->mqi_hdr   = (struct mq_hdr *)mptr;
    mqinfo->mqi_magic = MQI_MAGIC;
    mqinfo->mqi_flags = nonblock;
    return ((mqd_t)mqinfo);

pthreaderr:
    errno = i;

err:
    /* don't let following function calls change errno */
    save_errno = errno;
    if (created)
        unlink(fs_pathname);
    if (mptr != MAP_FAILED)
        munmap(mptr, filesize);
    if (mqinfo != NULL)
        free(mqinfo);
    close(fd);
    errno = save_errno;
    return ((mqd_t)-1);
}
