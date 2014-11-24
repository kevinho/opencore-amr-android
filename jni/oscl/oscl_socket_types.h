/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */


#ifndef OSCL_SOCKET_TYPES_H_INCLUDED
#define OSCL_SOCKET_TYPES_H_INCLUDED

#include "osclconfig_io.h"
#include "oscl_types.h"
#include "oscl_scheduler_types.h"
#include "oscl_namestring.h"
#include "oscl_stdstring.h"

enum TPVSocketFxn
{
    EPVSocketSend = 0
    , EPVSocketSendTo
    , EPVSocketRecv
    , EPVSocketRecvFrom
    , EPVSocketConnect
    , EPVSocketAccept
    , EPVSocketShutdown
    , EPVSocketBind
    , EPVSocketListen
    , EPVSocket_Last //placeholder
} ;

/** Return codes for asynchronous APIs
*/
enum TPVSocketEvent
{
    EPVSocketSuccess
    , EPVSocketPending
    , EPVSocketTimeout
    , EPVSocketFailure
    , EPVSocketCancel
    , EPVSocketNotImplemented //for unavailable features.
} ;

enum TPVSocketShutdown
{
    EPVSocketSendShutdown
    , EPVSocketRecvShutdown
    , EPVSocketBothShutdown
} ;

class OsclSocketTOS
{
    public:
        enum TPVServicePrecedence
        {
            EPVRoutine        = 0,
            EPVPriority       = 1,
            EPVImmediate      = 2,
            EPVFlash          = 3,
            EPVOverrideFlash  = 4,
            EPVCritic_Ecp     = 5,
            EPVInetControl    = 6,
            EPVNetControl     = 7
        };

        enum TPVServicePriority
        {
            EPVNoTOS        = 0x0,
            EPVLDelay       = (1 << 4),
            EPVHiThrpt      = (1 << 3),
            EPVHiRel        = (1 << 2)
        };

        OsclSocketTOS(): iTOSUnusedBits(2), iTOSPriorityBits(3)
        {
            ClearTOS();
        }

        void SetPrecedence(TPVServicePrecedence aPrecedence)
        {
            iPrecedence = aPrecedence;
        }

        void SetPriority(bool aMinimizeDelay, bool aMaximizeThroughput, bool MaximizeReliability)
        {
            iPriority   = (aMinimizeDelay ? EPVLDelay : EPVNoTOS) | (aMaximizeThroughput ? EPVHiThrpt : EPVNoTOS) | (MaximizeReliability ? EPVHiRel : EPVNoTOS);
        }

        void ClearTOS()
        {
            iPrecedence = OsclSocketTOS::EPVRoutine;
            iPriority   = OsclSocketTOS::EPVNoTOS;
        }

        uint8 GetTOS() const
        {
            /**Format of Ip Header's TOS field as specified in RFC 791
             *   0     1     2     3     4     5     6     7
             *+-----+-----+-----+-----+-----+-----+-----+-----+
             *|   PRECEDENCE    |  D  |  T  |  R  |  0  |  0  |
             *+-----+-----+-----+-----+-----+-----+-----+-----+
            */
            return ((iPrecedence << (iTOSPriorityBits + iTOSUnusedBits) & 0xFF) || ((iPriority << iTOSUnusedBits) & 0xFF));
        }

    private:
        uint8    iPrecedence;
        uint8    iPriority;
        const uint8 iTOSUnusedBits;
        const uint8 iTOSPriorityBits;
};

enum TPVSocketOptionName
{
    //IP Level
    EPVIPMulticastTTL
    , EPVIPAddMembership
    , EPVIPTOS
    //Socket level
    , EPVSockReuseAddr  //On Symbian this option is at IP level...
} ;

enum TPVSocketOptionLevel
{
    EPVIPProtoIP
    , EPVIPProtoTCP
    , EPVSocket
} ;
#define PVNETWORKADDRESS_LEN 50

class OsclNetworkAddress
{
    public:
        OsclNetworkAddress(): port(0)
        {
        }
        OsclNetworkAddress(const char *addr, int p)
        {
            ipAddr.Set(addr);
            port = p;
        }

        //Network address in dotted decimal string format.
        OsclNameString<PVNETWORKADDRESS_LEN> ipAddr;

        //Port number.
        int port;
        //@cmember equality comparison operator
        bool operator == (const OsclNetworkAddress & rhs) const
        {
            if (port == rhs.port)
            {
                if (0 == oscl_strcmp((const char*)ipAddr.Str(), (const char*)rhs.ipAddr.Str()))
                    return true;
            }
            return false;
        };

} ;

class OsclIpMReq
{
    public:
        OsclIpMReq(
            const char *intrfcAddr
            , const char* multcstAddr)
        {
            interfaceAddr.Set(intrfcAddr);
            multicastAddr.Set(multcstAddr);
        }
        OsclNameString<PVNETWORKADDRESS_LEN> interfaceAddr;
        OsclNameString<PVNETWORKADDRESS_LEN> multicastAddr;
};

/**
* Socket event observer.  The client implements this to get
* asynchronous command completion.
*/
class OsclSocketObserver
{
    public:
        /**
         * Socket Event callback.
         *
         * @param aId: The ID that was supplied when
         *    the socket was created.
         * @param aFxn: Type of socket function call.
         * @param aEvent: Function completion event.  Will be
         *    EPVSocketSuccess, EPVSocketTimeout, or EPVSocketFailure.
         * @param aError: When the event is EPVSocketFailure, this
         *    may contain a platform-specific error code, or zero if
         *    none is available.
         */
        OSCL_IMPORT_REF virtual void HandleSocketEvent(int32 aId, TPVSocketFxn aFxn, TPVSocketEvent aEvent, int32 aError) = 0;
        virtual ~OsclSocketObserver() {}
};


#endif
