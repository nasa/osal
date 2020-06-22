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
 *   OSAL BSP main entry point.
 *
 * History:
 *   2005/07/26  A. Cudmore      | Initial version for linux 
 */

/*
** OSAL includes 
*/
#include "osapi.h"

/*
** Types and prototypes for this module
*/

/*
**  External Declarations
*/
void OS_Application_Startup(void);
                                                                           
/*
** Global variables
*/


                                                                                                                                                
/******************************************************************************
**  Function:  main()
**
**  Purpose:
**    BSP Application entry point.
**
**  Arguments:
**    (none)
**
**  Return:
**    (none)
*/

int main(int argc, char *argv[])
{
   /*
   ** OS_API_Init is called by OS_Application_Startup
   ** Also note that OS_API_Init now also takes care of signal masking
   */

   /*
   ** Call application specific entry point.
   */
   OS_Application_Startup();

   /*
   ** OS_IdleLoop() will wait forever and return if
   ** someone calls OS_ApplicationShutdown(true)
   */
   OS_IdleLoop();

   /* Should typically never get here */
   return(EXIT_SUCCESS);
}

