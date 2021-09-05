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
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for errno.h
 */

#ifndef OVERRIDE_ERRNO_H
#define OVERRIDE_ERRNO_H

#include "OCS_errno.h"

/* ----------------------------------------- */
/* mappings for declarations in errno.h */
/* ----------------------------------------- */
#define EINTR     OCS_EINTR
#define EAGAIN    OCS_EAGAIN
#define EINVAL    OCS_EINVAL
#define EEXIST    OCS_EEXIST
#define EMSGSIZE  OCS_EMSGSIZE
#define ETIMEDOUT OCS_ETIMEDOUT
#define ESPIPE    OCS_ESPIPE
#define ENOTSUP   OCS_ENOTSUP
#define ENOSYS    OCS_ENOSYS
#define EROFS     OCS_EROFS

#define errno OCS_errno

#endif /* OVERRIDE_ERRNO_H */
