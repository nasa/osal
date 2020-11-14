#include "mqueue-internal.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

const size_t MQ_FS_NAME_MAX = 64;

static const char prefix[] = "/tmp";

static const size_t MQ_NAME_MAX = 32;

static size_t safe_strlen(const char *str, size_t max_len)
{
    const char *end = (const char *)memchr(str, '\0', max_len);
    if (end == NULL)
    {
        return max_len;
    }
    else
    {
        return end - str;
    }
}

int mq_get_fs_pathname(const char *const pathname, char *const out_pathname)
{
    if (pathname[0] != '/')
    {
        return EINVAL;
    }

    size_t pathname_len = safe_strlen(pathname, MQ_NAME_MAX);
    assert(pathname_len < MQ_NAME_MAX);

    size_t internal_len = strlen(prefix) + pathname_len + 1; // +1 for ending '\0'
    assert(internal_len < MQ_FS_NAME_MAX);

    snprintf(out_pathname, internal_len, "%s%s", prefix, pathname);
    return 0;
}
