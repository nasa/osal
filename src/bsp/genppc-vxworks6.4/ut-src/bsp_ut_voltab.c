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


{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        }
};

/*
 * An example of a static symbol loader table
 * Only used if OS_STATIC_LOADER is enabled in osconfig.h
 */
OS_static_symbol_record_t OS_STATIC_SYMBOL_TABLE[] =
{
        { "OS_Application_Startup", OS_Application_Startup },
        { NULL, NULL }
};


