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
 * \file
 *
 * Provide version identifiers for cFS' Operating System Abstraction Layer
 *  See @ref cfsversions for version and build number and description
 *
 */

#ifndef OSAPI_VERSION_H
#define OSAPI_VERSION_H

#include "common_types.h"

/*
 * Development Build Macro Definitions
 */
#define OS_BUILD_NUMBER   387
#define OS_BUILD_BASELINE "v5.1.0-rc1"

/*
 * Version Macro Definitions
 */
#define OS_MAJOR_VERSION 5 /*!< @brief ONLY APPLY for OFFICIAL releases. Major version number. */
#define OS_MINOR_VERSION 0 /*!< @brief ONLY APPLY for OFFICIAL releases. Minor version number. */
#define OS_REVISION      0 /*!< @brief ONLY APPLY for OFFICIAL releases. Revision number.      */

/*!
 * @brief Mission revision.
 *
 * Set to 0 on OFFIFICIAL releases, and set to 255 (0xFF) on development versions.
 * Values 1-254 are reserved for mission use to denote patches/customizations as needed.
 */
#define OS_MISSION_REV 0xFF

/*
 * Tools to construct version string
 */
#define OS_STR_HELPER(x) #x               /*!< @brief Helper function to concatenate strings from integer */
#define OS_STR(x)        OS_STR_HELPER(x) /*!< @brief Helper function to concatenate strings from integer */

/*! @brief Development Build Version Number.
 * @details Baseline git tag + Number of commits since baseline. @n
 * See @ref cfsversions for format differences between development and release versions.
 */
#define OS_VERSION OS_BUILD_BASELINE "+dev" OS_STR(OS_BUILD_NUMBER)

/*! @brief Version code name
 * All modular components which are tested/validated together should share the same code name
 */
#define OS_VERSION_CODENAME "Bootes"

/*! @brief Development Build Version String.
 * @details Reports the current development build's baseline, number, and name. Also includes a note about the latest
 * official version. @n See @ref cfsversions for format differences between development and release versions.
 */
#define OS_VERSION_STRING                                                                          \
    " OSAL Development Build\n"                                                                    \
    " " OS_VERSION " (Codename: " OS_VERSION_CODENAME ")\n" /* Codename for current development */ \
    " Latest Official Version: osal v5.0.0"                 /* For full support please use official release version */

/*! @brief Combines the revision components into a single value
 * @details Applications can check against this number @n
 * e.g. "#if OSAL_API_VERSION >= 40100" would check if some feature added in
OSAL 4.1 is present.
 */
#define OSAL_API_VERSION ((OS_MAJOR_VERSION * 10000) + (OS_MINOR_VERSION * 100) + OS_REVISION)

/*
 * Functions to get OSAL version info
 *
 * It is preferable to use the functions below to retrieve the OSAL version
 * at runtime, because if applications reference the macros above directly, the
 * macro will only get evaluated when the _application_ is built.
 *
 * When using the functions below, the version gets evaluated when the OSAL library
 * is built, and therefore if the OSAL library is re-linked without rebuilding the
 * application itself, the version will still be correct.
 */

/**
 * Gets the OSAL version/baseline ID as a string
 *
 * This returns the content of the #OS_VERSION macro defined above, and is specifically
 * just the baseline and development build ID (if applicable), without any extra info.
 *
 * \returns Basic version identifier.  This is a fixed value string and is never NULL.
 */
const char *OS_GetVersionString(void);

/**
 * Gets the OSAL version code name
 *
 * All NASA CFE/CFS components (including CFE framework, OSAL and PSP) that work
 * together will share the same code name.
 *
 * \returns OSAL code name.  This is a fixed value string and is never NULL.
 */
const char *OS_GetVersionCodeName(void);

/**
 * \brief Obtain the OSAL numeric version number
 *
 * This retrieves the numeric OSAL version identifier as an array of 4 uint8 values.
 *
 * The array of numeric values is in order of precedence:
 *  [0] = Major Number
 *  [1] = Minor Number
 *  [2] = Revision Number
 *  [3] = Mission Revision
 *
 * The "Mission Revision" (last output) also indicates whether this is an
 * official release, a patched release, or a development version.
 *  0 indicates an official release
 *  1-254 local patch level (reserved for mission use)
 *  255 indicates a development build
 *
 * \param[out] VersionNumbers  A fixed-size array to be filled with the version numbers
 */
void OS_GetVersionNumber(uint8 VersionNumbers[4]);

/**
 * \brief Obtain the OSAL library numeric build number
 *
 * The build number is a monotonically increasing number that (coarsely)
 * reflects the number of commits/changes that have been merged since the
 * epoch release.  During development cycles this number should increase
 * after each subsequent merge/modification.
 *
 * Like other version information, this is a fixed number assigned at compile time.
 *
 * \returns The OSAL library build number
 */
uint32 OS_GetBuildNumber(void);

#endif /* OSAPI_VERSION_H */
