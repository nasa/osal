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
#include "os-impl-dirs.h"
#include "os-shared-dir.h"

OS_impl_dir_internal_record_t OS_impl_dir_table[OS_MAX_NUM_OPEN_DIRS];
