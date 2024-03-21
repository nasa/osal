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
 *   OSAL BSP set up file system with tarfs
 */
#include <stdio.h>
#include <rtems/untar.h>
#include "bsp_setupfs.h"

/* Tar file symbols */
extern int _binary_tarfile_start;
extern int _binary_tarfile_size;

void OS_BSP_SetupFS(void)
{
    int status;

    /* Initialize the file system using tarfs */
    printf("Populating Root file system from TAR file.\n");
    status = Untar_FromMemory((unsigned char *)(&_binary_tarfile_start), (unsigned long)&_binary_tarfile_size);
    if (status != UNTAR_SUCCESSFUL)
    {
        printf("Error while untaring from memory\n");
    }
}
