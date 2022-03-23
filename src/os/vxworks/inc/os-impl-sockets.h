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
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_SOCKETS_H
#define OS_IMPL_SOCKETS_H

#include "os-impl-io.h"
#include "os-shared-globaldefs.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <hostLib.h>
#include <ioLib.h>

/*
 * Override the socket flag set routine on this platform.
 * This is required because some versions of VxWorks do not support
 * the standard POSIX fcntl() opcodes, and must use ioctl() instead.
 */
#define OS_IMPL_SET_SOCKET_FLAGS(impl) OS_VxWorks_SetSocketFlags_Impl(impl)

/* The "in.h" header file supplied in VxWorks 6.9 is missing the "in_port_t" typedef */
typedef u_short in_port_t;

/* VxWorks-specific helper function to configure the socket flags on a connection */
void OS_VxWorks_SetSocketFlags_Impl(const OS_object_token_t *token);

#endif /* OS_IMPL_SOCKETS_H */
