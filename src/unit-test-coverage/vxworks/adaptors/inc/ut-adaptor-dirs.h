/**
 * \file
 * \ingroup adaptors
 *
 * Declarations and prototypes for ut-adaptor-dirs
 */

#ifndef UT_ADAPTOR_DIRS_H
#define UT_ADAPTOR_DIRS_H

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
extern int32 UT_Call_OS_VxWorks_DirAPI_Impl_Init(void);

#endif /* UT_ADAPTOR_DIRS_H */
