/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* OSAL coverage stub replacement for functions in sys/stat.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "OCS_sys_select.h"

int OCS_select(int nfds, OCS_fd_set *readfds, OCS_fd_set *writefds, OCS_fd_set *exceptfds, struct OCS_timeval *timeout)
{
    return UT_DEFAULT_IMPL_RC(OCS_select, 1);
}

void OCS_FD_SET(int fd, OCS_fd_set *set)
{
    UT_DEFAULT_IMPL(OCS_FD_SET);
}

int OCS_FD_ISSET(int fd, OCS_fd_set *set)
{
    return UT_DEFAULT_IMPL(OCS_FD_ISSET);
}

void OCS_FD_CLR(int fd, OCS_fd_set *set)
{
    UT_DEFAULT_IMPL(OCS_FD_CLR);
}

void OCS_FD_ZERO(OCS_fd_set *set)
{
    UT_DEFAULT_IMPL(OCS_FD_ZERO);
}
