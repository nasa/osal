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
 * \ingroup ut-stubs
 *
 * OSAL coverage stub basic data types
 */

#ifndef OCS_BASETYPES_H
#define OCS_BASETYPES_H

/*
 * NOTE: These header files are intentionally _not_ overridden
 * in the replacement/override header directory, so this should
 * pull in the actual (native system) version of these files.
 *
 * It is important to pull in these definitions first before any
 * potential re-mapping (#define) statements are done.
 */

#include <stddef.h>  /* for correct size_t and ptrdiff_t types */
#include <stdint.h>  /* for correct fixed-width integer types */
#include <limits.h>  /* for correct INT_MAX, etc. */
#include <stdbool.h> /* for correct boolean semantics */

#endif /* OCS_BASETYPES_H */
