/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-portable-posix-files.h"

#include "os-impl-files.h"

OCS_uid_t UT_PortablePosixFileTest_GetSelfEUID(void)
{
    return OS_IMPL_SELF_EUID;
}

OCS_gid_t UT_PortablePosixFileTest_GetSelfEGID(void)
{
    return OS_IMPL_SELF_EGID;
}
