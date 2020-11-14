#ifndef _POSIX_MACOS_STUBS_H_
#define _POSIX_MACOS_STUBS_H_

#include <sys/param.h>

/// Warning: These are just stubs. They are not intended to be used.
#ifndef SIGRTMIN
#define SIGRTMIN (SIGUSR2 + 1)
#define SIGRTMAX 60
#endif

#endif /* _POSIX_MACOS_STUBS_H_ */
