/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     osapi-queue-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-queue.h"

/*
** Message Queue API
*/

UT_DEFAULT_STUB(OS_QueueCreate_Impl,(uint32 queue_id, uint32 flags))
UT_DEFAULT_STUB(OS_QueueDelete_Impl,(uint32 queue_id))
UT_DEFAULT_STUB(OS_QueueGet_Impl,(uint32 queue_id, void *data, uint32 size, uint32 *size_copied, int32 timeout))
UT_DEFAULT_STUB(OS_QueuePut_Impl,(uint32 queue_id, const void *data, uint32 size, uint32 flags))
UT_DEFAULT_STUB(OS_QueueGetInfo_Impl,(uint32 queue_id, OS_queue_prop_t *queue_prop))

