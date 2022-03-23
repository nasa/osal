/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for arpa/inet.h
 */

#ifndef OVERRIDE_ARPA_INET_H
#define OVERRIDE_ARPA_INET_H

#include "OCS_arpa_inet.h"

/* ----------------------------------------- */
/* mappings for declarations in arpa/inet.h */
/* ----------------------------------------- */
#define inet_ntop OCS_inet_ntop
#define inet_pton OCS_inet_pton

#endif /* OVERRIDE_ARPA_INET_H */
