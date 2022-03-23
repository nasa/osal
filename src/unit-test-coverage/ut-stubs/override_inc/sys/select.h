/**
 * \file
 * \ingroup ut-stubs
 *
 * Declarations and prototypes for select
 */

#ifndef OVERRIDE_SYS_SELECT_H
#define OVERRIDE_SYS_SELECT_H

#include "OCS_sys_select.h"

/* ----------------------------------------- */
/* constants normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* mappings for declarations in sys/select.h */
/* ----------------------------------------- */

#define fd_set OCS_fd_set
#define select OCS_select

#define FD_SET   OCS_FD_SET
#define FD_ISSET OCS_FD_ISSET
#define FD_CLR   OCS_FD_CLR
#define FD_ZERO  OCS_FD_ZERO

#define FD_SETSIZE OCS_FD_SETSIZE

#endif /* OVERRIDE_SYS_SELECT_H */
