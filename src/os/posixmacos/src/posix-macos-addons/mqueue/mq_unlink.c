#include "mqueue.h"

#include "mqueue-internal.h"

#include <errno.h>

int mq_unlink(const char *pathname)
{
    char fs_pathname[MQ_FS_NAME_MAX];
    if (mq_get_fs_pathname(pathname, fs_pathname) == EINVAL)
    {
        errno = EINVAL;
        return -1;
    };
    if (unlink(fs_pathname) == -1)
    {
        return -1;
    }
    return 0;
}
