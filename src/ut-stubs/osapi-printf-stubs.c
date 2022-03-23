/**
 * @file
 *
 * Auto-Generated stub implementations for functions defined in osapi-printf header
 */

#include <stdarg.h>

#include "osapi-printf.h"
#include "utgenstub.h"

extern void UT_DefaultHandler_OS_printf(void *, UT_EntryKey_t, const UT_StubContext_t *, va_list);

/*
 * ----------------------------------------------------
 * Generated stub function for OS_printf()
 * ----------------------------------------------------
 */
void OS_printf(const char *string, ...)
{
    va_list UtStub_ArgList;

    UT_GenStub_AddParam(OS_printf, const char *, string);

    va_start(UtStub_ArgList, string);
    UT_GenStub_Execute(OS_printf, Va, UT_DefaultHandler_OS_printf, UtStub_ArgList);
    va_end(UtStub_ArgList);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_printf_disable()
 * ----------------------------------------------------
 */
void OS_printf_disable(void)
{

    UT_GenStub_Execute(OS_printf_disable, Basic, NULL);
}

/*
 * ----------------------------------------------------
 * Generated stub function for OS_printf_enable()
 * ----------------------------------------------------
 */
void OS_printf_enable(void)
{

    UT_GenStub_Execute(OS_printf_enable, Basic, NULL);
}
