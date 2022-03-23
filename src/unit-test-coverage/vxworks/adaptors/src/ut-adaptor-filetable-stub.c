/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */
/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-filetable-stub.h"

#include "os-vxworks.h"
#include "os-impl-files.h"

OS_impl_file_internal_record_t OS_impl_filehandle_table[OS_MAX_NUM_OPEN_FILES];

void *const  UT_FileTableTest_OS_impl_filehandle_table      = OS_impl_filehandle_table;
size_t const UT_FileTableTest_OS_impl_filehandle_table_SIZE = sizeof(OS_impl_filehandle_table);
