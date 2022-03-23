/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-sockets.h"

#include "os-vxworks.h"
#include "os-shared-idmap.h"
#include "os-impl-sockets.h"

/*
 * A UT-specific wrapper function to invoke the VxWorks "SetFlag" helper
 */
void UT_SocketTest_CallVxWorksSetFlags_Impl(uint32 index)
{
    OS_object_token_t token = {.obj_idx = index};

    OS_VxWorks_SetSocketFlags_Impl(&token);
}
