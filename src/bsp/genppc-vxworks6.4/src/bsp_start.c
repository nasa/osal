/*
 * File:  bsp_start.c
 *
 * Purpose:
 *  OSAL main entry point.
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

