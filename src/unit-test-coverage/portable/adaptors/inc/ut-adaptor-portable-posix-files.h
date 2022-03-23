/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-portable-posix-files
 */

#ifndef UT_ADAPTOR_PORTABLE_POSIX_FILES_H
#define UT_ADAPTOR_PORTABLE_POSIX_FILES_H

#include "common_types.h"
#include "OCS_sys_types.h"

OCS_uid_t UT_PortablePosixFileTest_GetSelfEUID(void);
OCS_gid_t UT_PortablePosixFileTest_GetSelfEGID(void);

#endif /* UT_ADAPTOR_PORTABLE_POSIX_FILES_H */
