/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for dirent.h
 */

#ifndef OVERRIDE_DIRENT_H
#define OVERRIDE_DIRENT_H

#include "OCS_dirent.h"

/* ----------------------------------------- */
/* mappings for declarations in dirent.h */
/* ----------------------------------------- */

#define DIR       OCS_DIR
#define dirent    OCS_dirent
#define closedir  OCS_closedir
#define opendir   OCS_opendir
#define readdir   OCS_readdir
#define rewinddir OCS_rewinddir

#endif /* OVERRIDE_DIRENT_H */
