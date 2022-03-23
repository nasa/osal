/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for dosFsLib.h
 */

#ifndef OVERRIDE_DOSFSLIB_H
#define OVERRIDE_DOSFSLIB_H

#include "OCS_dosFsLib.h"
#include <vxWorks.h>

/* ----------------------------------------- */
/* mappings for declarations in dosFsLib.h */
/* ----------------------------------------- */
#define DOS_CHK_ONLY        OCS_DOS_CHK_ONLY
#define DOS_CHK_REPAIR      OCS_DOS_CHK_REPAIR
#define DOS_CHK_VERB_0      OCS_DOS_CHK_VERB_0
#define DOS_CHK_VERB_SILENT OCS_DOS_CHK_VERB_SILENT
#define DOS_OPT_BLANK       OCS_DOS_OPT_BLANK

#define dosFsVolFormat OCS_dosFsVolFormat

#endif /* OVERRIDE_DOSFSLIB_H */
