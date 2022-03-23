/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-files.h"

#include "os-vxworks.h"
#include "os-impl-files.h"

int32 UT_Call_OS_VxWorks_StreamAPI_Impl_Init(void)
{
    return OS_VxWorks_StreamAPI_Impl_Init();
}

/*
 * Allow UT to get the value of the OS_IMPL_SELF_EUID and
 * OS_IMPL_SELF_EGID constants.  These might be assigned
 * at runtime by the init function (above) or they might be
 * defined at compile time.
 */
unsigned int UT_FileTest_GetSelfEUID(void)
{
    return OS_IMPL_SELF_EUID;
}

unsigned int UT_FileTest_GetSelfEGID(void)
{
    return OS_IMPL_SELF_EGID;
}

void UT_FileTest_Set_Selectable(osal_index_t local_id, bool is_selectable)
{
    OS_impl_filehandle_table[local_id].selectable = is_selectable;
}
