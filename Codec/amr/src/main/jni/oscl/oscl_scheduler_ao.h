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


/** \file oscl_scheduler_ao.h
    \brief Oscl Scheduler user execution object classes.
*/


#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#define OSCL_SCHEDULER_AO_H_INCLUDED

#ifndef OSCL_SCHEDULER_TYPES_H_INCLUDED
#include "oscl_scheduler_types.h"
#endif

#ifndef OSCL_SCHEDULER_AOBASE_H_INCLUDED
#include "oscl_scheduler_aobase.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

/**
 * User base class for execution objects.
 * OsclActiveObject defines an execution object without any timer.
 * This AO can be used across threads, i.e. the request
 * can be activated in one thread and completed in another.
 */
class OsclActiveObject:
        public HeapBase,
        public PVActiveBase
{
    public:
        /**
         * Scheduling priorities.
         */
        enum OsclActivePriority
        {
            /**
            A low priority, useful for execution objects representing
            background processing.
            */
            EPriorityIdle = -100,
            /**
            A priority higher than EPriorityIdle but lower than EPriorityNominal.
            */
            EPriorityLow = -20,
            /**
            Most exec objects will have this priority.
            */
            EPriorityNominal = 0,
            /**
            A priority higher than EPriorityNominal; useful for execution objects
            handling user input.
            */
            EPriorityHigh = 10,
            /**
            A priority higher than EPriorityHighest.
            */
            EPriorityHighest = 20,
        };

        /**
         * Constructor.
         * @param aPriority (input param): scheduling priority
         * @param name (inpup param): optional name for this AO.
         */
        OSCL_IMPORT_REF OsclActiveObject(int32 aPriority, const char name[]);

        /**
         * Destructor.
         */
        OSCL_IMPORT_REF virtual ~OsclActiveObject();


        /**
         * Set object ready for this AO,
         * additionally sets the request status to OSCL_REQUEST_PENDING.
         * Will leave if the request is already readied,
         * or the execution object is not added to any scheduler,
         * or the calling thread context does not match
         * the scheduler thread.
         */
        OSCL_IMPORT_REF void SetBusy();

        /**
         * Return true if this AO is pending,
         * false otherwise.
         */
        OSCL_IMPORT_REF bool IsBusy() const;

        /**
         * Set request active for this AO and set the status to pending.
         * PendForExec is identical to SetActive, but it
         * additionally sets the request status to OSCL_REQUEST_PENDING.
         */
        OSCL_IMPORT_REF void PendForExec();

        /**
         * Complete the active request for the AO.
         * This API is thread-safe.
         * If the request is not pending, this call will leave.
         * @param aStatus: request completion status.
         */
        OSCL_IMPORT_REF void PendComplete(int32 aStatus);

        /**
         * Add this exec object to the current thread's scheduler.
         */
        OSCL_IMPORT_REF void AddToScheduler();

        /**
         * Remove this AO from its scheduler.
         * Will leave if the calling thread context does
         * not match the scheduling thread.
         * Cancels any readied request before removing.
         */
        OSCL_IMPORT_REF void RemoveFromScheduler();


        /**
         * Complete this AO's request immediately.
         * If the AO is already active, this will do nothing.
         * Will leave if the AO is not added to any scheduler,
         * or if the calling thread context does not match the
         * scheduling thread.
         */
        OSCL_IMPORT_REF void RunIfNotReady();

        /**
         * Cancel any pending request.
         * If the request is readied, this will call the DoCancel
         * routine, wait for the request to cancel, then set the
         * request idle.  The AO will not run.
         * If the request is not readied, it does nothing.
         * Request must be canceled from the same thread
         * in which it is scheduled.
         */
        OSCL_IMPORT_REF void Cancel();


        /**
        * Return scheduling priority of this exec object.
        */
        OSCL_IMPORT_REF int32 Priority() const;

        /**
        * Request status access
        */
        OSCL_IMPORT_REF int32 Status()const;
        OSCL_IMPORT_REF void SetStatus(int32);
        OSCL_IMPORT_REF OsclAOStatus& StatusRef();

    protected:
        /**
         * Cancel request handler.
         * This gets called by scheduler when the request
         * is cancelled.  The default routine will complete
         * the request.  If any additional action is needed,
         * the derived class may override this.  If the derived class
         * does override DoCancel, it must complete the request.
         */
        OSCL_IMPORT_REF virtual void DoCancel();

        /**
        * Run Error handler.
        * This gets called by scheduler when the Run routine leaves.
        * The default implementation simply returns the leave code.
        * If the derived class wants to handle errors from Run,
        * it may override this.  The RunError should return OsclErrNone
        * if it handles the error, otherwise it should return the
        * input error code.
        * @param aError: the leave code generated by the Run.
        */
        OSCL_IMPORT_REF virtual int32 RunError(int32 aError);

};

/**
 * User base class for execution objects.
 * OsclTimerObject defines an exec object with a timer.

 */
class OsclTimerObject: public HeapBase
        , public PVActiveBase
{
    public:
        /**
         * Constructor.
         * @param aPriority (input param): scheduling priority
         * @param name (input param): optional name for this AO.
         */
        OSCL_IMPORT_REF OsclTimerObject(int32 aPriority, const char name[]);

        /**
         * Destructor.
         */
        OSCL_IMPORT_REF virtual ~OsclTimerObject();

        /**
         * Add this AO to the current thread's scheduler.
         */
        OSCL_IMPORT_REF void AddToScheduler();


        /**
         * Remove this AO from its scheduler.
         * Will leave if the calling thread context does
         * not match the scheduling thread.
         * Cancels any pending request before removing.
         */
        OSCL_IMPORT_REF void RemoveFromScheduler();


        /**
        * 'After' sets the request ready, with request status
        * OSCL_REQUEST_STATUS_PENDING, and starts a timer.
        * When the timer expires, the request will complete with
        * status OSCL_REQUEST_ERR_NONE.
        * Must be called from the same thread in which the
        * active object is scheduled.
        * Will leave if the request is already readied, the object
        * is not added to any scheduler, or the calling thread
        * does not match the scheduling thread.
        * @param anInterval: timeout interval in microseconds.
        */
        OSCL_IMPORT_REF void After(int32 aDelayMicrosec);

        /**
         * Complete the request after a time interval.
         * RunIfNotReady is identical to After() except that it
         * first checks the request status, and if it is already
         * readied, it does nothing.
         *
         * @param aDelayMicrosec (input param): delay in microseconds.
         */
        OSCL_IMPORT_REF void RunIfNotReady(uint32 aDelayMicrosec = 0);

        /**
         * Set request ready for this AO.
         * Will leave if the request is already readied,
         * or the exec object is not added to any scheduler,
         * or the calling thread context does not match
         * the scheduler thread.
         */
        OSCL_IMPORT_REF void SetBusy();

        /**
         * Return true if this AO is active,
         * false otherwise.
         */
        OSCL_IMPORT_REF bool IsBusy() const;

        /**
         * Cancel any active request.
         * If the request is pending, this will call the DoCancel
         * routine, wait for the request to cancel, then set the
         * request idle.  The AO will not run.
         * If the request is not active, it does nothing.
         * Request must be canceled from the same thread
         * in which it is scheduled.
         */
        OSCL_IMPORT_REF void Cancel();

        /**
        * Return scheduling priority of this exec object.
        */
        OSCL_IMPORT_REF int32 Priority() const;
        /**
        * Request status access
        */
        OSCL_IMPORT_REF int32 Status()const;
        OSCL_IMPORT_REF void SetStatus(int32);
        OSCL_IMPORT_REF OsclAOStatus& StatusRef();

    protected:
        /**
         * Cancel request handler.
         * This gets called by scheduler when the request
         * is cancelled.  The default routine will cancel
         * the timer.  If any additional action is needed,
         * the derived class may override this.  If the
         * derived class does override this, it should explicitly
         * call OsclTimerObject::DoCancel in its own DoCancel
         * routine.
         */
        OSCL_IMPORT_REF virtual void DoCancel();

        /**
        * Run Leave handler.
        * This gets called by scheduler when the Run routine leaves.
        * The default implementation simply returns the leave code.
        * If the derived class wants to handle errors from Run,
        * it may override this.  The ExecError should return OsclErrNone
        * if it handles the error, otherwise it should return the
        * input error code.
        * @param aError: the leave code generated by the Run.
        */
        OSCL_IMPORT_REF virtual int32 RunError(int32 aError);
};


#endif

/*! @} */
