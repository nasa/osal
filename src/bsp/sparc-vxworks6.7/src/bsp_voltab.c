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

/*
 * File   : bsp_voltab.c
 * Author : Nicholas Yanchik / GSFC Code 582
 *
 * BSP Volume table for file systems
 */

/****************************************************************************************
                                    INCLUDE FILES
****************************************************************************************/
#include "common_types.h"
#include "osapi.h"


/* 
**  volume table. 
*/
OS_VolumeInfo_t OS_VolumeTable [NUM_TABLE_ENTRIES] = 
{
/* Dev Name  Phys Dev  Vol Type        Volatile?  Free?     IsMounted? Volname  MountPt BlockSz */

/*  RAM Disk */
{"/ramdev0", " ",      RAM_DISK,        true,      true,     false,     " ",      " ",     0        },

/* non-volatile Disk -- Auto-Mapped to an existing CF disk */
{"/eedev0",  "CF:0",      FS_BASED,        false,     false,     true,     "CF",      "/cf",     512        },

/* 
** Spare RAM disks to be used for SSR and other RAM disks 
*/
{"/ramdev1", " ",      RAM_DISK,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev2", " ",      RAM_DISK,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev3", " ",      RAM_DISK,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev4", " ",      RAM_DISK,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev5", " ",      RAM_DISK,        true,      true,     false,     " ",      " ",     0        },

/* 
** Hard disk mappings 
*/
{"/ssrdev0",  "/hd:0/SSR1", FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ssrdev1",  "/hd:0/SSR2", FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ssrdev2",  "/hd:0/SSR3", FS_BASED,        true,      true,     false,     " ",      " ",     0        },

{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        }

};



