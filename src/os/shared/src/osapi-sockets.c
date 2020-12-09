/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file     osapi-sockets.c
 * \ingroup  shared
 * \author   joseph.p.hickey@nasa.gov
 *
 *         This file  contains some of the OS APIs abstraction layer code
 *         that is shared/common across all OS-specific implementations.
 *
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * User defined include files
 */
#include "os-shared-idmap.h"
#include "os-shared-file.h"
#include "os-shared-sockets.h"

/*
 * Global data for the API
 */
enum
{
    LOCAL_NUM_OBJECTS = OS_MAX_NUM_OPEN_FILES,
    LOCAL_OBJID_TYPE  = OS_OBJECT_TYPE_OS_STREAM
};

/****************************************************************************************
                                Init Functions
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_SocketAPI_Init
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAPI_Init(void)
{
    /*
     * Placeholder - nothing right now
     * also serves to make this a non-empty compilation unit for
     * cases where OS_INCLUDE_NETWORK is off
     */
    return OS_SUCCESS;
} /* end OS_SocketAPI_Init */

/****************************************************************************************
                                Local Helper Functions
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 * Function: OS_CreateSocketName
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
void OS_CreateSocketName(const OS_object_token_t *token, const OS_SockAddr_t *Addr, const char *parent_name)
{
    size_t                       len;
    uint16                       port;
    OS_stream_internal_record_t *sock;

    sock = OS_OBJECT_TABLE_GET(OS_stream_table, *token);

    if (OS_SocketAddrToString_Impl(sock->stream_name, OS_MAX_API_NAME, Addr) != OS_SUCCESS)
    {
        sock->stream_name[0] = 0;
    }
    if (OS_SocketAddrGetPort_Impl(&port, Addr) == OS_SUCCESS)
    {
        len = strlen(sock->stream_name);
        snprintf(&sock->stream_name[len], OS_MAX_API_NAME - len, ":%u", (unsigned int)port);
    }
    sock->stream_name[OS_MAX_API_NAME - 1] = 0;

    if (parent_name)
    {
        /* Append the name from the parent socket. */
        len = strlen(sock->stream_name);
        snprintf(&sock->stream_name[len], sizeof(sock->stream_name) - len, "-%s", parent_name);
        sock->stream_name[sizeof(sock->stream_name) - 1] = 0;
    }
} /* end OS_CreateSocketName */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketOpen
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketOpen(osal_id_t *sock_id, OS_SocketDomain_t Domain, OS_SocketType_t Type)
{
    OS_object_token_t            token;
    OS_stream_internal_record_t *stream;
    int32                        return_code;

    /* Check for NULL pointers */
    if (sock_id == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /* Note - the common ObjectIdAllocate routine will lock the object type and leave it locked. */
    return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, NULL, &token);
    if (return_code == OS_SUCCESS)
    {
        stream = OS_OBJECT_TABLE_GET(OS_stream_table, token);

        /* Save all the data to our own internal table */
        memset(stream, 0, sizeof(OS_stream_internal_record_t));
        stream->socket_domain = Domain;
        stream->socket_type   = Type;

        /* Now call the OS-specific implementation.  This reads info from the table. */
        return_code = OS_SocketOpen_Impl(&token);

        /* Check result, finalize record, and unlock global table. */
        return_code = OS_ObjectIdFinalizeNew(return_code, &token, sock_id);
    }

    return return_code;
} /* end OS_SocketOpen */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketBind
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketBind(osal_id_t sock_id, const OS_SockAddr_t *Addr)
{
    OS_common_record_t *         record;
    OS_stream_internal_record_t *stream;
    OS_object_token_t            token;
    int32                        return_code;

    /* Check Parameters */
    if (Addr == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, sock_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_stream_table, token);
        stream = OS_OBJECT_TABLE_GET(OS_stream_table, token);

        if (stream->socket_domain == OS_SocketDomain_INVALID)
        {
            /* Not a socket */
            return_code = OS_ERR_INCORRECT_OBJ_TYPE;
        }
        else if (record->refcount != 0 || (stream->stream_state & (OS_STREAM_STATE_BOUND | OS_STREAM_STATE_CONNECTED)) != 0)
        {
            /* Socket must be neither bound nor connected */
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }
        else
        {
            return_code = OS_SocketBind_Impl(&token, Addr);

            if (return_code == OS_SUCCESS)
            {
                OS_CreateSocketName(&token, Addr, NULL);
                record->name_entry = stream->stream_name;
                stream->stream_state |= OS_STREAM_STATE_BOUND;
            }
        }

        OS_ObjectIdRelease(&token);
    }

    return return_code;

} /* end OS_SocketBind */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketAccept
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAccept(osal_id_t sock_id, osal_id_t *connsock_id, OS_SockAddr_t *Addr, int32 timeout)
{
    OS_common_record_t *         sock_record;
    OS_common_record_t *         conn_record;
    OS_stream_internal_record_t *sock;
    OS_stream_internal_record_t *conn;
    OS_object_token_t            sock_token;
    OS_object_token_t            conn_token;
    int32                        return_code;

    /* Check Parameters */
    if (Addr == NULL || connsock_id == NULL)
    {
        return OS_INVALID_POINTER;
    }

    /*
     * Note: setting "connrecord" here avoids a false warning
     * from static analysis tools about the value being
     * possibly used uninitialized (it cannot be, because
     * return_code is checked, and return_code is only
     * set to OS_SUCCESS when connrecord is also initialized)
     */
    conn_record = NULL;
    sock_record = NULL;
    sock        = NULL;
    conn        = NULL;
    memset(&sock_token, 0, sizeof(sock_token));
    memset(&conn_token, 0, sizeof(conn_token));

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, sock_id, &sock_token);
    if (return_code == OS_SUCCESS)
    {
        sock_record = OS_OBJECT_TABLE_GET(OS_global_stream_table, sock_token);
        sock        = OS_OBJECT_TABLE_GET(OS_stream_table, sock_token);

        if (sock->socket_type != OS_SocketType_STREAM)
        {
            /* Socket must be of the STREAM variety */
            return_code = OS_ERR_INCORRECT_OBJ_TYPE;
        }
        else if ((sock->stream_state & (OS_STREAM_STATE_BOUND | OS_STREAM_STATE_CONNECTED)) != OS_STREAM_STATE_BOUND)
        {
            /* Socket must be bound but not connected */
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }
        else
        {
            /* Now create a unique ID for the connection */
            return_code = OS_ObjectIdAllocateNew(LOCAL_OBJID_TYPE, NULL, &conn_token);
            if (return_code == OS_SUCCESS)
            {
                conn_record = OS_OBJECT_TABLE_GET(OS_global_stream_table, conn_token);
                conn        = OS_OBJECT_TABLE_GET(OS_stream_table, sock_token);

                /* Incr the refcount to record the fact that an operation is pending on this */
                memset(conn, 0, sizeof(OS_stream_internal_record_t));

                conn->socket_domain = sock->socket_domain;
                conn->socket_type   = sock->socket_type;

                /* bumps up the refcount by 1 before finalizing,
                 * to avoid having to re-acquire (should be cleaned up) */
                ++conn_record->refcount;

                return_code = OS_ObjectIdFinalizeNew(return_code, &conn_token, connsock_id);
            }
        }
    }

    if (return_code == OS_SUCCESS)
    {
        OS_SocketAddrInit_Impl(Addr, sock->socket_domain);

        /* The actual accept impl is done without global table lock, only refcount lock */
        return_code = OS_SocketAccept_Impl(&sock_token, &conn_token, Addr, timeout);
    }

    if (conn_record != NULL)
    {
        OS_Lock_Global(LOCAL_OBJID_TYPE);

        if (return_code == OS_SUCCESS)
        {
            /* Generate an entry name based on the remote address */
            OS_CreateSocketName(&conn_token, Addr, sock_record->name_entry);
            conn_record->name_entry = conn->stream_name;
            conn->stream_state |= OS_STREAM_STATE_CONNECTED;
        }
        else
        {
            /* Clear the connrecord */
            conn_record->active_id = OS_OBJECT_ID_UNDEFINED;
        }

        /* Decrement both ref counters that were increased earlier */
        --conn_record->refcount;
        OS_Unlock_Global(LOCAL_OBJID_TYPE);
    }

    OS_ObjectIdRelease(&sock_token);

    return return_code;
} /* end OS_SocketAccept */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketConnect
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketConnect(osal_id_t sock_id, const OS_SockAddr_t *Addr, int32 Timeout)
{
    OS_stream_internal_record_t *stream;
    OS_common_record_t *         record;
    OS_object_token_t            token;
    int32                        return_code;

    /* Check Parameters */
    if (Addr == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, sock_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_stream_table, token);
        stream = OS_OBJECT_TABLE_GET(OS_stream_table, token);

        if (stream->socket_domain == OS_SocketDomain_INVALID)
        {
            return_code = OS_ERR_INCORRECT_OBJ_TYPE;
        }
        else if (stream->socket_type == OS_SocketType_STREAM && (stream->stream_state & OS_STREAM_STATE_CONNECTED) != 0)
        {
            /* Stream socket must not be connected */
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }
        else
        {
            ++record->refcount;
        }

        OS_ObjectIdRelease(&token);
    }

    if (return_code == OS_SUCCESS)
    {
        return_code = OS_SocketConnect_Impl(&token, Addr, Timeout);

        OS_Lock_Global(LOCAL_OBJID_TYPE);
        if (return_code == OS_SUCCESS)
        {
            stream->stream_state |= OS_STREAM_STATE_CONNECTED | OS_STREAM_STATE_READABLE | OS_STREAM_STATE_WRITABLE;
        }
        --record->refcount;
        OS_Unlock_Global(LOCAL_OBJID_TYPE);
    }

    return return_code;
} /* end OS_SocketConnect */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketRecvFrom
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketRecvFrom(osal_id_t sock_id, void *buffer, size_t buflen, OS_SockAddr_t *RemoteAddr, int32 timeout)
{
    OS_stream_internal_record_t *stream;
    OS_object_token_t            token;
    int32                        return_code;

    /* Check Parameters */
    if (buffer == NULL || buflen == 0)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, sock_id, &token);
    if (return_code == OS_SUCCESS)
    {
        stream = OS_OBJECT_TABLE_GET(OS_stream_table, token);

        if (stream->socket_type != OS_SocketType_DATAGRAM)
        {
            return_code = OS_ERR_INCORRECT_OBJ_TYPE;
        }
        else if ((stream->stream_state & OS_STREAM_STATE_BOUND) == 0)
        {
            /* Socket needs to be bound first */
            return_code = OS_ERR_INCORRECT_OBJ_STATE;
        }
        else
        {
            return_code = OS_SocketRecvFrom_Impl(&token, buffer, buflen, RemoteAddr, timeout);
        }

        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_SocketRecvFrom */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketSendTo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketSendTo(osal_id_t sock_id, const void *buffer, size_t buflen, const OS_SockAddr_t *RemoteAddr)
{
    OS_stream_internal_record_t *stream;
    OS_object_token_t            token;
    int32                        return_code;

    /* Check Parameters */
    if (buffer == NULL || buflen == 0)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, LOCAL_OBJID_TYPE, sock_id, &token);
    if (return_code == OS_SUCCESS)
    {
        stream = OS_OBJECT_TABLE_GET(OS_stream_table, token);

        if (stream->socket_type != OS_SocketType_DATAGRAM)
        {
            return_code = OS_ERR_INCORRECT_OBJ_TYPE;
        }
        else
        {
            return_code = OS_SocketSendTo_Impl(&token, buffer, buflen, RemoteAddr);
        }

        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_SocketSendTo */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketGetIdByName
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketGetIdByName(osal_id_t *sock_id, const char *sock_name)
{
    int32 return_code;

    if (sock_id == NULL || sock_name == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return_code = OS_ObjectIdFindByName(LOCAL_OBJID_TYPE, sock_name, sock_id);

    return return_code;
} /* end OS_SocketGetIdByName */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketGetInfo
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketGetInfo(osal_id_t sock_id, OS_socket_prop_t *sock_prop)
{
    OS_common_record_t *record;
    OS_object_token_t   token;
    int32               return_code;

    /* Check parameters */
    if (sock_prop == NULL)
    {
        return OS_INVALID_POINTER;
    }

    memset(sock_prop, 0, sizeof(OS_socket_prop_t));

    /* Check Parameters */
    return_code = OS_ObjectIdGetById(OS_LOCK_MODE_GLOBAL, LOCAL_OBJID_TYPE, sock_id, &token);
    if (return_code == OS_SUCCESS)
    {
        record = OS_OBJECT_TABLE_GET(OS_global_stream_table, token);

        strncpy(sock_prop->name, record->name_entry, OS_MAX_API_NAME - 1);
        sock_prop->creator = record->creator;
        return_code        = OS_SocketGetInfo_Impl(&token, sock_prop);

        OS_ObjectIdRelease(&token);
    }

    return return_code;
} /* end OS_SocketGetInfo */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrInit
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrInit(OS_SockAddr_t *Addr, OS_SocketDomain_t Domain)
{
    if (Addr == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_SocketAddrInit_Impl(Addr, Domain);
} /* end OS_SocketAddrInit */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrToString
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrToString(char *buffer, size_t buflen, const OS_SockAddr_t *Addr)
{
    if (Addr == NULL || buffer == NULL || buflen == 0)
    {
        return OS_INVALID_POINTER;
    }

    return OS_SocketAddrToString_Impl(buffer, buflen, Addr);
} /* end OS_SocketAddrToString */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrFromString
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrFromString(OS_SockAddr_t *Addr, const char *string)
{
    if (Addr == NULL || string == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_SocketAddrFromString_Impl(Addr, string);
} /* end OS_SocketAddrFromString */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrGetPort
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrGetPort(uint16 *PortNum, const OS_SockAddr_t *Addr)
{
    if (PortNum == NULL || Addr == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_SocketAddrGetPort_Impl(PortNum, Addr);
} /* end OS_SocketAddrGetPort */

/*----------------------------------------------------------------
 *
 * Function: OS_SocketAddrSetPort
 *
 *  Purpose: Implemented per public OSAL API
 *           See description in API and header file for detail
 *
 *-----------------------------------------------------------------*/
int32 OS_SocketAddrSetPort(OS_SockAddr_t *Addr, uint16 PortNum)
{
    if (Addr == NULL)
    {
        return OS_INVALID_POINTER;
    }

    return OS_SocketAddrSetPort_Impl(Addr, PortNum);
} /* end OS_SocketAddrSetPort */
