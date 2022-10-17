/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file   os-impl-no-select.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose: All functions return OS_ERR_NOT_IMPLEMENTED.
 * This is used when network functionality is disabled by config.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <osapi.h>
#include "os-shared-select.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/***************************************************************************************
                                 FUNCTION PROTOTYPES
 **************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/****************************************************************************************
                                LOCAL FUNCTIONS
 ***************************************************************************************/

/****************************************************************************************
                                SELECT API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectSingle_Impl(const OS_object_token_t *token, uint32 *SelectFlags, int32 msecs)
{
    return OS_ERR_NOT_IMPLEMENTED;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SelectMultiple_Impl(OS_FdSet *ReadSet, OS_FdSet *WriteSet, int32 msecs)
{
    return OS_ERR_NOT_IMPLEMENTED;
}
