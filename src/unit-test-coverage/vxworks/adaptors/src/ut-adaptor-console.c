/**
 * \file
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* pull in the OSAL configuration */
#include "osconfig.h"
#include "ut-adaptor-console.h"

#include "os-vxworks.h"
#include "os-impl-console.h"

void *const  UT_Ref_OS_impl_console_table      = OS_impl_console_table;
size_t const UT_Ref_OS_impl_console_table_SIZE = sizeof(OS_impl_console_table);

int UT_ConsoleTest_TaskEntry(int arg)
{
    return OS_VxWorks_ConsoleTask_Entry(arg);
}
