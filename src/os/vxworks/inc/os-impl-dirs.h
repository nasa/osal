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

/**
 * \file
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_DIRS_H
#define OS_IMPL_DIRS_H

#include "osconfig.h"
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
 * In VxWorks 6.x the system mkdir() function only has a path argument
 * In VxWorks 7 it is now POSIX compilant and adds a mode argument
 *
 * This macro simply discards the second argument, allowing code to use
 * mkdir() in a consistent, POSIX compliant fashion.
 */
#ifdef OSAL_VXWORKS6_COMPATIBILITY
#define mkdir(path, mode) mkdir(path)
#endif

typedef struct
{
    DIR *dp;
} OS_impl_dir_internal_record_t;

/*
 * The directory handle table.
 */
extern OS_impl_dir_internal_record_t OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];

#endif /* OS_IMPL_DIRS_H */
