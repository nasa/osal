/*
 * File   : bsp_voltab.c
 *
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
#if defined(UT_OS_USE_ATA)
{"/cf",        "/ata0a/cf", FS_BASED,   false,     false,  true,       "CF",    "/cf",    1024     }
#elif defined(UT_OS_USE_RAM)
{"/cf",        "/ram0/cf",  RAM_DISK,   false,     false,  true,       "CF",    "/cf",    1024     }
#else
{"/ramdev13",  "/ram13",    RAM_DISK,   true,      true,   false,      " ",     " ",      0        },
#endif
};

