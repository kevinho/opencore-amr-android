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
/*! \addtogroup osclproc OSCL Proc
 *
 * @{
 */


/** \file oscl_scheduler_aobase.h
    \brief Oscl Scheduler internal active object classes.
*/

/** This file defines the PVActiveBase class, which is a common base for
  All PV ExecObjs on all platforms.
 */

#ifndef OSCL_SCHEDULER_AOBASE_H_INCLUDED
#define OSCL_SCHEDULER_AOBASE_H_INCLUDED

#ifndef OSCL_SCHEDULER_TYPES_H_INCLUDED
#include "oscl_scheduler_types.h"
#endif

#ifndef OSCL_NAMESTRING_H_INCLUDED
#include "oscl_namestring.h"
#endif

#ifndef OSCL_SCHEDULER_THREAD_CONTEXT_H_INCLUDED
#include "oscl_scheduler_threadcontext.h"
#endif

#ifndef OSCL_SCHEDULER_READYQ_H_INCLUDED
#include "oscl_scheduler_readyq.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

/*
 * Macro for zeroizing the memory of a newly created object
 *
 * @param ptr   pointer to the newly created object
 *
 * @param size  size of the memory
 */

#define OSCL_ZEROIZE(ptr, size) oscl_memset(ptr, 0, size)

/*
** Max length for scheduler and AO names.
*/
#define PVEXECNAMELEN 30

#if (PV_SCHED_ENABLE_AO_STATS)
/** PV AO statistics
*/
class PVActiveStats
{
    private:
        //should be constructed only when AO is added to scheduler.
        PVActiveStats(OsclExecSchedulerCommonBase* aScheduler, const char* aAOName, PVActiveBase* aActiveBase);
        ~PVActiveStats();

        void Combine(PVActiveStats&);

        //Queue link for scheduler iPVStatQ
        OsclDoubleLink iPVStatQLink;

        //this AO's scheduler
        OsclExecSchedulerCommonBase* iScheduler;
        //AO name
        OSCL_HeapString<OsclMemAllocator> iAOName;
        //pointer to original container object.
        PVActiveBase* iPVActiveBase;

        uint32 iNumRun;//how many Run calls
        uint32 iNumRunError;//how many RunError calls
        int32 iMaxTicksInRun;//max time in any one Run call

        uint32 iTotalTicksInRun;

        bool i64Valid;//use 64-bit stats instead of 32-bit
        int64 i64TotalTicksInRun;//avg number of clock ticks per Run

        //for internal computation-- percent of total time in this Run
        float iPercent;

        int32 iLeave;//last leave code from a Run call
        uint32 iNumCancel;//how many DoCancel calls.

        uint32 iNumInstances;//number of scheduler instances of this AO.
        int32 iPriority;//scheduler priority

        friend class PVActiveBase;
        friend class OsclExecScheduler;
        friend class OsclExecSchedulerCommonBase;
        friend class OsclActiveObject;
        friend class OsclTimerObject;
        friend class OsclReadyQ;
};
#endif //(PV_SCHED_ENABLE_AO_STATS)


/**
 * PV Scheduler internal AO base class.  Both OsclActiveObject
 * and OsclTimerObject derive from this class.  For Symbian, this
 * just container has the desired additions to the basic CTimer or OsclActiveObj
 * functionality.
 * For non-Symbian, this class contains the entire AO implementation.
 */
class PVActiveBase
{
    public:
        uint32 iAddedNum;

//iStatus, AddToScheduler(), RemoveFromScheduler etc are needed in PVSchedulerStopper (which is publically derived from OsclActiveObject and OsclActiveObject is publically derived from PVActiveBase )
//protected:

        PVActiveBase(const char name[], int32 pri);

        virtual ~PVActiveBase();

        /*
        ** Each AO has a name, to aid in statistics reporting and debugging.
        */
        OsclNameString<PVEXECNAMELEN> iName;

        /*
        ** Each AO knows its thread context, including a pointer to the scheduler
        ** that it is in.
        */
        PVThreadContext iThreadContext;

#if (PV_SCHED_ENABLE_AO_STATS)
        /*
        ** AO statistics
        */
        PVActiveStats *iPVActiveStats;
        friend class PVActiveStats;
#endif


        /*
        ** Non-Symbian AO implementation.
        */

        /*
        ** Queue link for scheduler iExecTimerQ or iReadyQ.
        */
        TReadyQueLink iPVReadyQLink;

        /*
        ** Return true if this AO is in any queue.
        */
        bool IsInAnyQ()
        {
            return(iPVReadyQLink.iIsIn != NULL);
        }

        /*
        ** The executing flag is set whenever a request is active (pending
        ** or complete).  The AO is de-activated by scheduler just before
        ** calling the Run.
        */
        bool iBusy;

        /**
        The request status associated with an asynchronous request.

        This is passed as a parameter to all asynchronous service providers.

        The active scheduler uses this to check whether the active object's request
        has completed.

        The function can use the completion code to judge the success or otherwise
        of the request.

        Request status contains one of the values
        OSCL_REQUEST_ERR_NONE: request completed with no error,
           or request is not active.
        OSCL_REQUEST_PENDING: request is active & pending
        OSCL_REQUEST_ERR_CANCEL: request was canceled before completion.
        or any user-defined value.
        */
        OsclAOStatus iStatus;

        /**
        * Virtual routine that gets called if the active object's
        * Run routine leaves.
        * @param aError: the leave code generated by the Run.
        * @return :returns OsclErrNone if the error was handled,
        *   or returns the input aError value if not handled.
        */
        virtual int32 RunError(int32 aError) = 0;

        /**
        Handles an active object's request completion event.

        A derived class must provide an implementation to handle the
        completed request. If appropriate, it may issue another request.

        The function is called by the active scheduler when a request
        completion event occurs, i.e. after the active scheduler's
        WaitForAnyRequest() function completes.

        Before calling this active object's Run() function, the active scheduler
        has:

        1. decided that this is the highest priority active object with
           a completed request

        2. marked this active object's request as complete (i.e. the request is no
           longer outstanding)

        Run() runs under a trap harness in the active scheduler. If it leaves,
        then the active scheduler calls ExecError() to handle the leave.

        Note that once the active scheduler's Start() function has been called,
        all user code is run under one of the program's active object's Run() or
        RunError() functions.
        */
        virtual void Run() = 0;

        /**
        Implements cancellation of an outstanding request.

        This function is called as part of the active object's Cancel().

        It must call the appropriate cancel function offered by the active object's
        asynchronous service provider. The asynchronous service provider's cancel
        is expected to act immediately.

        DoCancel() must not wait for event completion; this is handled by Cancel().
        */
        virtual void DoCancel() = 0;

        /*
        ** Common methods to support OsclActiveObject and OsclTimerObject APIs
        */
        void AddToScheduler();
        void RemoveFromScheduler();
        void Destroy();
        void Activate();
        OSCL_IMPORT_REF bool IsAdded()const;
        void Cancel();

        friend class OsclSchedulerCommonBase;
        friend class OsclActiveObject;
        friend class OsclTimerObject;
        friend class OsclReadyQ;
        friend class OsclReadyCompare;
        friend class OsclReadySetPosition;
        friend class OsclExecScheduler;

};

#endif




/*! @} */

