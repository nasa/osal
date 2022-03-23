/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for dirent.h
 */

#ifndef OCS_DIRENT_H
#define OCS_DIRENT_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in dirent.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in dirent.h */
/* ----------------------------------------- */

typedef struct OCS_DIR OCS_DIR;

struct OCS_dirent
{
    char d_name[4];
};

/* ----------------------------------------- */
/* prototypes normally declared in dirent.h */
/* ----------------------------------------- */

extern int                OCS_closedir(OCS_DIR *dirp);
extern OCS_DIR *          OCS_opendir(const char *name);
extern struct OCS_dirent *OCS_readdir(OCS_DIR *dirp);
extern void               OCS_rewinddir(OCS_DIR *dirp);

#endif /* OCS_DIRENT_H */
