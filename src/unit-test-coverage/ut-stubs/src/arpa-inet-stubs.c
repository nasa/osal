/**
 * \brief    Stubs for arpa/inet.h
 * \ingroup  ut-stubs
 */
#include <arpa/inet.h>
#include "utstubs.h"
#include "OCS_arpa_inet.h"

const char *OCS_inet_ntop(int af, const void *cp, char *buf, size_t len)
{
    int32 Status;

    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_ntop), af);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_ntop), cp);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_ntop), buf);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_ntop), len);

    Status = UT_DEFAULT_IMPL(OCS_inet_ntop);

    if (Status == 0)
    {
        /* "nominal" response */
        return buf;
    }

    return (char *)0;
}

int OCS_inet_pton(int af, const char *cp, void *buf)
{
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_pton), af);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_pton), cp);
    UT_Stub_RegisterContextGenericArg(UT_KEY(OCS_inet_pton), buf);

    return UT_DEFAULT_IMPL(OCS_inet_pton);
}
