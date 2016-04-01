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



/** \file pvscheduler.h
    \brief Main include file for Oscl Scheduler
*/


#ifndef OSCL_SCHEDULER_H_INCLUDED
#define OSCL_SCHEDULER_H_INCLUDED

#ifndef OSCL_SCHEDULER_TYPES_H_INCLUDED
#include "oscl_scheduler_types.h"
#endif

#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif

#ifndef OSCL_SCHEDULER_THREADCONTEXT_H_INCLUDED
#include "oscl_scheduler_threadcontext.h"
#endif

#ifndef OSCL_MUTEX_H_INCLUDED
#include "oscl_mutex.h"
#endif

#ifndef OSCL_SEMAPHORE_H_INCLUDED
#include "oscl_semaphore.h"
#endif

#ifndef OSCL_SCHEDULER_READYQ_H_INCLUDED
#include "oscl_scheduler_readyq.h"
#endif

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

class Oscl_DefAlloc;
class OsclCoeActiveScheduler;


/**
 * Per-thread scheduler initialization and cleanup.
 */
class OsclScheduler
{
    public:
        /**
         * This routine creates and installs a scheduler in
         * the calling thread.
         * @param name: (input param) scheduler name.
         * @param alloc: (input param) optional allocator to use for
         *   the internal implementation.
         * @param nreserve: (input param) optional value for ready queue
         *   reserve size.
         */
        OSCL_IMPORT_REF static void Init(const char *name, Oscl_DefAlloc *alloc = NULL, int nreserve = 20);


        /**
         * This routine uninstalls and destroys Oscl scheduler
         * for the calling thread.
         */
        OSCL_IMPORT_REF static void Cleanup();
};


/**
 * PV Scheduler class
 */

#define PVSCHEDNAMELEN 30

class PVSchedulerStopper;
class PVJump;
class PVLogger;

/**
 * OsclSchedulerObserver is an observer class for use when running scheduler
 * in non-blocking mode.
 * The scheduler observer can register for callbacks so it
 * will be notified when it is necessary to run scheduler again.
 * Note: non-blocking mode and scheduler callbacks are not supported on Symbian.
 */
class OsclSchedulerObserver
{
    public:
        /**
         * OsclSchedulerTimerCallback is called when the front of the timer queue
         * is updated.  This means the minimum delay has changed and scheduler needs
         * to be run again after aDelayMsec.  Calling context is in-thread.
         *
         * The current observer is cleared before making the callback, so the
         * observer must call RegisterForCallback again if it wants further
         * notifications.
         */
        virtual void OsclSchedulerTimerCallback(OsclAny* aContext, uint32 aDelayMsec) = 0;

        /**
         * OsclSchedulerReadyCallback is called when the ready queue is updated, meaning
         * an AO is ready to run.  Scheduler needs to be run ASAP.
         * Calling context may be any thread, so be careful!
         *
         * The current observer is cleared before making the callback, so the
         * observer must call RegisterForCallback again if it wants further
         * notifications.
         */
        virtual void OsclSchedulerReadyCallback(OsclAny* aContext) = 0;

        virtual ~OsclSchedulerObserver() {}
};


/*
** A common base class for variations of OsclExecScheduler
**/

class OsclExecSchedulerCommonBase
{
    public:
        /**
         * Start scheduling.  This call blocks until scheduler
         * is stopped or an error occurs.
         * @param sem: optional startup semaphore.  If provied,
         *    the scheduler will signal this semaphore when
         *    the startup has progressed to the point that it's
         *    safe to call StopScheduler or SuspendScheduler
         *    from another thread.
         */
        OSCL_IMPORT_REF void StartScheduler(OsclSemaphore *sem = NULL);

        /**
         * Stop scheduling.  This API may be called from the
         * scheduling thread or some other thread.
         */
        OSCL_IMPORT_REF void StopScheduler();

        /**
         * Suspend scheduling when the current Run is complete.
         * This API only applies to a blocking loop scheduler.
         */
        OSCL_IMPORT_REF void SuspendScheduler();

        /**
         * Resume scheduling immediately.
         * This API only applies to a blocking loop scheduler.
         */
        OSCL_IMPORT_REF void ResumeScheduler();

        /**
         * Start the OS native scheduling loop.  This is an alternative
         * to the PV scheduling loop.
         * To stop the native scheduler, use the StopScheduler API.
         */
        OSCL_IMPORT_REF void StartNativeScheduler();

        /**
         * Get name of scheduler for current thread.
         */
        OSCL_IMPORT_REF static OsclNameString<PVSCHEDNAMELEN> *GetName();

        /**
         * Get numeric ID of current thread.
         */
        OSCL_IMPORT_REF static uint32 GetId();


    protected:
        /*
        static OsclExecScheduler *NewL(const char *name,Oscl_DefAlloc *a,int nreserve);
        */
        virtual ~OsclExecSchedulerCommonBase();
        void InstallScheduler();
        void UninstallScheduler();
        friend class OsclScheduler;

    protected:
        void Error(int32 anError) const;

    protected:
        static OsclExecSchedulerCommonBase* GetScheduler();
        static OsclExecSchedulerCommonBase* SetScheduler(OsclExecSchedulerCommonBase*);
        friend class PVThreadContext;
        friend class OsclCoeActiveScheduler;

    protected:
        OsclExecSchedulerCommonBase(Oscl_DefAlloc*);
        virtual void ConstructL(const char *name, int);

        void BeginScheduling(bool blocking, bool native);
        void EndScheduling();

        void BlockingLoopL();

        bool iBlockingMode;//current blocking mode setting.
        bool iNativeMode;//current native mode setting.

        bool IsStarted();
        inline bool IsInstalled();

        //Stopper AO is a special PV AO used to implement
        //the StopScheduler and SuspendScheduler functions.
        PVSchedulerStopper *iStopper;
        OsclNoYieldMutex iStopperCrit;

        //Scheduler thread context
        PVThreadContext iThreadContext;

        //Scheduler name
        OsclNameString<PVSCHEDNAMELEN> iName;

        //For stop, suspend, and resume commands.
        bool iDoStop;
        bool iDoSuspend;
        bool iSuspended;
        OsclSemaphore iResumeSem;

        OsclErrorTrapImp* iErrorTrapImp;

        //Ready AO queue.  The AOs in this queue are ready to run.
        OsclReadyQ iReadyQ;

        //Pending Timer AO queue.  The AOs in this queue are OsclTimerObjects
        //and have a timeout pending.
        OsclTimerQ iExecTimerQ;
        void AddToExecTimerQ(PVActiveBase *active, uint32);

        void PendComplete(PVActiveBase *, int32 aReason, TPVThreadContext aContext);
        void RequestCanceled(PVActiveBase*);

        //Scheduling loop implementation.
        PVActiveBase* UpdateTimers(uint32 &aDelay);
        PVActiveBase* UpdateTimersMsec(uint32 &aDelay);
        PVActiveBase* WaitForReadyAO();
        void CallRunExec(PVActiveBase*);

        static const uint32 iTimeCompareThreshold;
        friend class OsclTimerCompare;
        friend class OsclReadyQ;

        friend class OsclError;



        uint32 iNumAOAdded;//for non-fair-scheduling sorts.

        //For statistics
#if (PV_SCHED_ENABLE_AO_STATS)
        OsclDoubleList<PVActiveStats> iPVStatQ;//queue of all PV ExecObjs.
        enum TOtherExecStats
        {
            EOtherExecStats_WaitTime //scheduler wait time
            , EOtherExecStats_QueueTime //scheduler queue operation time
            , EOtherExecStats_NativeOS //time in symbian native ExecObjs, not added by PV code (symbian only)
            , EOtherExecStats_ReleaseTime //thread release time (Brew only)
            , EOtherExecStats_Last
        };
        PVActiveStats* iOtherExecStats[EOtherExecStats_Last];//placeholder for stats of all ExecObjs.
        void ConstructStatQ();
        void BeginStats();
        void EndStats();
        static void ShowStats(PVActiveStats *active);
        static void ShowSummaryStats(PVActiveStats *active, PVLogger*, int64, int64&, float&);
        void CleanupStatQ();
        uint8* iTotalTicksTemp;
        int64 iGrandTotalTicks;
        float iTotalPercent;
        friend class PVActiveStats;
        uint32 iTime;
        int32 iDelta;
        PVActiveStats* iPVStats;
#endif //PV_SCHED_ENABLE_AO_STATS

        //Queue utils.
        PVActiveBase *FindPVBase(PVActiveBase *active, OsclDoubleList<PVActiveBase> &);
        void CleanupExecQ();
        void InitExecQ(int);

        PVLogger *iLogger;
        PVLogger* iDebugLogger;

#if(PV_SCHED_ENABLE_PERF_LOGGING)
        //for perf logging
        char* iLogPerfIndentStr;
        int32 iLogPerfIndentStrLen;
        uint32 iLogPerfTotal;
        void ResetLogPerf();
        void IncLogPerf(uint32);
#endif

        Oscl_DefAlloc *iAlloc;//allocator for scheduler.
        OsclMemAllocator iDefAlloc;//default allocator

        friend class OsclActiveObject;
        friend class OsclTimerObject;
        friend class PVActiveBase;
        friend class PVSchedulerStopper;


        friend class OsclExecScheduler;
};

class OsclErrorTrapImp;

class OsclExecScheduler: public OsclExecSchedulerCommonBase
        , public OsclExecSchedulerBase
{
    public:
        /**
         * Get currently installed scheduler for calling thread,
         * or NULL if no scheduler is installed.
         */
        OSCL_IMPORT_REF static OsclExecScheduler* Current();

        /**
         * Non-Blocking scheduler APIs.
         */

        /**
         * Run PV scheduler in non-blocking mode.  This call returns
         * when the desired number of Run calls have been made,
         * or when there are no more active objects that are ready
         * to run.
         *
         * @param aTargetCount: (input param) the maximum number of Run calls to make.
         * @param aReady: (output param) tells the number of active objects that
         *    are currently ready to run.
         * @param aDelayMsec: (output param) If no active objects are ready to
         *   run, but one or more active objects are waiting on timers, this
         *   parameter will tell the time interval from the current time until the
         *   first of the pending timer objects will be ready to run, in milliseconds.
         *
         * Note: On Symbian, non-blocking mode is not supported and this call will
         *  leave.
         *
         */
        OSCL_IMPORT_REF void RunSchedulerNonBlocking(int32 aTargetCount, int32 &aReady, uint32 &aDelayMsec);

        /**
         * Register for a notification when non-blocking scheduler needs to run
         * again.
         *
         * Note: On Symbian, non-blocking mode is not supported and this call will
         *  leave.
         */
        OSCL_IMPORT_REF void RegisterForCallback(OsclSchedulerObserver* aCallback, OsclAny* aCallbackContext);


    private:
        static OsclExecScheduler *NewL(const char *name, Oscl_DefAlloc *a, int nreserve);
        ~OsclExecScheduler();
        friend class OsclScheduler;


        OsclExecScheduler(Oscl_DefAlloc*);
        void ConstructL(const char *name, int);

};


/**
 * Scheduler stopper AO class, for internal use
 * by scheduler.
 */
class PVSchedulerStopper: public OsclActiveObject
{
    public:
        PVSchedulerStopper();
        ~PVSchedulerStopper();
    private:
        void Run();
};


#endif


/*! @} */
