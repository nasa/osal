/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for sched.h
 */

#ifndef OCS_SCHED_H
#define OCS_SCHED_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in sched.h */
/* ----------------------------------------- */
#define OCS_SCHED_FIFO 0x1200
#define OCS_SCHED_RR   0x1201

/* ----------------------------------------- */
/* types normally defined in sched.h */
/* ----------------------------------------- */
struct OCS_sched_param
{
    int sched_priority;
};

/* ----------------------------------------- */
/* prototypes normally declared in sched.h */
/* ----------------------------------------- */

extern int OCS_sched_get_priority_max(int policy);
extern int OCS_sched_get_priority_min(int policy);

#endif /* OCS_SCHED_H */
