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
 * \file     vxWorks.h
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_VXWORKS_H_
#define INCLUDE_VXWORKS_H_

#include <OCS_vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in vxWorks.h */
/* ----------------------------------------- */
#define ERROR                OCS_ERROR
#define OK                   OCS_OK
#define WAIT_FOREVER         OCS_WAIT_FOREVER
#define NO_WAIT              OCS_NO_WAIT


#define STATUS          OCS_STATUS
#define BOOL            OCS_BOOL
#define FUNCPTR         OCS_FUNCPTR
#define VOIDFUNCPTR     OCS_VOIDFUNCPTR

#define UINT      OCS_UINT
#define INT8      OCS_INT8
#define UINT8     OCS_UINT8
#define INT16     OCS_INT16
#define UINT16    OCS_UINT16
#define INT32     OCS_INT32
#define UINT32    OCS_UINT32

#define _Vx_usr_arg_t   OCS_Vx_usr_arg_t


#endif  /* INCLUDE_VXWORKS_H_ */

