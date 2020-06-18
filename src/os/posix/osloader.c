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
 * \file   osloader.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: This file contains the module loader and symbol lookup functions for the OSAL.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "os-impl.h"

/*
 * The posix OSAL (by definition) always uses the posix dlfcn.h loading functions
 * Other non-posix OS's (i.e. classic RTEMS) share posix-style dl functions which is
 * why it is put into a separate code blob.
 */
#include "../portable/os-impl-posix-dl.c"

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ModuleGetInfo_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ModuleGetInfo_Impl ( uint32 module_id, OS_module_prop_t *module_prop )
{
   /*
    * Note that the "dlinfo()" function might return some interesting
    * information for this API, but this is actually a non-posix GNU extension.
    * However this is not done for now, limiting to only posix APIs here
    */
    return(OS_SUCCESS);

} /* end OS_ModuleGetInfo_Impl */


                        
/*----------------------------------------------------------------
 *
 * Function: OS_SymbolTableDump_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SymbolTableDump_Impl ( const char *filename, uint32 SizeLimit )
{

   return(OS_ERR_NOT_IMPLEMENTED);

} /* end OS_SymbolTableDump_Impl */

