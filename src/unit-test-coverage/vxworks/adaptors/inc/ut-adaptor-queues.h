/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-queues
 */

#ifndef UT_ADAPTOR_QUEUES_H
#define UT_ADAPTOR_QUEUES_H

#include "common_types.h"
#include "OCS_msgQLib.h"

extern void *const  UT_Ref_OS_impl_queue_table;
extern size_t const UT_Ref_OS_impl_queue_table_SIZE;

/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not part of the implementation API.
 *
 *****************************************************/

int32 UT_Call_OS_VxWorks_QueueAPI_Impl_Init(void);

#endif /* UT_ADAPTOR_QUEUES_H */
