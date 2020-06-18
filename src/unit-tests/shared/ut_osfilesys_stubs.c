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

/*================================================================================*
** File:  ut_osfilesys_stubs.c
** Owner: Tam Ngo
** Date:  March 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_os_stubs.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/* As defined in osfilesys.c */
extern OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES];

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

UT_OsReturnCode_t  g_fsMake = {0,0};
UT_OsReturnCode_t  g_fsRemove = {0,0};
UT_OsReturnCode_t  g_fsInit = {0,0};
UT_OsReturnCode_t  g_fsMount = {0,0};
UT_OsReturnCode_t  g_fsUnmount = {0,0};
UT_OsReturnCode_t  g_fsBlocksFree = {0,0};
UT_OsReturnCode_t  g_fsBytesFree = {0,0};
UT_OsReturnCode_t  g_fsGetPhysDriveName = {0,0};
UT_OsReturnCode_t  g_fsTranslatePath = {0,0};

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/

int32 OS_mkfs(char* address, char* devname,char* volname, uint32 blocksize, uint32 numblocks)
{
    return (g_fsMake.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_rmfs(char* devname)
{
    return (g_fsRemove.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_initfs(char* address,char* devname, char* volname, uint32 blocksize, uint32 numblocks)
{
    return (g_fsInit.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_mount(const char* devname, char* mountpoint)
{
    return (g_fsMount.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_unmount(const char* mountpoint)
{
    return (g_fsUnmount.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_fsBlocksFree(const char* name)
{
    return (g_fsBlocksFree.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_fsBytesFree(const char* name, uint64* bytes_free)
{
    return (g_fsBytesFree.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_FS_GetPhysDriveName(char* PhysDriveName, char* MountPoint)
{
    return (g_fsGetPhysDriveName.value);
}

/*--------------------------------------------------------------------------------*/

int32 OS_TranslatePath(const char* VirtualPath, char* LocalPath)
{
    return (g_fsTranslatePath.value);
}

/*================================================================================*
** End of File: ut_osfilesys_stubs.c
**================================================================================*/
