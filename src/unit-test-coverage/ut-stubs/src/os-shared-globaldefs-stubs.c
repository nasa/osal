/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in os-shared-globaldefs header
 */

#include <stdarg.h>

#include "os-shared-globaldefs.h"
#include "utgenstub.h"

/*
 * ----------------------------------------------------
 * Generated stub function for OS_DebugPrintf()
 * ----------------------------------------------------
 */
void OS_DebugPrintf(uint32 Level, const char *Func, uint32 Line, const char *Format, ...)
{
    va_list UtStub_ArgList;

    UT_GenStub_AddParam(OS_DebugPrintf, uint32, Level);
    UT_GenStub_AddParam(OS_DebugPrintf, const char *, Func);
    UT_GenStub_AddParam(OS_DebugPrintf, uint32, Line);
    UT_GenStub_AddParam(OS_DebugPrintf, const char *, Format);

    va_start(UtStub_ArgList, Format);
    UT_GenStub_Execute(OS_DebugPrintf, Va, NULL, UtStub_ArgList);
    va_end(UtStub_ArgList);
}
