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
{"/ramdev0", "./ram0",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev1", "./ram1",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev2", "./ram2",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev3", "./ram3",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev4", "./ram4",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"/ramdev5", "./ram5",      FS_BASED,        true,      true,     false,     " ",      " ",     0        },

/*
** The following entry is a "pre-mounted" path to a non-volatile device
*/
{"/eedev0",  "./eeprom1",      FS_BASED,        false,     false,    true,     "CF",      "/cf",     512   },

{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        },
{"unused",   "unused",    FS_BASED,        true,      true,     false,     " ",      " ",     0        }
};



