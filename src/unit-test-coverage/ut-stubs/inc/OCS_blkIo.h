/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub basic data types
 */

#ifndef OCS_BLKIO_H
#define OCS_BLKIO_H

#include "OCS_basetypes.h"
#include "OCS_vxWorks.h"

/* The module and blk_dev types are used in several headers */
typedef struct OCS_BLK_DEV
{
    int bd;
} OCS_BLK_DEV;
typedef OCS_BLK_DEV *OCS_BLK_DEV_ID;

#endif /* OCS_BLKIO_H */
