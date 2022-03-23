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
 * OSAL coverage stub replacement for sys/types.h
 */

#ifndef OCS_SYS_TYPES_H
#define OCS_SYS_TYPES_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in sys/types.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/types.h */
/* ----------------------------------------- */
typedef ptrdiff_t          OCS_ssize_t;
typedef long               OCS_off_t;
typedef unsigned int       OCS_mode_t;
typedef long               OCS_time_t;
typedef int                OCS_pid_t;
typedef int                OCS_gid_t;
typedef int                OCS_uid_t;
typedef unsigned short int OCS_u_short;

/* ----------------------------------------- */
/* prototypes normally declared in sys/types.h */
/* ----------------------------------------- */

#endif /* OCS_SYS_TYPES_H */
