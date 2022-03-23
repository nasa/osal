/**
 * \file
 * \ingroup ut-stubs
 *
 * Declarations and prototypes for OCS_sys_select
 */

#ifndef OCS_SYS_SELECT_H
#define OCS_SYS_SELECT_H

#include "OCS_basetypes.h"
#include "OCS_time.h"

/* ----------------------------------------- */
/* constants normally defined in sys/select.h */
/* ----------------------------------------- */

#define OCS_FD_SETSIZE 10

/* ----------------------------------------- */
/* types normally defined in sys/select.h */
/* ----------------------------------------- */
typedef struct
{
    unsigned int fds;
} OCS_fd_set;

/* ----------------------------------------- */
/* prototypes normally declared in sys/select.h */
/* ----------------------------------------- */

extern int OCS_select(int nfds, OCS_fd_set *readfds, OCS_fd_set *writefds, OCS_fd_set *exceptfds,
                      struct OCS_timeval *timeout);

extern void OCS_FD_SET(int fd, OCS_fd_set *set);
extern int  OCS_FD_ISSET(int fd, OCS_fd_set *set);
extern void OCS_FD_CLR(int fd, OCS_fd_set *set);
extern void OCS_FD_ZERO(OCS_fd_set *set);

#endif /* OCS_SYS_SELECT_H */
