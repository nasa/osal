/**
 * \brief    Stubs for netinet/in.h
 * \ingroup  ut-stubs
 */
#include "utstubs.h"
#include "OCS_arpa_inet.h"

uint16_t OCS_htons(uint16_t hostshort)
{
    return UT_DEFAULT_IMPL(OCS_htons);
}

uint16_t OCS_ntohs(uint16_t netshort)
{
    return UT_DEFAULT_IMPL(OCS_ntohs);
}

uint32_t OCS_htonl(uint32_t hostlong)
{
    return UT_DEFAULT_IMPL(OCS_htonl);
}

uint32_t OCS_ntohl(uint32_t netlong)
{
    return UT_DEFAULT_IMPL(OCS_ntohl);
}
