/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* OSAL coverage stub replacement for sys/stat.h */
#ifndef _OSAL_STUB_SYS_STAT_H_
#define _OSAL_STUB_SYS_STAT_H_

/* ----------------------------------------- */
/* constants normally defined in sys/stat.h */
/* ----------------------------------------- */

/* note these constants also exist in fcntl.h */
#include "utbits/filemodes.h"


/* ----------------------------------------- */
/* types normally defined in sys/stat.h */
/* ----------------------------------------- */
typedef unsigned int    OCS_mode_t;

struct OCS_stat
{
    unsigned int st_mode;
    unsigned int st_size;
    unsigned int st_mtime;
    unsigned int st_uid;
    unsigned int st_gid;
};

/* ----------------------------------------- */
/* prototypes normally declared in sys/stat.h */
/* ----------------------------------------- */

extern int OCS_fchmod (int fd, OCS_mode_t mode);
extern int OCS_chmod (const char *path, OCS_mode_t mode);
extern int OCS_mkdir (const char * path, OCS_mode_t mode);
extern int OCS_stat (const char * file, struct OCS_stat * buf);



#endif /* _OSAL_STUB_SYS_STAT_H_ */

