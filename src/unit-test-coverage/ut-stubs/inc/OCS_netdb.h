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

/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for netdb.h
 */

#ifndef OCS_NETDB_H
#define OCS_NETDB_H

#include "OCS_basetypes.h"
#include "OCS_sys_socket.h"

/* ----------------------------------------- */
/* constants normally defined in netdb.h */
/* ----------------------------------------- */

#define OCS_EAI_AGAIN  0x1a1a
#define OCS_EAI_NONAME 0x1a1b

/* ----------------------------------------- */
/* types normally defined in netdb.h */
/* ----------------------------------------- */

struct OCS_addrinfo
{
    int                  ai_flags;
    int                  ai_family;
    int                  ai_socktype;
    int                  ai_protocol;
    OCS_socklen_t        ai_addrlen;
    struct OCS_sockaddr *ai_addr;
    char *               ai_canonname;
    struct OCS_addrinfo *ai_next;
};

/* ----------------------------------------- */
/* prototypes normally declared in netdb.h */
/* ----------------------------------------- */
int OCS_getnameinfo(const struct OCS_sockaddr *addr, OCS_socklen_t addrlen, char *host, OCS_socklen_t hostlen,
                    char *serv, OCS_socklen_t servlen, int flags);

int OCS_getaddrinfo(const char *node, const char *service, const struct OCS_addrinfo *hints, struct OCS_addrinfo **res);

void OCS_freeaddrinfo(struct OCS_addrinfo *res);

#endif /* OCS_NETDB_H */
