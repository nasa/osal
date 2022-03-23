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

/**
 * \file
 *
 * Purpose: This file contains functions to implement a set of tools for use in unit testing.
 */

#ifndef UTTOOLS_H
#define UTTOOLS_H

/*
 * Includes
 */

#include "common_types.h"
#include "utassert.h"

/*
 * Exported Functions
 */

/**
 * \brief Copies a region of memory to a binary file.
 *
 * This file can be reloaded by calling UtBinFile2Mem or it can be
 * used to verify test results by calling UtMem2BinFileCmp.
 *
 * \sa UtBinFile2Mem, UtMem2BinFileCmp
 *
 * \param Memory    pointer to memory region
 * \param Filename  file name to use
 * \param Length    size of memory region
 *
 * \returns true if copy was successful
 */
bool UtMem2BinFile(const void *Memory, const char *Filename, uint32 Length);

/**
 * \brief Copies a binary file to a region of memory.
 *
 * \param Memory    pointer to memory region
 * \param Filename  file name to use
 * \param Length    size of memory region
 *
 * \returns true if copy was successful
 */
bool UtBinFile2Mem(void *Memory, const char *Filename, uint32 Length);

/**
 * \brief Copies a region of memory to a hex file
 *
 * \param Memory    pointer to memory region
 * \param Filename  file name to use
 * \param Length    size of memory region
 *
 * \returns true if copy was successful
 */
bool UtMem2HexFile(const void *Memory, const char *Filename, uint32 Length);

/**
 * \brief Fills a region of memory with a byte count pattern.
 *
 * \param Memory pointer to memory region
 * \param Length size of memory region
 */
void UtMemFill(void *Memory, uint32 Length);

/**
 * \brief Just like the standard sprintf except it returns a pointer to the result string.
 *
 * The result string cannot be larger than 256 bytes, and will be truncated if the output
 * would exceed that length
 *
 * \param Spec Printf-style format string
 *
 * \returns pointer to result string
 */
char *UtSprintf(const char *Spec, ...);

/**
 * \brief print a range of memory as hex bytes.
 *
 * \param Memory pointer to memory region
 * \param Length size of memory region
 */
void UtPrintx(const void *Memory, uint32 Length);

/**
 * \brief Compares a region of memory to a static pattern and determines if they are equal.
 *
 * \note Use standard memset() to fill a region of memory with a static value.
 *
 * \param Memory pointer to memory region
 * \param Value  static fill value to check for
 * \param Length size of memory region
 *
 * \returns true if check was successful
 */
bool UtMemCmpValue(const void *Memory, uint8 Value, uint32 Length);

/**
 * \brief Compares a region of memory to a byte count pattern and determines if they are equal.
 *
 * \note Use UtMemFill() to fill a region of memory with a byte count pattern.
 *
 * \param Memory pointer to memory region
 * \param Length size of memory region
 *
 * \returns true if check was successful
 */
bool UtMemCmpCount(const void *Memory, uint32 Length);

/**
 * \brief Compares a region of memory with the contents of a binary file and determines if they are equal.
 *
 * \note Use UtMem2BinFile to copy a region of memory to a binary file.
 *
 * The size of the file determines the size of the memory region to check.  The memory region must be
 * at least the size of the file.
 *
 * \param Memory pointer to memory region
 * \param Filename  file name to use
 *
 * \returns true if check was successful
 */
bool UtMem2BinFileCmp(const void *Memory, const char *Filename);

/* Macros to implement simple printf-like functions for unit testing */
#define UtPrintf(...) UtAssert_Message(UTASSERT_CASETYPE_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define UtDebug(...)  UtAssert_Message(UTASSERT_CASETYPE_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

#endif /* UTTOOLS_H */
