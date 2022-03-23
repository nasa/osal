/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-sockets header
 */

#include "os-shared-sockets.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_CreateSocketName()
 * ----------------------------------------------------
 */
void OS_CreateSocketName(const OS_object_token_t *token, const OS_SockAddr_t *Addr, const char *parent_name)
{
    UT_GenStub_AddParam(OS_CreateSocketName, const OS_object_token_t *, token);
    UT_GenStub_AddParam(OS_CreateSocketName, const OS_SockAddr_t *, Addr);
    UT_GenStub_AddParam(OS_CreateSocketName, const char *, parent_name);

    UT_GenStub_Execute(OS_CreateSocketName, Basic, NULL);
}
