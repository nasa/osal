/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     ut-adaptor-filetable-stub.c
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include "osconfig.h"
#include "ut-adaptor-filetable-stub.h"

#include <os-posix.h>
#include <os-impl-files.h>




OS_Posix_file_internal_record_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];


void*  const UT_FileTableTest_OS_impl_filehandle_table = OS_impl_filehandle_table;
size_t const UT_FileTableTest_OS_impl_filehandle_table_SIZE = sizeof(OS_impl_filehandle_table);


uid_t OS_IMPL_SELF_EUID = 100;
gid_t OS_IMPL_SELF_EGID = 100;

/*
 * Flag(s) to set on file handles for regular files
 * This sets all regular filehandles to be non-blocking by default.
 *
 * In turn, the implementation will utilize select() to determine
 * a filehandle readiness to read/write.
 */
const int OS_IMPL_REGULAR_FILE_FLAGS = 1;

