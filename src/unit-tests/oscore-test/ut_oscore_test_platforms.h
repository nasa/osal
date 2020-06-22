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

/*================================================================================*
** File:  ut_oscore_test_platforms.h
** Owner: Tam Ngo
** Date:  September 2013
**================================================================================*/

#ifndef _UT_OSCORE_TEST_PLATFORMS_H_
#define _UT_OSCORE_TEST_PLATFORMS_H_

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#ifdef _LINUX_OS_
#include <unistd.h>
#include <semaphore.h>
#include <limits.h>
#include <time.h>
#endif  /* _LINUX_OS_ */

#ifdef _VXWORKS_OS_
#include <semaphore.h>
#endif  /* _VXWORKS_OS_ */

#ifdef OSP_ARINC653
#include "apex_api.h"
#endif  /* OSP_ARINC653 */

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#ifdef OSP_ARINC653
#define SEM_VALUE_MAX  255
#endif  /* OSP_ARINC653 */

/*--------------------------------------------*/
#ifdef _LINUX_OS_
/*--------------------------------------------*/

#define UT_OS_WAIT_MACRO  {                                     \
    struct timespec req = { 0, 2*1000 }; /* 2us delay */        \
    (void) nanosleep(&req, 0);                                  \
  }

/*--------------------------------------------*/
#endif  /* _LINUX_OS_ */
/*--------------------------------------------*/

/*--------------------------------------------*/
#ifdef _VXWORKS_OS_
/*--------------------------------------------*/

#define UT_OS_WAIT_MACRO  taskDelay(20);

/*--------------------------------------------*/
#endif  /* _VXWORKS_OS_ */
/*--------------------------------------------*/

/*--------------------------------------------*/
#ifdef OSP_ARINC653
/*--------------------------------------------*/
#define UT_OS_WAIT_MACRO  for (j=0; j < 10000; j++) { }

/*--------------------------------------------*/
#endif  /* OSP_ARINC653 */
/*--------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------*/
#ifdef _LINUX_OS_
/*--------------------------------------------*/
/*--------------------------------------------*/
#endif  /* _LINUX_OS_ */
/*--------------------------------------------*/

/*--------------------------------------------*/
#ifdef OSP_ARINC653
/*--------------------------------------------*/
/*--------------------------------------------*/
#endif  /* OSP_ARINC653 */
/*--------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

#endif  /* _UT_OSCORE_TEST_PLATFORMS_H_ */

/*================================================================================*
** End of File: ut_oscore_test_platforms.h
**================================================================================*/
