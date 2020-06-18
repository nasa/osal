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
** File:  ut_psp_voltab_stubs.c
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

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*
** OSAL volume table. This is the only file in the PSP that still has the
** OS_ naming convention, since it belongs to the OSAL.
*/
OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES] =
{
/* DevName     PhysDev      VolType     Volatile?  Free?   IsMounted?  VolName  MountPnt  BlockSz */
{"/ramdev0",   "/ram0",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev1",   "/ram1",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev2",   "/ram2",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev3",   "/ram3",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev4",   "/ram4",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev5",   "/ram5",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev6",   "/ram6",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev7",   "/ram7",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev8",   "/ram8",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev9",   "/ram9",     RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev10",  "/ram10",    RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev11",  "/ram11",    RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
{"/ramdev12",  "/ram12",    RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
#if defined(UT_OS_USE_RAM)
{"/cf",        "/ram0/cf",  FS_BASED,   false,     false,  true,       "CF",    "/cf",    1024     }
#elif defined(UT_OS_USE_ATA)
{"/cf",        "/ata0a/cf", FS_BASED,   false,     false,  true,       "CF",    "/cf",    1024     }
#else
{"/ramdev13",  "/ram13",    RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
#endif
};

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function definitions
**--------------------------------------------------------------------------------*/

/*================================================================================*
** End of File: ut_psp_voltab_stubs.c
**================================================================================*/
