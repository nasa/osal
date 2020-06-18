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
 * File: osapi-version.h
 *
 * Purpose: 
 *  The OSAL version numbers 
 *
 * Notes:
 */
#ifndef _osapi_version_h_
#define _osapi_version_h_

/*
 * Note about the OS_REVISION value:
 * During development of the "next" version of OSAL (whatever number it might be), the
 * OS_REVISION value should be something high (>90) to indicate the fact that this is a development
 * version and not an official release version.  For instance, at the time of this writing
 * the official version is 4.1.1 and the next version would likely be 4.2.0.  So anyone
 * who uses the "bleeding edge" development branch before 4.2.0 is officially released will
 * get a version that reads e.g. "4.1.91" to show that this is a pre-4.2 build.
 *
 * This still leaves room for patches to be applied to 4.1.1 to produce 4.1.2 if needed.
 *
 * During development, if an API-affecting change is introduced then the OS_REVISION value should
 * be incremented with it to indicate this.  Client code that depends on the new API can do a "#if"
 * against the development version number e.g. "#if OSAL_API_VERSION >= 40191" and this check
 * will still be valid after the version becomes 40200 (although at some point it can be cleaned
 * up to reflect the official version number, it is not going to break the build in the meantime).
 */

#define OS_MAJOR_VERSION 5
#define OS_MINOR_VERSION 0
#define OS_REVISION      0
#define OS_MISSION_REV   0

/**
 * Combine the revision components into a single value that application code can check against
 * e.g. "#if OSAL_API_VERSION >= 40100" would check if some feature added in OSAL 4.1 is present.
 */
#define OSAL_API_VERSION ((OS_MAJOR_VERSION * 10000) + (OS_MINOR_VERSION * 100) + OS_REVISION)
      
#endif /* _osapi_version_h_ */

/************************/
/*  End of File Comment */
/************************/
