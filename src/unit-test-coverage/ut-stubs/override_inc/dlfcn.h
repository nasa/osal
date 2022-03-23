/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for dlfcn.h
 */

#ifndef OVERRIDE_DLFCN_H
#define OVERRIDE_DLFCN_H

#include "OCS_dlfcn.h"

/* ----------------------------------------- */
/* mappings for declarations in dlfcn.h */
/* ----------------------------------------- */

#define dlclose OCS_dlclose
#define dlerror OCS_dlerror
#define dlopen  OCS_dlopen
#define dlsym   OCS_dlsym

#endif /* OVERRIDE_DLFCN_H */
