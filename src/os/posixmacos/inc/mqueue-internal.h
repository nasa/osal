#ifndef _MQUEUE_INTERNAL_H_
#define _MQUEUE_INTERNAL_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern const size_t MQ_FS_NAME_MAX;
    int                 mq_get_fs_pathname(const char *const pathname, char *const out_pathname);

#ifdef __cplusplus
}
#endif

#endif /* _MQUEUE_INTERNAL_H_ */
