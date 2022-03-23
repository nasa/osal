/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "os-shared-common.h"
#include "OCS_semLib.h"

UT_DEFAULT_STUB(OS_API_Impl_Init, (osal_objtype_t idtype))

int OS_VxWorks_GenericSemTake(OCS_SEM_ID vxid, int sys_ticks)
{
    return UT_DEFAULT_IMPL(OS_VxWorks_GenericSemTake);
}

int OS_VxWorks_GenericSemGive(OCS_SEM_ID vxid)
{
    return UT_DEFAULT_IMPL(OS_VxWorks_GenericSemGive);
}
