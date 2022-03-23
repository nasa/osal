/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-dirs.h"

#include "os-vxworks.h"
#include "os-impl-dirs.h"

int32 UT_Call_OS_VxWorks_DirAPI_Impl_Init(void)
{
    return OS_VxWorks_DirAPI_Impl_Init();
}
