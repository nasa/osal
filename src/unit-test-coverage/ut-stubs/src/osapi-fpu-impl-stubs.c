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
 * \file     osapi-fpu-impl-stubs.c
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "utstubs.h"

#include "os-shared-fpu.h"

/*
 * FPU API low-level handlers
 */
UT_DEFAULT_STUB(OS_FPUExcAttachHandler_Impl,(uint32 ExceptionNumber, osal_task_entry ExceptionHandler,int32 parameter))
UT_DEFAULT_STUB(OS_FPUExcEnable_Impl,(int32 ExceptionNumber))
UT_DEFAULT_STUB(OS_FPUExcDisable_Impl,(int32 ExceptionNumber))
UT_DEFAULT_STUB(OS_FPUExcSetMask_Impl,(uint32 mask))
UT_DEFAULT_STUB(OS_FPUExcGetMask_Impl,(uint32 *mask))

