/************************************************************************
 * NASA Docket No. GSC-19,200-1, and identified as "cFS Draco"
 *
 * Copyright (c) 2023 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/* OSAL coverage stub replacement for netdb.h */
#include <string.h>
#include "utstubs.h"

#include "OCS_netdb.h"

/* ----------------------------------------- */
/* prototypes normally declared in netdb.h */
/* ----------------------------------------- */
int OCS_getnameinfo(const struct OCS_sockaddr *addr, OCS_socklen_t addrlen, char *host, OCS_socklen_t hostlen,
                    char *serv, OCS_socklen_t servlen, int flags)
{
    int32 Status;

    Status = UT_DEFAULT_IMPL(OCS_getnameinfo);

    if (hostlen > 0)
    {
        memset(host, 0, hostlen);
        UT_Stub_CopyToLocal(UT_KEY(OCS_getaddrinfo), host, hostlen - 1);
    }

    if (servlen > 0)
    {
        memset(serv, 0, servlen);
    }

    return Status;
}

int OCS_getaddrinfo(const char *node, const char *service, const struct OCS_addrinfo *hints, struct OCS_addrinfo **res)
{
    static struct OCS_addrinfo DEFAULT_RESULT;
    int32                      Status;

    Status = UT_DEFAULT_IMPL(OCS_getaddrinfo);

    if (Status == 0)
    {
        /* this needs to output a value */
        if (UT_Stub_CopyToLocal(UT_KEY(OCS_getaddrinfo), res, sizeof(*res)) < sizeof(*res))
        {
            memset(&DEFAULT_RESULT, 0, sizeof(DEFAULT_RESULT));
            *res = &DEFAULT_RESULT;
        }
    }
    else
    {
        *res = NULL;
    }

    return Status;
}

void OCS_freeaddrinfo(struct OCS_addrinfo *res)
{
    UT_DEFAULT_IMPL(OCS_freeaddrinfo);
}
