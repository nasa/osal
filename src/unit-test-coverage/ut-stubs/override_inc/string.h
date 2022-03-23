/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for string.h
 */

#ifndef OVERRIDE_STRING_H
#define OVERRIDE_STRING_H

#include "OCS_string.h"

/* ----------------------------------------- */
/* mappings for declarations in string.h */
/* ----------------------------------------- */
#define memchr   OCS_memchr
#define memcpy   OCS_memcpy
#define memset   OCS_memset
#define strcmp   OCS_strcmp
#define strcpy   OCS_strcpy
#define strlen   OCS_strlen
#define strncmp  OCS_strncmp
#define strncpy  OCS_strncpy
#define strchr   OCS_strchr
#define strrchr  OCS_strrchr
#define strcat   OCS_strcat
#define strncat  OCS_strncat
#define strerror OCS_strerror

#endif /* OVERRIDE_STRING_H */
