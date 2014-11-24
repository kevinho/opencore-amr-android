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

//                  OSCL_T H R E A D (T H R E A D  I M P L E M E N T A T I O N)

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \file oscl_thread.h .This file provides THREAD implementation that can be ported
to three OS LINUX, SYMBIAN, WIN32
*/


// Definition file for OSCL Threads
#ifndef OSCL_THREAD_H_INCLUDED
#define OSCL_THREAD_H_INCLUDED

#ifndef OSCLCONFIG_PROC_H_INCLUDED
#include "osclconfig_proc.h"
#endif

#ifndef OSCL_PROCSTATUS_H_INCLUDED
#include "oscl_procstatus.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


// Thread state on creation
enum OsclThread_State
{
    Start_on_creation
    , Suspend_on_creation
};

// Enumerated Priority Values
enum OsclThreadPriority
{
    ThreadPriorityLowest
    , ThreadPriorityLow
    , ThreadPriorityBelowNormal
    , ThreadPriorityNormal
    , ThreadPriorityAboveNormal
    , ThreadPriorityHighest
    , ThreadPriorityTimeCritical
};

//thread function pointer type.
typedef TOsclThreadFuncRet(OSCL_THREAD_DECL *TOsclThreadFuncPtr)(TOsclThreadFuncArg);

/**
 * Thread Class.  A subset of Thread APIs.
 * It implements platform independendent APIs for thread creation, exiting, suspend, resume, priority
 * and termination. With the use of proper defines it implements the basic thread festures.
 * It provides an opaque layer through which user doesn't need to worry about OS specific data.
 */
class OsclThread
{

    public:

        /**
         * Class constructor
         */
        OSCL_IMPORT_REF OsclThread();

        /**
         * Class destructor
         */
        OSCL_IMPORT_REF ~OsclThread();

        /**
         *  This routine will create a thread.  The thread may be
         *  launched immediately or may be created in a suspended
         *  state and launched with a Resume call.
         *
         * @param
         * func  =  Name of the thread Function
         * stack_size  =  Size of the thread stack.  If zero, then the
         *                platform-specific default stack size will be used.
         * argument = Argument to be passed to thread function
         * state = Enumeration which specifies the state of the thread on creation
         *         with values Running and Suspend.  Note: the Suspend option
         *         may not be available on all platforms.  If it is not supported,
         *         the Create call will return INVALID_PARAM_ERROR.
         * oIsJoinable = A boolean, which when set to true, creates a Joinable thread.
         *               The default value for this is false, which creates a
         *               Detached thread.
         *               Note 1: When a joinable thread is created, it is imperative
         *                       to call thread Terminate. Otherwise, it would cause
         *                       a memory leak.
         *               Note 2: This is currently available only for platforms that
         *                       have support for pthreads.
         * @return eOsclProcError
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Create(TOsclThreadFuncPtr func,
                int32 stack_size,
                TOsclThreadFuncArg argument,
                OsclThread_State state = Start_on_creation,
                bool oIsJoinable = false
                                                             );

        /**
         * Exit is a static function which is used to end the current thread. When called it
         * just ends the execution of the current thread.
         * @param
         * exitcode  =  Exitcode of the thread. This can be used by other threads to know the
         *              exit status of this thread.
         * @return None
         */
        OSCL_IMPORT_REF static void Exit(OsclAny* exitcode);

        /**
         * EnableKill is a static function which can
         *  be called by the thread routine in order to enable
         *  thread termination without waiting for cancellation
         *  points.
         *  EnableKill only applies to pthread implementations.
         *  For other implementations this function will do nothing.
         *
         * @return None
         */
        OSCL_IMPORT_REF static void EnableKill();

        /**
         * GetThreadPriority gets the priority of the thread. It takes reference of the input argument
         * and assigns priority to it from one of the already defined priorities.
         * @param
         * int16& refThreadPriority : Output Priority value
         * @return Error code
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError GetPriority(OsclThreadPriority& refThreadPriority);

        /**
         * SetThreadPriority sets the priority of the thread. It takes priority as the input argument
         * and assigns it to the thread referred.
         * @param
         * ePriorityLevel : Input Priority value
         * @return Error code
         * Note: this function may not be supported on all platforms, and
         * may return NOT_IMPLEMENTED.
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError SetPriority(OsclThreadPriority ePriority);

        /**
         * This API suspends the thread being referred. The thread can later be brought into execution
         * by calling OSCL_ResumeThread() on it.
         * @param None
         * @return Error code
         * Note: this function may not be supported on all platforms, and
         * may return NOT_IMPLEMENTED.
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Suspend();

        /**
         * ResumeThread resumes the suspended thread and brings it into execution.
         * @param None
         * @return Error code
         * Note: this function may not be supported on all platforms, and
         * may return NOT_IMPLEMENTED.
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Resume();

        /**
         * Terminate a thread other than the calling thread.
         *
         * Note: for pthread implementations, the Terminate call will
         *   block until the thread has terminated.  By default,
         *   threads will not terminate until a cancellation point
         *   is reached.  The EnableKill method may be used to override
         *   this default behavior and allow immediate termination.
         *
         * @param
         * exitcode  =  Exitcode of the thread.
         * @return Error code
         */
        OSCL_IMPORT_REF OsclProcStatus::eOsclProcError Terminate(OsclAny* exitcode);


        /**
         * Static routine to retrieve ID of calling thread.
         * @param Thread ID passed by the application
         * @return Error code
         */
        OSCL_IMPORT_REF static OsclProcStatus::eOsclProcError GetId(TOsclThreadId& refThreadId);

        /**
         * Static routine to compare whether two thread ID's are equal.
         * @param t1, t2: thread ID passed by the application
         * @return true if equal.
         */
        OSCL_IMPORT_REF static bool CompareId(TOsclThreadId &t1, TOsclThreadId &t2);

        /**
         * Suspend current thread execution for specified time.
         * @param msec, t2: sleep time in milliseconds.
         */
        OSCL_IMPORT_REF static void SleepMillisec(const int32 msec);

    private:

        /**
         * Helper Function
         * Map the Operating system errors to OSCL defined erros
         * @param
         * error : Input error as one of the OS errors
         * @return Error code ( User defined )
         */
        OsclProcStatus::eOsclProcError MapOsclError(int16 error);


        TOsclMutexObject mutex;
        TOsclConditionObject  condition;
        uint8 suspend;



        TOsclThreadObject ObjThread;
        bool bCreated;
        bool iJoined;
};

#endif

