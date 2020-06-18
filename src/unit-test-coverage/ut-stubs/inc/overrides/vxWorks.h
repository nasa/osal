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

/* OSAL coverage stub replacement for vxWorks.h */
#ifndef _OSAL_STUB_VXWORKS_H_
#define _OSAL_STUB_VXWORKS_H_

/* ----------------------------------------- */
/* constants normally defined in vxWorks.h */
/* ----------------------------------------- */
enum
{
    OCS_ERROR = -1,
    OCS_OK    =  0
};

enum
{
    OCS_WAIT_FOREVER = -1,
    OCS_NO_WAIT      = 0
};

/* Fixme: these don't all normally come from vxworks.h, just
 * being held here for now.
 */


#define OCS_NULLDEV             0
#define OCS_FIOUNMOUNT          39      /* unmount disk volume */
#define OCS_FIOCHKDSK           48



/* ----------------------------------------- */
/* types normally defined in vxWorks.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in vxWorks.h */
/* ----------------------------------------- */



#endif /* _OSAL_STUB_VXWORKS_H_ */

