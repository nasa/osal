#ifndef _POSIX_MACOS_TIME_H_
#define _POSIX_MACOS_TIME_H_

#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef TIMER_ABSTIME
/// We are not using this variable, so the value does not matter.
#define TIMER_ABSTIME 12345
#endif

    void __timespec_diff(const struct timespec *lhs, const struct timespec *rhs, struct timespec *out);

    int clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *request, struct timespec *remain);

#ifdef __cplusplus
}
#endif

#endif /* _POSIX_MACOS_TIME_H_ */
