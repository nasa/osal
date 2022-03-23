/**
 * \file
 *
 * Owner: Tam Ngo
 * Date:  November 2014
 */

#ifndef UT_OSLOADER_TEST_PLATFORMS_H
#define UT_OSLOADER_TEST_PLATFORMS_H

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

#define UT_OS_GENERIC_MODULE_DIR      "/utmod/"
#define UT_OS_GENERIC_MODULE_BASENAME "MODULE"

#define UT_OS_GENERIC_MODULE_NAME1 UT_OS_GENERIC_MODULE_DIR UT_OS_GENERIC_MODULE_BASENAME "0" OS_MODULE_FILE_EXTENSION
#define UT_OS_GENERIC_MODULE_NAME2 UT_OS_GENERIC_MODULE_DIR UT_OS_GENERIC_MODULE_BASENAME "1" OS_MODULE_FILE_EXTENSION

#define UT_OS_GENERIC_MODULE_NAME_TEMPLATE UT_OS_GENERIC_MODULE_BASENAME "%d"
#define UT_OS_GENERIC_MODULE_FILE_TEMPLATE \
    UT_OS_GENERIC_MODULE_DIR UT_OS_GENERIC_MODULE_NAME_TEMPLATE OS_MODULE_FILE_EXTENSION

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/

#endif /* UT_OSLOADER_TEST_PLATFORMS_H */
