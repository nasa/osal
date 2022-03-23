/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/* pull in the OSAL configuration */
#include "osconfig.h"
#include <stdlib.h>
#include <string.h>

#include "os-vxworks.h"
#include "os-impl-idmap.h"
#include "ut-adaptor-idmap.h"

int32 UT_Call_OS_VxWorks_TableMutex_Init(osal_objtype_t idtype)
{
    return OS_VxWorks_TableMutex_Init(idtype);
}

void UT_IdMapTest_SetImplTableMutex(osal_objtype_t idtype, OCS_SEM_ID vxid)
{
    OS_impl_objtype_lock_table[idtype]->vxid = vxid;
}
