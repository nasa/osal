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

/*
 * File:  bsp_start.c
 *
 * Purpose:
 *   
 *  OSAL main entry point.
 *
 * History:
 */

/*
**  Include Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "vxWorks.h"
#include "sysLib.h"
#include "taskLib.h"

/*
** OSAL includes 
*/
#include "common_types.h"
#include "osapi.h"

/*
**  External Declarations
*/
void OS_Application_Startup(void);

/******************************************************************************
**  Function:  OS_BSPMain()
**
**  Purpose:
**    vxWorks/BSP Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

void OS_BSPMain( void )
{
   int    TicksPerSecond;
   
   /* 
   ** Delay for one second. 
   */
   TicksPerSecond = sysClkRateGet();
   (void) taskDelay( TicksPerSecond );

   OS_printf("Starting Up OSAPI App.\n");
   
   /*
   ** Call OSAL entry point.    
   */
   OS_Application_Startup();

   /*
   ** Exit the main thread.
   ** in VxWorks we can delete all of the OS tasks if we want.
   */

}

