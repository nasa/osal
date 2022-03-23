/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
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
 * OSAL coverage stub replacement for arpa/inet.h
 */

#ifndef OCS_ARPA_INET_H
#define OCS_ARPA_INET_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in arpa/inet.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in arpa/inet.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in arpa/inet.h */
/* ----------------------------------------- */

extern const char *OCS_inet_ntop(int af, const void *cp, char *buf, size_t len);
extern int         OCS_inet_pton(int af, const char *cp, void *buf);

#endif /* OCS_ARPA_INET_H */
