/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for sys/types.h
 */

#ifndef OVERRIDE_SYS_TYPES_H
#define OVERRIDE_SYS_TYPES_H

#include "OCS_sys_types.h"

/* ----------------------------------------- */
/* mappings for declarations in sys/types.h */
/* ----------------------------------------- */
#define ssize_t OCS_ssize_t
#define off_t   OCS_off_t
#define mode_t  OCS_mode_t
#define pid_t   OCS_pid_t
#define gid_t   OCS_gid_t
#define uid_t   OCS_uid_t
#define u_short OCS_u_short

#endif /* OVERRIDE_SYS_TYPES_H */
