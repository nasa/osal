/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/*
 * File: utassert.c
 *
 * Purpose: This code implements a standard set of asserts for use in unit tests.
 */

/*
 * Includes
 */
#include <stdlib.h>
#include <stdarg.h>

#include "common_types.h"
#include "utassert.h"
#include "uttools.h"
#include "utbsp.h"

/*
 * Local Data
 */

UtAssert_CaseType_t    DefaultContext     = UTASSERT_CASETYPE_FAILURE;
UtAssert_TestCounter_t UT_SegmentCounters = {0};
UtAssert_TestCounter_t UT_TotalCounters   = {0};
static char            CurrentSegment[64];

/*
 * Function Definitions
 */

void UtAssert_DoReport(const char *File, uint32 LineNum, uint32 SegmentNum, uint32 TestSeq, uint8 MessageType,
                       const char *SubsysName, const char *ShortDesc)
{
    uint32      FileLen;
    const char *BasePtr;
    char        ReportBuffer[320];

    FileLen = strlen(File);
    BasePtr = File + FileLen;
    while (FileLen > 0)
    {
        --BasePtr;
        --FileLen;
        if (*BasePtr == '/' || *BasePtr == '\\')
        {
            ++BasePtr;
            break;
        }
    }

    snprintf(ReportBuffer, sizeof(ReportBuffer), "%02u.%03u %s:%u - %s", (unsigned int)SegmentNum,
             (unsigned int)TestSeq, BasePtr, (unsigned int)LineNum, ShortDesc);

    UT_BSP_DoText(MessageType, ReportBuffer);
}

void UtAssert_DoTestSegmentReport(const char *SegmentName, const UtAssert_TestCounter_t *TestCounters)
{
    char ReportBuffer[144];

    snprintf(ReportBuffer, sizeof(ReportBuffer),
             "%02u %-20s TOTAL::%-4u  PASS::%-4u  FAIL::%-4u  MIR::%-4u  TSF::%-4u  TTF::%-4u  WARN::%-4u\n",
             (unsigned int)TestCounters->TestSegmentCount, SegmentName, (unsigned int)TestCounters->TotalTestCases,
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_PASS],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_FAILURE],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_MIR],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_TSF],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_TTF],
             (unsigned int)TestCounters->CaseCount[UTASSERT_CASETYPE_WARN]);

    UT_BSP_DoText(UTASSERT_CASETYPE_END, ReportBuffer);
}

uint32 UtAssert_GetPassCount(void)
{
    return UT_TotalCounters.CaseCount[UTASSERT_CASETYPE_PASS];
}

uint32 UtAssert_GetFailCount(void)
{
    return UT_TotalCounters.CaseCount[UTASSERT_CASETYPE_FAILURE];
}

const UtAssert_TestCounter_t *UtAssert_GetCounters(void)
{
    return &UT_TotalCounters;
}

void UtAssert_BeginTest(const char *SegmentName)
{
    uint32 TestSegmentCount;

    UT_BSP_Lock();

    memset(&UT_SegmentCounters, 0, sizeof(UT_SegmentCounters));
    strncpy(CurrentSegment, SegmentName, sizeof(CurrentSegment) - 1);
    CurrentSegment[sizeof(CurrentSegment) - 1] = 0;
    TestSegmentCount                           = 1 + UT_TotalCounters.TestSegmentCount;

    UT_BSP_Unlock();

    UT_BSP_StartTestSegment(TestSegmentCount, SegmentName);
}

const char *UtAssert_GetSegmentName(void)
{
    return CurrentSegment;
}

void UtAssert_EndTest(void)
{
    uint32                 Ct;
    bool                   SegmentValid;
    UtAssert_TestCounter_t Local_SegmentCounters;
    char                   Local_SegmentName[sizeof(CurrentSegment)];

    UT_BSP_Lock();

    SegmentValid = (UT_SegmentCounters.TotalTestCases > 0);
    if (SegmentValid)
    {
        ++UT_TotalCounters.TestSegmentCount;
        UT_SegmentCounters.TestSegmentCount = UT_TotalCounters.TestSegmentCount;
        UT_TotalCounters.TotalTestCases += UT_SegmentCounters.TotalTestCases;
        for (Ct = 0; Ct < UTASSERT_CASETYPE_MAX; ++Ct)
        {
            UT_TotalCounters.CaseCount[Ct] += UT_SegmentCounters.CaseCount[Ct];
        }
        memcpy(&Local_SegmentCounters, &UT_SegmentCounters, sizeof(Local_SegmentCounters));

        /*
         * note, strcpy is OK because both are fixed size buffers of the same size,
         * and the null termination on CurrentSegment was locally enforced already
         */
        strcpy(Local_SegmentName, CurrentSegment);
    }

    memset(&UT_SegmentCounters, 0, sizeof(UT_SegmentCounters));

    UT_BSP_Unlock();

    if (SegmentValid)
    {
        UtAssert_DoTestSegmentReport(Local_SegmentName, &Local_SegmentCounters);
    }
    else
    {
        UT_BSP_DoText(UTASSERT_CASETYPE_END, "No test cases\n");
    }
}

void UtAssert_SetContext(UtAssert_CaseType_t Context)
{
    DefaultContext = Context;
}

UtAssert_CaseType_t UtAssert_GetContext(void)
{
    return DefaultContext;
}

bool UtAssert(bool Expression, const char *Description, const char *File, uint32 Line)
{
    return UtAssertEx(Expression, UtAssert_GetContext(), File, Line, "%s", Description);
}

bool UtAssertEx(bool Expression, UtAssert_CaseType_t CaseType, const char *File, uint32 Line, const char *MessageFormat,
                ...)
{
    va_list va;
    char    FinalMessage[256];
    uint32  TestSegmentCount;
    uint32  TotalTestCases;

    UT_BSP_Lock();

    ++UT_SegmentCounters.TotalTestCases;

    if (Expression)
    {
        CaseType = UTASSERT_CASETYPE_PASS;
    }

    if ((uint32)CaseType < UTASSERT_CASETYPE_MAX)
    {
        ++UT_SegmentCounters.CaseCount[(uint32)CaseType];
    }

    TestSegmentCount = 1 + UT_TotalCounters.TestSegmentCount;
    TotalTestCases   = UT_SegmentCounters.TotalTestCases;

    UT_BSP_Unlock();

    va_start(va, MessageFormat);
    vsnprintf(FinalMessage, sizeof(FinalMessage), MessageFormat, va);
    va_end(va);

    UtAssert_DoReport(File, Line, TestSegmentCount, TotalTestCases, CaseType, CurrentSegment, FinalMessage);

    return Expression;
}

void UtAssert_Abort(const char *Message)
{
    UT_BSP_DoText(UTASSERT_CASETYPE_ABORT, Message);
}

const char *UtAssert_GetCaseTypeAbbrev(UtAssert_CaseType_t CaseType)
{
    const char *AbbrevStr;

    switch (CaseType)
    {
        case UTASSERT_CASETYPE_ABORT:
            AbbrevStr = "ABORT";
            break;
        case UTASSERT_CASETYPE_FAILURE:
            AbbrevStr = "FAIL";
            break;
        case UTASSERT_CASETYPE_MIR:
            AbbrevStr = "MIR";
            break;
        case UTASSERT_CASETYPE_TSF:
            AbbrevStr = "TSF";
            break;
        case UTASSERT_CASETYPE_TTF:
            AbbrevStr = "TTF";
            break;
        case UTASSERT_CASETYPE_WARN:
            AbbrevStr = "WARN";
            break;
        case UTASSERT_CASETYPE_NA:
            AbbrevStr = "N/A";
            break;
        case UTASSERT_CASETYPE_BEGIN:
            AbbrevStr = "BEGIN";
            break;
        case UTASSERT_CASETYPE_END:
            AbbrevStr = "END";
            break;
        case UTASSERT_CASETYPE_PASS:
            AbbrevStr = "PASS";
            break;
        case UTASSERT_CASETYPE_INFO:
            AbbrevStr = "INFO";
            break;
        case UTASSERT_CASETYPE_FLOW:
            AbbrevStr = "FLOW";
            break;
        case UTASSERT_CASETYPE_DEBUG:
            AbbrevStr = "DEBUG";
            break;
        default:
            /* do not return NULL, as the result may be directly passed to C library functions */
            AbbrevStr = "OTHER";
            break;
    }

    return AbbrevStr;
}

void UtAssert_Message(uint8 MessageType, const char *File, uint32 Line, const char *Spec, ...)
{
    va_list     va;
    char        FinalMessage[256];
    const char *BaseName;
    size_t      MsgLen;

    if (File != NULL)
    {
        BaseName = strrchr(File, '/');
        if (BaseName == NULL)
        {
            BaseName = File;
        }
        else
        {
            ++BaseName;
        }
        snprintf(FinalMessage, sizeof(FinalMessage), "%s:%u:", BaseName, (unsigned int)Line);
        MsgLen = strlen(FinalMessage);
    }
    else
    {
        MsgLen = 0;
    }

    va_start(va, Spec);
    vsnprintf(&FinalMessage[MsgLen], sizeof(FinalMessage) - MsgLen, Spec, va);
    va_end(va);

    UT_BSP_DoText(MessageType, FinalMessage);
}

const char *UtAssert_GetOpText(UtAssert_Compare_t CompareType)
{
    const char *OpText;

    switch (CompareType)
    {
        case UtAssert_Compare_EQ: /* actual equals reference value */
            OpText = "==";
            break;
        case UtAssert_Compare_NEQ: /* actual does not non equal reference value */
            OpText = "!=";
            break;
        case UtAssert_Compare_LT: /* actual less than reference (exclusive) */
            OpText = "<";
            break;
        case UtAssert_Compare_GT: /* actual greater than reference (exclusive)  */
            OpText = ">";
            break;
        case UtAssert_Compare_LTEQ: /* actual less than or equal to reference (inclusive) */
            OpText = "<=";
            break;
        case UtAssert_Compare_GTEQ: /* actual greater than reference (inclusive) */
            OpText = ">=";
            break;
        case UtAssert_Compare_BITMASK_SET: /* bit(s) in reference are set in actual */
            OpText = "&";
            break;
        case UtAssert_Compare_BITMASK_UNSET: /* bit(s) in reference are not set in actual */
            OpText = "&~";
            break;
        default: /* should never happen */
            OpText = "??";
            break;
    }

    return OpText;
}

bool UtAssert_GenericUnsignedCompare(unsigned long ActualValue, UtAssert_Compare_t CompareType,
                                     unsigned long ReferenceValue, UtAssert_Radix_t RadixType, const char *File,
                                     uint32 Line, const char *Desc, const char *ActualText, const char *ReferenceText)
{
    bool        Result;
    const char *FormatStr;

    switch (CompareType)
    {
        case UtAssert_Compare_EQ: /* actual equals reference value */
            Result = (ActualValue == ReferenceValue);
            break;
        case UtAssert_Compare_NEQ: /* actual does not non equal reference value */
            Result = (ActualValue != ReferenceValue);
            break;
        case UtAssert_Compare_LT: /* actual less than reference (exclusive) */
            Result = (ActualValue < ReferenceValue);
            break;
        case UtAssert_Compare_GT: /* actual greater than reference (exclusive)  */
            Result = (ActualValue > ReferenceValue);
            break;
        case UtAssert_Compare_LTEQ: /* actual less than or equal to reference (inclusive) */
            Result = (ActualValue <= ReferenceValue);
            break;
        case UtAssert_Compare_GTEQ: /* actual greater than reference (inclusive) */
            Result = (ActualValue >= ReferenceValue);
            break;
        case UtAssert_Compare_BITMASK_SET: /* bit(s) in reference are set in actual */
            Result = (ActualValue & ReferenceValue) == ReferenceValue;
            break;
        case UtAssert_Compare_BITMASK_UNSET: /* bit(s) in reference are not set in actual */
            Result = (ActualValue & ReferenceValue) == 0;
            break;
        default: /* should never happen */
            Result = false;
            break;
    }

    switch (RadixType)
    {
        case UtAssert_Radix_OCTAL:
            FormatStr = "%s%s (0%lo) %s %s (0%lo)";
            break;
        case UtAssert_Radix_DECIMAL:
            FormatStr = "%s%s (%lu) %s %s (%lu)";
            break;
        default:
            /* for unsigned, default is hex */
            FormatStr = "%s%s (0x%lx) %s %s (0x%lx)";
            break;
    }

    return UtAssertEx(Result, UTASSERT_CASETYPE_FAILURE, File, Line, FormatStr, Desc, ActualText, ActualValue,
                      UtAssert_GetOpText(CompareType), ReferenceText, ReferenceValue);
}

bool UtAssert_GenericSignedCompare(long ActualValue, UtAssert_Compare_t CompareType, long ReferenceValue,
                                   UtAssert_Radix_t RadixType, const char *File, uint32 Line, const char *Desc,
                                   const char *ActualText, const char *ReferenceText)
{
    bool        Result;
    const char *FormatStr;

    switch (CompareType)
    {
        case UtAssert_Compare_EQ: /* actual equals reference value */
            Result = (ActualValue == ReferenceValue);
            break;
        case UtAssert_Compare_NEQ: /* actual does not non equal reference value */
            Result = (ActualValue != ReferenceValue);
            break;
        case UtAssert_Compare_LT: /* actual less than reference (exclusive) */
            Result = (ActualValue < ReferenceValue);
            break;
        case UtAssert_Compare_GT: /* actual greater than reference (exclusive)  */
            Result = (ActualValue > ReferenceValue);
            break;
        case UtAssert_Compare_LTEQ: /* actual less than or equal to reference (inclusive) */
            Result = (ActualValue <= ReferenceValue);
            break;
        case UtAssert_Compare_GTEQ: /* actual greater than reference (inclusive) */
            Result = (ActualValue >= ReferenceValue);
            break;
        default: /* should never happen */
            Result = false;
            break;
    }

    switch (RadixType)
    {
        case UtAssert_Radix_OCTAL:
            FormatStr = "%s%s (0%lo) %s %s (0%lo)";
            break;
        case UtAssert_Radix_HEX:
            FormatStr = "%s%s (0x%lx) %s %s (0x%lx)";
            break;
        default:
            /* for signed, default is decimal */
            FormatStr = "%s%s (%ld) %s %s (%ld)";
            break;
    }

    return UtAssertEx(Result, UTASSERT_CASETYPE_FAILURE, File, Line, FormatStr, Desc, ActualText, ActualValue,
                      UtAssert_GetOpText(CompareType), ReferenceText, ReferenceValue);
}

bool UtAssert_StringBufCompare(const char *String1, size_t String1Max, const char *String2, size_t String2Max,
                               UtAssert_Compare_t CompareType, const char *File, uint32 Line)
{
    char        ScrubbedString1[256];
    char        ScrubbedString2[256];
    const char *EndPtr1;
    const char *EndPtr2;
    size_t      FormatLen1;
    size_t      FormatLen2;
    bool        Result;
    int         Compare;

    /* Locate the actual end of both strings */
    if (String1 == NULL)
    {
        EndPtr1 = NULL;
    }
    else if (String1Max == UTASSERT_STRINGBUF_NULL_TERM)
    {
        /*
         * NOTE: it is technically undefined behavior to pass a size to memchr()
         * that is larger than the actual buffer, even if it is known/guaranteed
         * to find a match within the actual buffer.  Therefore the regular strlen()
         * is used instead.
         */
        EndPtr1 = String1 + strlen(String1);
    }
    else
    {
        EndPtr1 = memchr(String1, 0, String1Max);
    }

    if (EndPtr1 != NULL)
    {
        FormatLen1 = EndPtr1 - String1;
    }
    else
    {
        FormatLen1 = String1Max;
    }

    if (String2 == NULL)
    {
        EndPtr2 = NULL;
    }
    else if (String2Max == UTASSERT_STRINGBUF_NULL_TERM)
    {
        EndPtr2 = String2 + strlen(String2);
    }
    else
    {
        EndPtr2 = memchr(String2, 0, String2Max);
    }

    if (EndPtr2 != NULL)
    {
        FormatLen2 = EndPtr2 - String2;
    }
    else
    {
        FormatLen2 = String2Max;
    }

    if (FormatLen1 == 0 && FormatLen2 == 0)
    {
        /* Two empty strings are considered equal */
        Compare = 0;
    }
    else
    {
        /* Compare actual content based on the shorter of the two strings */
        if (FormatLen1 < FormatLen2)
        {
            Compare = memcmp(String1, String2, FormatLen1);
        }
        else
        {
            Compare = memcmp(String1, String2, FormatLen2);
        }

        /* If initial content was the same, go by whichever is longer */
        if (Compare == 0)
        {
            /*
             * If String1 is longer, compare should be positive (String1 > String2)
             * If String2 is longer, compare should be negative (String1 < String2)
             * If strings are the same length, compare should be 0.
             */
            Compare = FormatLen1 - FormatLen2;
        }
    }

    switch (CompareType)
    {
        case UtAssert_Compare_EQ: /* actual equals reference value */
            Result = (Compare == 0);
            break;
        case UtAssert_Compare_NEQ: /* actual does not non equal reference value */
            Result = (Compare != 0);
            break;
        case UtAssert_Compare_LT: /* actual less than reference (exclusive) */
            Result = (Compare < 0);
            break;
        case UtAssert_Compare_GT: /* actual greater than reference (exclusive)  */
            Result = (Compare > 0);
            break;
        case UtAssert_Compare_LTEQ: /* actual less than or equal to reference (inclusive) */
            Result = (Compare <= 0);
            break;
        case UtAssert_Compare_GTEQ: /* actual greater than reference (inclusive) */
            Result = (Compare >= 0);
            break;
        default: /* should never happen */
            Result = false;
            break;
    }

    /* Now make "safe" copies of the strings */
    /* Check for a newline within the string, and if present, end the string there instead */
    if (FormatLen1 > 0)
    {
        EndPtr1 = memchr(String1, '\n', FormatLen1);
        if (EndPtr1 != NULL)
        {
            FormatLen1 = EndPtr1 - String1;
        }
        memcpy(ScrubbedString1, String1, FormatLen1);
    }
    ScrubbedString1[FormatLen1] = 0;

    if (FormatLen2 > 0)
    {
        EndPtr2 = memchr(String2, '\n', FormatLen2);
        if (EndPtr2 != NULL)
        {
            FormatLen2 = EndPtr2 - String2;
        }
        memcpy(ScrubbedString2, String2, FormatLen2);
    }
    ScrubbedString2[FormatLen2] = 0;

    return UtAssertEx(Result, UTASSERT_CASETYPE_FAILURE, File, Line, "String: \'%s\' == \'%s\'", ScrubbedString1,
                      ScrubbedString2);
}
