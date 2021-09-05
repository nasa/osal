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
 * \ingroup  posix
 *
 */

#ifndef OS_IMPL_SOCKETS_H
#define OS_IMPL_SOCKETS_H

#include "os-impl-io.h"

#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define OS_NETWORK_SUPPORTS_IPV6

/*
 * Socket descriptors should be usable with the select() API
 */
#define OS_IMPL_SOCKET_SELECTABLE true

/*
 * A full POSIX-compliant I/O layer should support using
 * nonblocking I/O calls in combination with select().
 */
#define OS_IMPL_SOCKET_FLAGS O_NONBLOCK

#endif /* OS_IMPL_SOCKETS_H */
