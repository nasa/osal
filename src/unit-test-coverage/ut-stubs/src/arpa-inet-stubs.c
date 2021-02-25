/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \brief    Stubs for arpa/inet.h
 * \ingroup  ut-stubs
 */
#include <arpa/inet.h>
#include "utstubs.h"
#include "OCS_arpa_inet.h"

const char *OCS_inet_ntop(int af, const void *cp, char *buf, size_t len)
{
    int32 Status;

    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_ntop), af);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_ntop), cp);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_ntop), buf);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_ntop), len);

    Status = UT_DEFAULT_IMPL(OCS_inet_ntop);

    if (Status == 0)
    {
        /* "nominal" response */
        return buf;
    }

    return (char *)0;
}

int OCS_inet_pton(int af, const char *cp, void *buf)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_pton), af);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_pton), cp);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_pton), buf);

    return UT_DEFAULT_IMPL(OCS_inet_pton);
}
