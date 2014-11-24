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



/** \file oscl_scheduler_threadcontext.h
    \brief Thread context functions needed by oscl scheduler.
*/

#ifndef OSCL_SCHEDULER_THREAD_CONTEXT_H_INCLUDED
#define OSCL_SCHEDULER_THREAD_CONTEXT_H_INCLUDED

#ifndef OSCL_AOSTATUS_H_INCLUDED
#include "oscl_aostatus.h"
#endif

#ifndef OSCL_DOUBLE_LIST_H_INCLUDED
#include "oscl_double_list.h"
#endif

#ifndef OSCL_MUTEX_H_INCLUDED
#include "oscl_mutex.h"
#endif

/**
//Thread context type
*/
enum TPVThreadContext
{
    EPVThreadContext_InThread //context is in-thread
    , EPVThreadContext_OsclThread //some other thread that has Oscl initialized
    , EPVThreadContext_NonOsclThread //some thread that does not have Oscl initialized
    , EPVThreadContext_Undetermined
};

/**
//
// Thread context class.
//
*/
class OsclExecSchedulerCommonBase;
class PVActiveBase;
class OsclBrewThreadYield;
class PVThreadContext
{
    public:

        OSCL_IMPORT_REF PVThreadContext();
        OSCL_IMPORT_REF ~PVThreadContext();
        /**
        //compare caller's thread context to this one.
        */
        OSCL_IMPORT_REF bool IsSameThreadContext();

        /**
        //static routine to get a unique thread ID for
        //caller's thread context.
        */
        OSCL_IMPORT_REF static uint32 Id();

        /**
        //enter and exit thread context.
        */
        OSCL_IMPORT_REF void EnterThreadContext();
        OSCL_IMPORT_REF void ExitThreadContext();

        /**
        // a static utility to tell whether the calling
        // thread has any scheduler-- either Oscl scheduler
        // or native scheduler.
        */
        OSCL_IMPORT_REF static bool ThreadHasScheduler();

    private:

        /**
        //leave if caller's thread doesn't match
        //given thread.
        */
        static void LeaveIfWrongThread(PVThreadContext &a);


        bool iOpen;

        /**
        //complete & cancel requests for this thread context.
        */
        void PendComplete(PVActiveBase*, int32 aReason, TPVThreadContext aCallingContext);


        OsclExecSchedulerCommonBase *iScheduler;

        /**
        //the thread ID is OS-specific.
        */
        TOsclThreadId iThreadId;

        friend class PVActiveBase;
        friend class OsclActiveObject;
        friend class OsclTimerObject;
        friend class OsclExecScheduler;
        friend class OsclCoeActiveScheduler;
        friend class OsclExecSchedulerCommonBase;
        friend class OsclExecSchedulerBase;
        friend class OsclCoeActiveSchedulerBase;
};




#endif //


/*! @} */
