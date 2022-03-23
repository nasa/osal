/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for stat.h
 */

#ifndef OVERRIDE_STAT_H
#define OVERRIDE_STAT_H

#include "OCS_stat.h"

/* ----------------------------------------- */
/* mappings for declarations in sys/stat.h */
/* ----------------------------------------- */

#define stat   OCS_stat
#define fstat  OCS_fstat
#define fchmod OCS_fchmod
#define chmod  OCS_chmod
#define mkdir  OCS_mkdir

/* ----------------------------------------- */
/* mappings for declarations in sys/statvfs.h */
/* ----------------------------------------- */
#define statvfs OCS_statvfs
#define statfs  OCS_statvfs

#endif /* OVERRIDE_STAT_H */
