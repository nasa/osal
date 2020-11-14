#include "posix-macos-semaphore.h"

#include <assert.h>
#include <errno.h>
#include <string.h>
#include <printf.h>

int mac_sem_init(mac_sem_t *psem, int flags, unsigned count)
{
    int result;

    if (psem == NULL)
    {
        return -1;
    }
    result = pthread_mutex_init(&psem->count_lock, NULL);
    if (result != 0)
    {
        return result;
    }
    result = pthread_cond_init(&psem->count_bump, NULL);
    if (result != 0)
    {
        pthread_mutex_destroy(&psem->count_lock);
        return result;
    }
    psem->count = count;
    return 0;
}

int mac_sem_destroy(mac_sem_t *psem)
{
    mac_sem_t *poldsem;

    if (!psem)
    {
        return EINVAL;
    }
    poldsem = (mac_sem_t *)psem;

    int result = pthread_mutex_trylock(&poldsem->count_lock);
    if (result == 0)
    {
        pthread_mutex_unlock(&poldsem->count_lock);
    }
    else if (result == EBUSY)
    {
        printf("mac_sem_destroy() is called when the lock is still being used\n");
    }
    else
    {
        assert(0 && "Should not reach here");
    }

    pthread_mutex_destroy(&poldsem->count_lock);
    pthread_cond_destroy(&poldsem->count_bump);
    return 0;
}

int mac_sem_post(mac_sem_t *psem)
{
    mac_sem_t *pxsem;
    int        result, xresult;

    if (!psem)
    {
        return EINVAL;
    }
    pxsem = (mac_sem_t *)psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    pxsem->count = pxsem->count + 1;

    xresult = pthread_cond_signal(&pxsem->count_bump);

    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int mac_sem_trywait(mac_sem_t *psem)
{
    mac_sem_t *pxsem;
    int        result, xresult;

    if (!psem)
    {
        return EINVAL;
    }
    pxsem = (mac_sem_t *)psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count > 0)
    {
        pxsem->count--;
    }
    else
    {
        xresult = EAGAIN;
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int mac_sem_wait(mac_sem_t *psem)
{
    mac_sem_t *pxsem;
    int        result, xresult;

    if (!psem)
    {
        return EINVAL;
    }
    pxsem = (mac_sem_t *)psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count == 0)
    {
        xresult = pthread_cond_wait(&pxsem->count_bump, &pxsem->count_lock);
    }
    if (!xresult)
    {
        if (pxsem->count > 0)
        {
            pxsem->count--;
        }
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int mac_sem_timedwait(mac_sem_t *psem, const struct timespec *abstim)
{
    mac_sem_t *pxsem;
    int        result, xresult;

    if (psem == NULL)
    {
        return EINVAL;
    }
    pxsem = (mac_sem_t *)psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count == 0)
    {
        xresult = pthread_cond_timedwait(&pxsem->count_bump, &pxsem->count_lock, abstim);
    }
    if (xresult == 0)
    {
        if (pxsem->count > 0)
        {
            pxsem->count--;
        }
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int mac_sem_getvalue(mac_sem_t *sem, int *sval)
{
    assert(sval);

    int value;

    // TODO: Written very fast to make things compiled. Check this.
    assert(pthread_mutex_lock(&sem->count_lock) == 0);

    value = sem->count;

    assert(pthread_mutex_unlock(&sem->count_lock) == 0);

    *sval = value;

    return 0;
}
