/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "OCS_bsp-impl.h"

/*----------------------------------------------------------------
   Stub for OS_BSP_Lock_Impl
 ------------------------------------------------------------------*/
void OCS_OS_BSP_Lock_Impl(void)
{
    UT_DEFAULT_IMPL(OCS_OS_BSP_Lock_Impl);
}

/*----------------------------------------------------------------
   Stub for OS_BSP_Unlock_Impl
 ------------------------------------------------------------------*/
void OCS_OS_BSP_Unlock_Impl(void)
{
    UT_DEFAULT_IMPL(OCS_OS_BSP_Unlock_Impl);
}

/*----------------------------------------------------------------
   Function: OS_BSP_ConsoleOutput_Impl

    Purpose: Low level raw console data output.  Writes a sequence of
             characters directly to the BSP debug terminal or console device.

             The string is not required to be null terminated, and
             any control characters will be passed through.  Any
             non-printable ASCII codes will have platform-defined
             interpretation.

       Note: This should write the string as-is without buffering.
 ------------------------------------------------------------------*/
void OCS_OS_BSP_ConsoleOutput_Impl(const char *Str, size_t DataLen)
{
    int32_t retcode = UT_DEFAULT_IMPL(OCS_OS_BSP_ConsoleOutput_Impl);

    if (retcode == 0)
    {
        UT_Stub_CopyFromLocal(UT_KEY(OCS_OS_BSP_ConsoleOutput_Impl), Str, DataLen);
    }
}

/*----------------------------------------------------------------
   Function: OS_BSP_ConsoleSetMode_Impl

    Purpose: Set the console output mode, if supported by the BSP.

             Causes any future text written to the debug console to
             be colored/highlighted accordingly.  Intended for use
             with test applications where certain messages may need
             visual distinction (e.g. failures).

             See the OS_BSP_CONSOLEMODE constants for possible values.
             Values may be bitwise OR'ed together.

             This call is ignored if the BSP does not support console
             control codes.
 ------------------------------------------------------------------*/
void OCS_OS_BSP_ConsoleSetMode_Impl(uint32_t ModeBits)
{
    int32_t retcode = UT_DEFAULT_IMPL(OCS_OS_BSP_ConsoleSetMode_Impl);

    if (retcode == 0)
    {
        UT_Stub_CopyFromLocal(UT_KEY(OCS_OS_BSP_ConsoleSetMode_Impl), &ModeBits, sizeof(ModeBits));
    }
}
