/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/*
 * \file
 *
 *   OSAL BSP set up file system
 */

/*
**  Include Files
*/
/* TODO clean these */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <rtems.h>
#include <rtems/bdbuf.h>
#include <rtems/blkdev.h>
#include <rtems/diskdevs.h>
#include <rtems/bdpart.h>
#include <rtems/error.h>
#include <rtems/ramdisk.h>
#include <rtems/dosfs.h>
#include <rtems/fsmount.h>

#if defined(OS_RTEMS_4_DEPRECATED) || defined(OS_RTEMS_5)
#include <rtems/mkrootfs.h>
#endif

/* TODO break up and if there's config move to inc dir supporting overrides */
#include "pcrtems_bsp_internal.h"

/*
**  External Declarations
*/
extern rtems_status_code rtems_ide_part_table_initialize(const char *);

void OS_BSP_SetupFS(void)
{
    int         status;
    struct stat statbuf;

#if defined(OS_RTEMS_4_DEPRECATED) || defined(OS_RTEMS_5)
    /*
    ** Create the RTEMS Root file system
    */
    status = rtems_create_root_fs();
    if (status != RTEMS_SUCCESSFUL)
    {
        printf("Creating Root file system failed: %s\n", rtems_status_text(status));
    }
#endif

    /*
     * Create the mountpoint for the general purpose file system
     */
    if (stat(RTEMS_USER_FS_MOUNTPOINT, &statbuf) < 0)
    {
        status = mkdir(RTEMS_USER_FS_MOUNTPOINT,
                       S_IFDIR | S_IRWXU | S_IRWXG | S_IRWXO); /* For nonvol filesystem mountpoint */
        if (status < 0)
        {
            printf("mkdir failed: %s\n", strerror(errno));
        }
    }

    /*
     * Register the IDE partition table.
     */
    status = rtems_ide_part_table_initialize("/dev/hda");
    if (status != RTEMS_SUCCESSFUL)
    {
        /* note this is not necessarily an error, it just means there
         * will be no persistent storage in this instance.  The IMFS
         * is still available. */
        BSP_DEBUG("warning: /dev/hda partition table not found: %s / %s\n", rtems_status_text(status), strerror(errno));
        BSP_DEBUG("Persistent storage will NOT be mounted\n");
    }
    else
    {
        status = mount("/dev/hda1", RTEMS_USER_FS_MOUNTPOINT, RTEMS_FILESYSTEM_TYPE_DOSFS, RTEMS_FILESYSTEM_READ_WRITE,
                       NULL);
        if (status < 0)
        {
            BSP_DEBUG("mount failed: %s\n", strerror(errno));
        }
    }

    /*
     * Change to the user storage mountpoint dir, which
     * will be the basis of relative directory refs.
     * If mounted, it will be persistent, otherwise
     * it will be an IMFS dir, but should generally work.
     */
    chdir(RTEMS_USER_FS_MOUNTPOINT);
}
