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
#ifndef OSCL_TIMER_H_INCLUDED
#define OSCL_TIMER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#include "osclconfig_util.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_TICKCOUNT_H_INCLUDED
#include "oscl_tickcount.h"
#endif

#ifndef OSCL_RAND_H_INCLUDED
#include "oscl_rand.h"
#endif

#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif


/**
 * The observer class to receive timeout callbacks
 */
class OsclTimerObserver
{
    public:
        /**
         * This function will be called when the timer associated
         * with this observer is executed
         *
         * @param timerID The ID given at timer request.
         * @param timeoutInfo
         *                Any extra info given at timer request.
         */
        virtual void TimeoutOccurred(int32 timerID, int32 timeoutInfo) = 0;

        virtual ~OsclTimerObserver() {}
};

/**
 * A timer class for scheduling one or more timeout events.
 * The timeout event will trigger a callback to an observer
 * class.
 */
template<class Alloc>
class OsclTimer ;

class CallbackTimerObserver
{
    public:
        virtual void TimerBaseElapsed() = 0;
        virtual ~CallbackTimerObserver() {}
};

template<class Alloc>
class CallbackTimer: public OsclTimerObject
{
    public:
        CallbackTimer(CallbackTimerObserver& aContainer, const char *name, int32 aPriority = OsclActiveObject::EPriorityNominal)
                : OsclTimerObject(aPriority, name)
        {
            iContainer = &aContainer;
            AddToScheduler();
        }
        ~CallbackTimer()
        {
            RemoveFromScheduler();
        }
        void Run()
        {
            if (Status() == OSCL_REQUEST_ERR_NONE)
                iContainer->TimerBaseElapsed();
        }
    private:
        CallbackTimerObserver *iContainer;
};


template<class Alloc>
class OsclTimer : public CallbackTimerObserver
{
    public:

        typedef CallbackTimer<Alloc> callback_timer_type;

        /**
         * Constructor
         *
         * @param frequency The frequency of the timer in cycles/second.  A value of
         *                  1 means the timer will cycle in 1 second intervals.
         */
        OsclTimer(const char *name, uint32 frequency = 1, int32 priority = OsclActiveObject::EPriorityNominal);
        virtual ~OsclTimer();

        /**
         * Set the global observer.  Each timer can request a local
         * observer, which if set overrides the global observer.
         *
         * @param obs    observer object.
         */
        void SetObserver(OsclTimerObserver *obs)
        {
            iObserver = obs;
        }
        /**
         * Set the frequency of the timer in cycles/second.
         *
         * @param frequency A value of 1 means the timer will cycle in one second
         *                  intervals, 1000 means millisecond intervals, etc.
         */
        void SetFrequency(uint32 frequency);

        /**
         * Set the exact frequency of the timer in microsecond.
         *
         * @param frequency A value of 1 means the timer will cycle in one microsecond
         *                  intervals, 1000 means millisecond intervals, etc.
         */
        void SetExactFrequency(uint32 frequency);

        /**
         * Request a timer
         *
         * @param timerID used to identify the timer for cancellation.  This value
         *                will be returned as part of the timeout event.
         * @param timeoutInfo
         *                for user info.  Returned to the observer on a timeout event
         * @param cycles  the number of cycles to wait before a timeout event.  If
         *                the timer frequency is 1 and the cycles are set to 2, then
         *                the timeout event will occur in 2 seconds.
         * @param obs     a local observer object to be called on a timeout event.
         *                This observer overides the global observer if set.
         */
        void Request(int32 timerID, int32 timeoutInfo, int32 cycles, OsclTimerObserver *obs = 0, bool recurring = 0);
        /**
         * Cancel a timer
         *
         * @param timerID used to identify the timer to cancel.
         * @param timeoutInfo
         *                if not set to -1, this value will be used as additional
         *                matching criteria to cancel a timer.
         */
        void Cancel(int32 timerID, int32 timeoutInfo = -1);
        /**
         * Cancel all pending timers.
         */
        void Clear();

    private:
        //Note: the timer needs to be a new'd object so that
        //the CBase construction zeros the memory.
        callback_timer_type *iTimer;

        typedef struct  _TimerEntry
        {
            int32 iCounter ;
            int32 iTimerID ;
            int32 iParam ;
            OsclTimerObserver *iObserver;
            bool iRecurring;
            int32 iOrigCounter;
        } TimerEntry;

        typedef TimerEntry                    entry_type;
        typedef Oscl_Vector<entry_type*, Alloc> entries_type;
        typedef typename entries_type::iterator entries_type_iterator;

        OsclTimerObserver *iObserver;
        entries_type iEntries;
        entries_type iEntriesWaitingToAdd;
        entries_type iEntriesWaitingToCancel;
        Oscl_TAlloc<entry_type, Alloc> iEntryAllocator;

        bool iInCallback;

        uint32 iCyclePeriod;
        uint32 iTickCountPeriod;
        uint32 iExpectedTimeout;

    protected:
        void TimerBaseElapsed();
        friend class CallbackTimer<Alloc>;
};

template<class Alloc>
OsclTimer<Alloc>::OsclTimer(const char *name, uint32 frequency, int32 priority) :
        iObserver(0)
        , iInCallback(false)
        , iTickCountPeriod(0)
        , iExpectedTimeout(0)
{
    if (frequency == 0)
    {
        OSCL_LEAVE(OsclErrArgument);
    }
    else
    {
        //use the allocator with placement 'new'
        Alloc alloc;
        iTimer = OSCL_PLACEMENT_NEW(alloc.ALLOCATE(sizeof(CallbackTimer<Alloc>)), CallbackTimer<Alloc>(*this, name, priority));
        SetFrequency(frequency);
    }
}

template<class Alloc>
OsclTimer<Alloc>::~OsclTimer()
{
    // Make sure we're cancelled
    if (iTimer)
        iTimer->Cancel();
    if (iTimer)
    {
        iTimer->OSCL_TEMPLATED_DESTRUCTOR_CALL(callback_timer_type, CallbackTimer);
        Alloc alloc;
        alloc.deallocate(iTimer);
    }
    iTimer = NULL;

    for (entries_type_iterator it = iEntries.begin(); it != iEntries.end(); it++)
    {
        iEntryAllocator.deallocate(*it);
    }
}

template<class Alloc>
void OsclTimer<Alloc>::SetFrequency(uint32 frequency)
{
    // timer takes microseconds
    iCyclePeriod = 1000000 / frequency;
    // get tick count period
    iTickCountPeriod = OsclTickCount::TickCountPeriod();
}

template<class Alloc>
void OsclTimer<Alloc>::SetExactFrequency(uint32 frequency)
{
    // timer takes microseconds
    iCyclePeriod = frequency;
    // get tick count period
    iTickCountPeriod = OsclTickCount::TickCountPeriod();
}

// Request a timer
template<class Alloc>
void OsclTimer<Alloc>::Request(int32 timerID, int32 param, int32 cycles, OsclTimerObserver *obs, bool recurring)
{

    // add to list of timers
    entry_type *entry = iEntryAllocator.ALLOCATE(1);
    entry->iTimerID = timerID;
    entry->iParam = param;
    entry->iCounter = cycles;
    entry->iObserver = obs;
    entry->iRecurring = recurring;
    entry->iOrigCounter = entry->iCounter;

    // if the request is called inside of a callback, then we must add it later
    if (iInCallback)
    {
        iEntriesWaitingToAdd.push_back(entry);
        return;
    }

    iEntries.push_back(entry);

    if (iTimer)
    {
        iTimer->RunIfNotReady(iCyclePeriod);
    }

    if (iExpectedTimeout == 0)
    {
        iExpectedTimeout = (OsclTickCount::TickCount() * iTickCountPeriod) + iCyclePeriod;
    }
}

// Cancel a timer
template<class Alloc>
void OsclTimer<Alloc>::Cancel(int32 timerID, int32 param)
{

    if (iInCallback)
    {
        // add to list of timers
        entry_type *entry = iEntryAllocator.ALLOCATE(1);
        entry->iTimerID = timerID;
        entry->iParam = param;

        iEntriesWaitingToCancel.push_back(entry);
        return;
    }

    // remove from list of timers
    for (entries_type_iterator it = iEntries.begin(); it != iEntries.end(); it++)
    {
        if ((*it)->iTimerID == timerID)
        {
            // make sure the param matches unless it is not specified (-1)
            if ((*it)->iParam == param || param == -1)
            {
                iEntryAllocator.deallocate(*it);
                iEntries.erase(it);
                return;
            }
        }
    }
}

// Clear all waiting timers
template<class Alloc>
void OsclTimer<Alloc>::Clear()
{
    for (entries_type_iterator it = iEntries.begin(); it != iEntries.end(); it++)
    {
        iEntryAllocator.deallocate(*it);
    }
    iEntries.clear();
}

template<class Alloc>
void OsclTimer<Alloc>::TimerBaseElapsed()
{
    uint8 expiredFound = 0;

    {
        // call all whose timers have expired
        for (entries_type_iterator it = iEntries.begin(); it != iEntries.end(); it++)
        {
            entry_type *entry = (*it);
            if (--(entry->iCounter) <= 0)
            {
                if (!entry->iRecurring) expiredFound = 1;
                if (entry->iRecurring) entry->iCounter = entry->iOrigCounter;

                // use local observer if it exists, otherwise use global observer
                OsclTimerObserver *obs = (entry->iObserver ? entry->iObserver : iObserver);
                if (obs)
                {
                    iInCallback = true;
                    obs->TimeoutOccurred(entry->iTimerID, entry->iParam);
                    iInCallback = false;
                }
            }
        }
    }

    // remove from list all whose timers have expired
    while (expiredFound)
    {
        expiredFound = 0;
        for (entries_type_iterator it = iEntries.begin(); it != iEntries.end(); it++)
        {
            entry_type *entry = (*it);
            if (entry->iCounter <= 0)
            {
                expiredFound = 1;
                iEntryAllocator.deallocate(entry);
                iEntries.erase(it);
                break;
            }
        }
    }

    {
        // if any timers were cancelled in the callback, process them now
        for (entries_type_iterator it = iEntriesWaitingToCancel.begin(); it != iEntriesWaitingToCancel.end(); it++)
        {
            entry_type *entry = (*it);
            Cancel(entry->iTimerID, entry->iParam);
            iEntryAllocator.deallocate(entry);
        }
        iEntriesWaitingToCancel.clear();
    }

    {
        // if any timers were requested in the callback, process them now
        for (entries_type_iterator it = iEntriesWaitingToAdd.begin(); it != iEntriesWaitingToAdd.end(); it++)
        {
            entry_type *entry = (*it);
            Request(entry->iTimerID, entry->iParam, entry->iCounter, entry->iObserver);
            iEntryAllocator.deallocate(entry);
        }
        iEntriesWaitingToAdd.clear();
    }

    if (!iEntries.empty())
    {
        // adjust for the jitter
        uint32 time = OsclTickCount::TickCount() * iTickCountPeriod;
        int32 jitter = time - iExpectedTimeout;
        int32 waitperiod = iCyclePeriod - jitter;

        // currently there is some problem on the phone if we send
        // in real-time rather than with a slower (growing delay) H.223 mux output
        // if jitter is too large in either direction, start over
        if ((uint)OSCL_ABS(jitter) > iCyclePeriod)
        {
            iExpectedTimeout = time;
        }
        else
        {
            iExpectedTimeout += iCyclePeriod;
        }

        waitperiod = OSCL_MAX(waitperiod, 0);

        if (iTimer)
        {
            iTimer->RunIfNotReady(waitperiod);
        }
    }
    else
    {
        iExpectedTimeout = 0;
    }
}



#endif
