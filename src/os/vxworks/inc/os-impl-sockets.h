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
 *
 * \ingroup  vxworks
 *
 */

#ifndef OS_IMPL_SOCKETS_H
#define OS_IMPL_SOCKETS_H

#include "os-impl-io.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <hostLib.h>

/*
 * Socket descriptors should be usable with the select() API
 */
#define OS_IMPL_SOCKET_SELECTABLE true

/*
 * Use the O_NONBLOCK flag on sockets
 *
 * NOTE: the fcntl() F_GETFL/F_SETFL opcodes that set descriptor flags may not
 * work correctly on some version of VxWorks.
 *
 * This flag is not strictly required, things still mostly work without it,
 * but lack of this mode does introduce some potential race conditions if more
 * than one task attempts to use the same descriptor handle at the same time.
 */
#define OS_IMPL_SOCKET_FLAGS O_NONBLOCK

/* The "in.h" header file supplied in VxWorks 6.9 is missing the "in_port_t" typedef */
typedef u_short in_port_t;

#endif /* OS_IMPL_SOCKETS_H */
