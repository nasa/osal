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
 * Declarations and prototypes for select
 */

#ifndef OVERRIDE_SYS_SELECT_H
#define OVERRIDE_SYS_SELECT_H

#include "OCS_sys_select.h"

/* ----------------------------------------- */
/* constants normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* mappings for declarations in sys/select.h */
/* ----------------------------------------- */

#define fd_set OCS_fd_set
#define select OCS_select

#define FD_SET   OCS_FD_SET
#define FD_ISSET OCS_FD_ISSET
#define FD_CLR   OCS_FD_CLR
#define FD_ZERO  OCS_FD_ZERO

#define FD_SETSIZE OCS_FD_SETSIZE

#endif /* OVERRIDE_SYS_SELECT_H */
