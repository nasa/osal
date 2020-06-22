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
 * \file   os-impl-no-network.c
 * \author joseph.p.hickey@nasa.gov
 *
 *
 * Purpose: This file contains the network functionality for for
 *      systems which do not implement any networking (OS_INCLUDE_NETWORK is false).
 *
 * It implements the required calls and returns OS_ERR_NOT_IMPLEMENTED for all of them.
 *
 * NOTE: This is a "template" file and not a directly usable source file.
 *       It must be adapted/instantiated from within the OS-specific
 *       implementation on platforms that wish to use this template.
 */


/****************************************************************************************
                                    Network API
 ***************************************************************************************/


                        
/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetID_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetID_Impl             (int32 *IdBuf)
{
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_NetworkGetID_Impl */



                        
/*----------------------------------------------------------------
 *
 * Function: OS_NetworkGetHostName_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_NetworkGetHostName_Impl       (char *host_name, uint32 name_len)
{
    return OS_ERR_NOT_IMPLEMENTED;
} /* end OS_NetworkGetHostName_Impl */


