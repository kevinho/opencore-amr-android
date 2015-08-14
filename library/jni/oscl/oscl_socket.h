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
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ S O C K E T

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclio OSCL IO
 *
 * @{
 */


/*! \file oscl_socket.h
    \brief The file oscl_socket.h defines the OSCL Socket APIs.

*/
#ifndef OSCL_SOCKET_H_INCLUDED
#define OSCL_SOCKET_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_SOCKET_TYPES_H_INCLUDED
#include "oscl_socket_types.h"
#endif

#ifndef OSCL_HEAPBASE_H_INCLUDED
#include "oscl_heapbase.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

/**
* Socket server
*/
class OsclSocketServI;

class OsclSocketServ : public HeapBase
{
    public:
        /**
         * Create a socket server.
         * May leave if failure.
         *
         * @param alloc: Memory allocator.
         * @return Returns pointer to socket server
         */
        OSCL_IMPORT_REF static OsclSocketServ *NewL(Oscl_DefAlloc &alloc);

        /**
         * Destructor.  The server object must be deleted using
         * the same allocator used in the NewL call.
         */
        OSCL_IMPORT_REF ~OsclSocketServ();

        /**
         * Connect to socket server.
         * This is a synchronous method.
         *
         * @param Number of message slots.
         * @return Returns OsclErrNone for success,
         *    or a platform-specific code.
         */
        OSCL_IMPORT_REF int32 Connect(uint32 aMessageSlots = 8, bool aShareSession = false);

        /**
         * Close socket server.
         * This is a synchronous method.
         * @param aCleanup: cleanup the socket system?
         *   the socket cleanup should only be done when
         *   all parts of the application are done using
         *   sockets.
         */
        OSCL_IMPORT_REF void Close(bool aCleanup = true);

    private:
        OsclSocketServ(Oscl_DefAlloc &alloc): iServ(NULL), iAlloc(alloc) {}

        OsclSocketServI *iServ;
        Oscl_DefAlloc &iAlloc;

        friend class OsclTCPSocket;
        friend class OsclUDPSocket;
        friend class OsclDNS;

};


class OsclUDPSocketI;

/**
* The UDP Socket class
*/

class OsclUDPSocket : public HeapBase
{
    public:
        /**
         * Create a UDP Socket.
         * May leave if failure.
         *
         * @param alloc: Memory allocator.
         * @param aServ: Socket server.  Must be connected.
         * @param aObserver: Socket observer.
         * @param aId: Socket ID.  The caller must assign an ID
         *   to each socket.  The ID is used to identify the socket
         *   in observer callbacks.
         * @return Returns pointer to socket.
        */
        OSCL_IMPORT_REF static OsclUDPSocket *NewL(Oscl_DefAlloc &alloc,
                OsclSocketServ& aServ,
                OsclSocketObserver *aObserver,
                uint32 aId);

        /**
         * Destructor.  The object must be deleted using
         * the same allocator used in the NewL call.
         */
        OSCL_IMPORT_REF ~OsclUDPSocket();

        /**
         * Thread logoff routine.  This will prepare for transfer and
        * use of the socket in another thread.
         * All socket requests must be complete prior to calling this
         * routine.  If any requests are still active, it will return
        * EPVSocketFailure;
         */
        OSCL_IMPORT_REF TPVSocketEvent ThreadLogoff();
        /**
        * Thread logon routine.  This will complete the transfer of
        * a socket from another thread for use in the current thread.
        * The ThreadLogoff API must be called in the original thread
        * prior to calling ThreadLogon.
        */
        OSCL_IMPORT_REF TPVSocketEvent ThreadLogon(
            OsclSocketServ& aServ,
            OsclSocketObserver *aObserver
        );

        /**
         * Close a UDP socket.
         *  This is a synchronous method.
         *
         * Once it is closed a socket cannot be re-opened.
         * Sockets are automatically closed when they
         * are deleted.  This method may be used to see
         * any error code returned from the platform's
         * socket close call.
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code.
        */
        OSCL_IMPORT_REF int32 Close();

        /**
         * Bind a UDP socket to an address.
         *  This is a synchronous method.
         *
         * @param aAddress: Bind address.
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code.
        */
        OSCL_IMPORT_REF int32 Bind(OsclNetworkAddress& aAddress);

        /**
         * Bind a UDP socket to an address
         * and Join the multicast group.
         *  This is a synchronous method.
         *
         * @param aAddress: Bind address.
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code.
         * May throw an OsclErrNotSupported Exception
        */
        OSCL_IMPORT_REF int32 Join(OsclNetworkAddress& aAddress);

        /**
        * Join the multicast group.
        * @param aMReq: Multicast group information.
        * @return Returns: OsclErrNone for success, or a
        *   platform-specific error code. or
        *   PVSOCK_ERR_NOT_SUPPORTED, if underlying OS doesn't support joining
        *   multicast group
        *   PVSOCK_ERR_BAD_PARAM, if config io file is not configured in
        *   accordance with underlying OS
        *   PVSOCK_ERR_NOT_IMPLEMENTED, if this API is not implemented in OSCL
        *   for the underlying OS
        */
        OSCL_IMPORT_REF int32 JoinMulticastGroup(OsclIpMReq& aMReq);

        /**
         * Controls the number of intermediate systems through which a multicast
         * datagram can be forwarded.
         * @param aTTL:Specifies the time-to-live value for multicast datagrams
         * sent through this socket.
         * @return Returns: OsclErrNone for success, or a
         *   platform-specific error code. or
         *   PVSOCK_ERR_NOT_SUPPORTED, if underlying OS doesn't support joining
         *   multicast group
         *   PVSOCK_ERR_BAD_PARAM, if config io file is not configured in
         *   accordance with underlying OS
         *   PVSOCK_ERR_NOT_IMPLEMENTED, if this API is not implemented in OSCL
         *   for the underlying OS
        */
        OSCL_IMPORT_REF int32 SetMulticastTTL(int32 aTTL);

        /**
         * Allows the server to bind to an address which is in a TIME_WAIT state.
         * @return Returns: OsclErrNone for success, or a
         *   platform-specific error code. or
         *   PVSOCK_ERR_NOT_SUPPORTED, if underlying OS doesn't support joining
         *   multicast group
         *   PVSOCK_ERR_BAD_PARAM, if config io file is not configured in
         *   accordance with underlying OS
         *   PVSOCK_ERR_NOT_IMPLEMENTED, if this API is not implemented in OSCL
         *   for the underlying OS
        */
        OSCL_IMPORT_REF int32 SetOptionToReuseAddress();

        /**
         * Sets the Type of Service field of each outgoing IP datagram.
         * @param aTOS: Specifies the type of service requested.
         * @return Returns: OsclErrNone for success, or a
         *   platform-specific error code. or
         *   PVSOCK_ERR_NOT_SUPPORTED, if underlying OS doesn't support joining
         *   multicast group
         *   PVSOCK_ERR_BAD_PARAM, if config io file is not configured in
         *   accordance with underlying OS
         *   PVSOCK_ERR_NOT_IMPLEMENTED, if this API is not implemented in OSCL
         *   for the underlying OS
        */
        OSCL_IMPORT_REF int32 SetTOS(const OsclSocketTOS& aTOS);

        /**
         * Retrieves the peer address of the socket
         * @param aPeerName: This will store the peer address when API returns
         * successfully.
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code. or
         *   PVSOCK_ERR_NOT_IMPLEMENTED, if this API is not implemented in OSCL
         *   for the underlying OS
        */
        OSCL_IMPORT_REF int32 GetPeerName(OsclNetworkAddress& aPeerName);

        /**
         * Bind a UDP socket to an address.
         *  This is an asynchronous method.
         *
         * @param aAddress: Bind address.
         * @param aTimeoutMsec: Optional timeout.  Use a negative
         *    value for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent BindAsync(OsclNetworkAddress& aAddress
                , int32 aTimeoutMsec = (-1));

        /**
         * Cancel Bind
         *
         * This method will cancel any pending BindAsync operation
         * on the current socket, causing the BindAsync to complete
         * with error EPVSocketCancel.  If there is no pending
         * BindAsync operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelBind();

        /**
         * Retrieve the received data after a successful
         * RecvFrom operation.
         * This is a synchronous method.
         *
         * @param aLength: (output) number of bytes of data received.
         * @return Returns pointer to received data, or NULL
         *    if none.
        */
        OSCL_IMPORT_REF uint8 *GetRecvData(int32 *aLength);

        /**
         * Retrieve the sent data after a successful
         * SendTo operation.
         * This is a synchronous method.
         *
         * @param aLength: (output) number of bytes of data sent.
         * @return Returns pointer to sent data, or NULL
         *    if none.
        */
        OSCL_IMPORT_REF uint8 *GetSendData(int32 *aLength);

        /**
         * Send Data.
         * This is an asynchronous method.
         *
         * @param aPtr: Data to send.
         * @param aLen: Length of data to send.
         * @param aAddress: Destination address.
         * @param aTimeoutMsec: Timeout in milliseconds, or (-1)
         *   for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent SendTo(const uint8* aPtr, uint32 aLen,
                                              OsclNetworkAddress& aAddress,
                                              int32 aTimeoutMsec = -1);
        /**
         * Cancel SendTo
         *
         * This method will cancel any pending SendTo operation
         * on the current socket, causing the SendTo to complete
         * with error EPVSocketCancel.  If there is no pending
         * SendTo operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelSendTo();

        /**
         * Receive Data.
         * This is an asynchronous method.
         *
         * @param aPtr: Buffer to receive incoming data
         * @param aMaxLen: Length of buffer.
         * @param aAddress: (output) Source address.
         * @param aTimeoutMsec: Timeout in milliseconds, or (-1)
         *   for infinite wait.
         *
         * @param aMultiRecvLimit (optional input): Configures multiple
         *   packet receive mode.  As long as there are packets queued at
         *   the socket and at least aMultiRecvLimit bytes are available
         *   in the buffer, recvfrom operations will continue.
         *   A value of zero disabled multiple packet mode.
         *   The individual packet lengths can be retrieved in the
         *   aPacketLen parameter; and the individual packet
         *   source addresses can be retrieved in the aPacketSource parameter.
         * @param aPacketLen: (optional output) a vector of packet lengths,
         *     in case multiple packets were received.
         * @param aPacketSource: (optional output) a vector of source addresses,
         *     in case multiple packets were received.
         *
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent RecvFrom(uint8* aPtr, uint32 aMaxLen,
                                                OsclNetworkAddress& aAddress,
                                                int32 aTimeoutMsec = -1,
                                                uint32 aMultiRecvLimit = 0,
                                                Oscl_Vector<uint32, OsclMemAllocator>* aPacketLen = NULL,
                                                Oscl_Vector<OsclNetworkAddress, OsclMemAllocator>* aPacketSource = NULL);
        /**
         * Cancel RecvFrom
         *
         * This method will cancel any pending RecvFrom operation
         * on the current socket, causing the RecvFrom to complete
         * with error EPVSocketCancel.  If there is no pending
         * RecvFrom operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelRecvFrom();

        /**
         * Set the buffer size of the socket
         * This is a synchronous method.
         * @param size: buffer size
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code.
         * May throw an OsclErrNotSupported Exception.
        */

        OSCL_IMPORT_REF int32 SetRecvBufferSize(uint32 size);

    private:
        OsclUDPSocket(Oscl_DefAlloc &alloc): iUDPSocket(NULL), iAlloc(alloc) {}
        OsclUDPSocketI *iUDPSocket;
        Oscl_DefAlloc &iAlloc;
};

class OsclTCPSocketI;

/**
* The TCP Socket class
*/

class OsclTCPSocket : public HeapBase
{
    public:
        /**
         * Create a TCP Socket.
         * May leave if failure.
         *
         * @param alloc: Memory allocator.
         * @param aServ: Socket server.  Must be connected.
         * @param aObserver: Socket observer.
         * @param aId: Socket ID.  The caller must assign an ID
         *   to each socket.  The ID is used to identify the socket
         *   in observer callbacks.
         * @return Returns pointer to socket.
        */
        OSCL_IMPORT_REF static OsclTCPSocket *NewL(Oscl_DefAlloc &alloc,
                OsclSocketServ& aServ,
                OsclSocketObserver *aObserver,
                uint32 aId);

        /**
         * Destructor.  The object must be deleted using
         * the same allocator used in the NewL call.
         */
        OSCL_IMPORT_REF ~OsclTCPSocket();

        /**
         * Thread logoff routine.  This will prepare for transfer and
         * use of the socket in another thread.
         * All socket requests must be complete prior to calling this
         * routine.  If any requests are still active, it will return
         * EPVSocketFailure;
         */
        OSCL_IMPORT_REF TPVSocketEvent ThreadLogoff();
        /**
         * Thread logon routine.  This will complete the transfer of
         * a socket from another thread for use in the current thread.
         * The ThreadLogoff API must be called in the original thread
         * prior to calling ThreadLogon.
         */
        OSCL_IMPORT_REF TPVSocketEvent ThreadLogon(
            OsclSocketServ& aServ,
            OsclSocketObserver *aObserver
        );

        /**
         * Close a TCP socket.
         *  This is a synchronous method.
         *
         * Once it is closed a socket cannot be re-opened.
         * Sockets are automatically closed when they
         * are deleted.  This method may be used to see
         * any error code returned from the platform's
         * socket close call.
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code.
        */
        OSCL_IMPORT_REF int32 Close();

        /**
         * Bind a TCP socket to an address.
         *  This is a synchronous method.
         *
         * @param aAddress: Bind address.
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code.
        */
        OSCL_IMPORT_REF int32 Bind(OsclNetworkAddress& aAddress);

        /**
         * Bind a TCP socket to an address.
         *  This is an asynchronous method.
         *
         * @param aAddress: Bind address.
         * @param aTimeoutMsec: Optional timeout.  Use a negative
         *    value for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent BindAsync(OsclNetworkAddress& aAddress
                , int32 aTimeoutMsec = (-1));

        /**
         * Cancel Bind
         *
         * This method will cancel any pending BindAsync operation
         * on the current socket, causing the BindAsync to complete
         * with error EPVSocketCancel.  If there is no pending
         * BindAsync operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelBind();

        /**
         * Allows the server to bind to an address which is in a TIME_WAIT state.
         * @return Returns: OsclErrNone for success, or a
         *   platform-specific error code. or
         *   PVSOCK_ERR_NOT_SUPPORTED, if underlying OS doesn't support joining
         *   multicast group
         *   PVSOCK_ERR_BAD_PARAM, if config io file is not configured in
         *   accordance with underlying OS
         *   PVSOCK_ERR_NOT_IMPLEMENTED, if this API is not implemented in OSCL
         *   for the underlying OS
        */
        OSCL_IMPORT_REF int32 SetOptionToReuseAddress();

        /**
         * Sets the Type of Service field of each outgoing IP datagram.
         * @param aTOS: Specifies the type of service requested.
         * @return Returns: OsclErrNone for success, or a
         *   platform-specific error code. or
         *   PVSOCK_ERR_NOT_SUPPORTED, if underlying OS doesn't support joining
         *   multicast group
         *   PVSOCK_ERR_BAD_PARAM, if config io file is not configured in
         *   accordance with underlying OS
         *   PVSOCK_ERR_NOT_IMPLEMENTED, if this API is not implemented in OSCL
         *   for the underlying OS
        */
        OSCL_IMPORT_REF int32 SetTOS(const OsclSocketTOS& aTOS);

        /**
         * Retrieves the peer address of the socket
         * @param aPeerName: This will store the peer address when API returns
         * successfully.
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code. or
         *   PVSOCK_ERR_NOT_IMPLEMENTED, if this API is not implemented in OSCL
         *   for the underlying OS
        */
        OSCL_IMPORT_REF int32 GetPeerName(OsclNetworkAddress& aPeerName);
        /**
         * Listen.
         *  This is a synchronous method.
         *
         * @param aQueueSize: Queue size.
         * @return Returns OsclErrNone for success, or a
         *   platform-specific error code.
        */
        OSCL_IMPORT_REF int32 Listen(int32 aQueueSize);

        /**
         * ListenAsync
         *  This is an asynchronous method.
         *
         * @param aQueueSize: Queue size.
         * @param aTimeoutMsec: Optional timeout.  Use a negative
         *    value for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent ListenAsync(int32 aQueueSize, int32 aTimeoutMsec = (-1));

        /**
         * Cancel Async Listen
         *
         * This method will cancel any pending ListenAsync operation
         * on the current socket, causing the Listen to complete
         * with error EPVSocketCancel.  If there is no pending
         * Listen operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelListen();

        /**
         * Retrieve the accept socket after a successful
         * Accept operation.
         * This is a synchronous method.
         *
         * @param aId: Socket ID.  The caller must assign an ID
         *   to each socket.  The ID is used to identify the socket
         *   in observer callbacks.
         * @return Returns pointer to socket, or NULL if error.
         *   Note: The caller is reponsible for deleting any accepted
         *   socket that it retrieves.
        */
        OSCL_IMPORT_REF OsclTCPSocket *GetAcceptedSocketL(uint32 aId);

        /**
         * Retrieve the received data after a successful
         * Recv operation.
         * This is a synchronous method.
         *
         * @param aLength: (output) number of bytes of data received.
         * @return Returns pointer to received data, or NULL
         *    if none.
        */
        OSCL_IMPORT_REF uint8 *GetRecvData(int32 *aLength);

        /**
         * Retrieve the sent data after a successful
         * Send operation.
         * This is a synchronous method.
         *
         * @param aLength: (output) number of bytes of data sent.
         * @return Returns pointer to sent data, or NULL
         *    if none.
        */
        OSCL_IMPORT_REF uint8 *GetSendData(int32 *aLength);

        /**
         * Connect to an address.
         * This is an asynchronous method.
         *
         * @param aAddress: a network address.
         * @param aTimeoutMsec: Timeout in milliseconds, or (-1)
         *   for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent Connect(OsclNetworkAddress& aAddress,
                                               int32 aTimeoutMsec = -1);

        /**
         * Cancel Connect
         *
         * This method will cancel any pending Connect operation
         * on the current socket, causing the Connect to complete
         * with error EPVSocketCancel.  If there is no pending
         * Connect operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelConnect();

        /**
         * Shutdown a socket.
         * This is an asynchronous method.
         *
         * @param aHow: type of shutdown
         * @param aTimeoutMsec: Timeout in milliseconds, or (-1)
         *   for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent Shutdown(TPVSocketShutdown aHow,
                                                int32 aTimeoutMsec = -1);

        /**
         * Cancel Shutdown
         *
         * This method will cancel any pending Shutdown operation
         * on the current socket, causing the Shutdown to complete
         * with error EPVSocketCancel.  If there is no pending
         * Shutdown operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelShutdown();

        /**
         * Accept incoming connections.
         * This is an asynchronous method.
         *
         * @param aTimeoutMsec: Timeout in milliseconds, or (-1)
         *   for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent Accept(int32 aTimeout = -1);

        /**
         * Cancel Accept
         *
         * This method will cancel any pending Accept operation
         * on the current socket, causing the Accept to complete
         * with error EPVSocketCancel.  If there is no pending
         * Accept operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelAccept();

        /**
         * Send Data.
         * This is an asynchronous method.
         *
         * @param aPtr: Data to send.
         * @param aLen: Length of data to send.
         * @param aTimeoutMsec: Timeout in milliseconds, or (-1)
         *   for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent Send(const uint8* aPtr, uint32 aLen,
                                            int32 aTimeoutMsec = -1);

        /**
         * Cancel Send
         *
         * This method will cancel any pending Send operation
         * on the current socket, causing the Send to complete
         * with error EPVSocketCancel.  If there is no pending
         * Send operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelSend();

        /**
         * Receive Data.
         * This is an asynchronous method.
         *
         * @param aPtr: Buffer for received data.
         * @param aMaxLen: Length of buffer.
         * @param aTimeoutMsec: Timeout in milliseconds, or (-1)
         *   for infinite wait.
         * @return Will return EPVSocketPending if successful.
         *   When the operation is complete, a callback to the
         *   observer will occur with the completion status.
         *   If the operation cannot be initiated, the call will
         *   return EPVSocketFailure and there will be no callback.
        */
        OSCL_IMPORT_REF TPVSocketEvent Recv(uint8* aPtr, uint32 aMaxLen,
                                            int32 aTimeoutMsec = -1);

        /**
         * Cancel Recv
         *
         * This method will cancel any pending Recv operation
         * on the current socket, causing the Recv to complete
         * with error EPVSocketCancel.  If there is no pending
         * Recv operation, this method will have no effect.
        */
        OSCL_IMPORT_REF void CancelRecv();

    private:
        static OsclTCPSocket *NewL(Oscl_DefAlloc &alloc, OsclTCPSocketI* aSocket);

    private:
        OsclTCPSocket(Oscl_DefAlloc &alloc): iTCPSocket(NULL), iAlloc(alloc) {}
        OsclTCPSocketI *iTCPSocket;
        Oscl_DefAlloc &iAlloc;
};

#endif


