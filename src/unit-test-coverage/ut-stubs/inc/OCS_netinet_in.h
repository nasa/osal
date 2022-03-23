/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for netinet/in.h
 */

#ifndef OCS_NETINET_IN_H
#define OCS_NETINET_IN_H

#include "OCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in netinet/in.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in netinet/in.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* prototypes normally declared in netinet/in.h */
/* ----------------------------------------- */

extern uint16_t OCS_htons(uint16_t hostshort);
extern uint16_t OCS_ntohs(uint16_t netshort);
extern uint32_t OCS_htonl(uint32_t hostlong);
extern uint32_t OCS_ntohl(uint32_t netlong);

#endif /* OCS_NETINET_IN_H */
