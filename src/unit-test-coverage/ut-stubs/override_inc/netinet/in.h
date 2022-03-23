/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for netinet/in.h
 */

#ifndef OVERRIDE_NETINET_IN_H
#define OVERRIDE_NETINET_IN_H

#include "OCS_netinet_in.h"

/* ----------------------------------------- */
/* mappings for declarations in netinet/in.h */
/* ----------------------------------------- */

#define htons OCS_htons
#define ntohs OCS_ntohs
#define htonl OCS_htonl
#define ntohl OCS_ntohl

#endif /* OVERRIDE_NETINET_IN_H */
