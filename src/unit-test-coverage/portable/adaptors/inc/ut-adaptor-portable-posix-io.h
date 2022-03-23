/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-portable-posix-io
 */

#ifndef UT_ADAPTOR_PORTABLE_POSIX_IO_H
#define UT_ADAPTOR_PORTABLE_POSIX_IO_H

#include "common_types.h"

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not exposed directly through the implementation API.
 *
 *****************************************************/
void UT_PortablePosixIOTest_Set_Selectable(osal_index_t local_id, bool is_selectable);
void UT_PortablePosixIOTest_Set_FD(osal_index_t local_id, int fd);
bool UT_PortablePosixIOTest_Get_Selectable(osal_index_t local_id);
void UT_PortablePosixIOTest_ResetImpl(osal_index_t local_id);

#endif /* UT_ADAPTOR_PORTABLE_POSIX_IO_H */
