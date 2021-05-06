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
 * OSAL coverage stub replacement for sched.h
 */

#ifndef OVERRIDE_SCHED_H
#define OVERRIDE_SCHED_H

#include "OCS_sched.h"

/* ----------------------------------------- */
/* mappings for declarations in sched.h */
/* ----------------------------------------- */

#define SCHED_FIFO OCS_SCHED_FIFO
#define SCHED_RR   OCS_SCHED_RR

#define sched_param OCS_sched_param

#define sched_get_priority_max OCS_sched_get_priority_max
#define sched_get_priority_min OCS_sched_get_priority_min

#endif /* OVERRIDE_SCHED_H */
