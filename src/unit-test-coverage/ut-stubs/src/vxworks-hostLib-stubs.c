/**
 * \file
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 */

/* OSAL coverage stub replacement for hostLib.h */
#include <string.h>
#include <stdlib.h>
#include "utstubs.h"

#include "OCS_hostLib.h"

int OCS_hostGetByName(char *name)
{
    return (UT_DEFAULT_IMPL(OCS_hostGetByName));
}
