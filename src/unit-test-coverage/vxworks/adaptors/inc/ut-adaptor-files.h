/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-files
 */

#ifndef UT_ADAPTOR_FILES_H
#define UT_ADAPTOR_FILES_H

#include "common_types.h"

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/

/*
 * Prototype for table init function (needs to be called from UT)
 */
extern int32 UT_Call_OS_VxWorks_StreamAPI_Impl_Init(void);

/*
 * Allow UT to get the value of the OS_IMPL_SELF_EUID and
 * OS_IMPL_SELF_EGID constants.  These might be assigned
 * at runtime by the init function (above) or they might be
 * defined at compile time.
 */
unsigned int UT_FileTest_GetSelfEUID(void);
unsigned int UT_FileTest_GetSelfEGID(void);

void UT_FileTest_Set_Selectable(osal_index_t local_id, bool is_selectable);

#endif /* UT_ADAPTOR_FILES_H */
