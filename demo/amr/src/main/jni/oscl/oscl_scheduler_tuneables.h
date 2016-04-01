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



/** \file oscl_scheduler_tuneables.h
    \brief Tuneable settings for Oscl Scheduler
*/


#ifndef OSCL_SCHEDULER_TUNEABLES_H_INCLUDED
#define OSCL_SCHEDULER_TUNEABLES_H_INCLUDED

#include "osclconfig_proc.h"

//Enable/disable scheduler stats gathering for Run times.
//This data gathering is fairly expensive so should only be used
//for debug and profiling.
#ifndef PV_SCHED_ENABLE_AO_STATS
//defaults for cases where the flag is not defined in the osclconfig_proc.h
#if defined( NDEBUG)||(OSCL_RELEASE_BUILD)
//for release builds
#define PV_SCHED_ENABLE_AO_STATS 0
#else
//for debug builds
#define PV_SCHED_ENABLE_AO_STATS 1
#endif
#endif

//Enable this to track time spent in scheduling loop in addition to Run times.
//This data gathering is fairly expensive so should only be used
//for debug and profiling.
#ifndef PV_SCHED_ENABLE_LOOP_STATS
//defaults for cases where the flag is not defined in the osclconfig_proc.h
#if defined( NDEBUG)||(OSCL_RELEASE_BUILD)
//for release builds
#define PV_SCHED_ENABLE_LOOP_STATS 0
#else
//for debug builds
#define PV_SCHED_ENABLE_LOOP_STATS 0
#endif
#endif

//Note: loop stats requires PV_SCHED_ENABLE_AO_STATS
#if(PV_SCHED_ENABLE_LOOP_STATS) && !(PV_SCHED_ENABLE_AO_STATS)
#error Invalid Config!
#endif

//Enable this to do detailed logging for all Run calls.
//This logging is very expensive so should only be used
//for debug and profiling.
#ifndef PV_SCHED_ENABLE_PERF_LOGGING
//defaults for cases where the flag is not defined in the osclconfig_proc.h
#if defined( NDEBUG)||(OSCL_RELEASE_BUILD)
//for release builds
#define PV_SCHED_ENABLE_PERF_LOGGING 0
#else
//for debug builds
#define PV_SCHED_ENABLE_PERF_LOGGING 1
#endif
#endif

//Note: Perf logging requires PV_SCHED_ENABLE_AO_STATS
#if(PV_SCHED_ENABLE_PERF_LOGGING) && !(PV_SCHED_ENABLE_AO_STATS)
#error Invalid Config!
#endif

//Enable/disable thread context checking here. Context checks are important during
//development but may be costly depending on the OS thread ID retrieval efficiency.
#if defined( NDEBUG)||(OSCL_RELEASE_BUILD)
//for release builds
#define PV_SCHED_ENABLE_THREAD_CONTEXT_CHECKS 0
#else
//for debug builds
#define PV_SCHED_ENABLE_THREAD_CONTEXT_CHECKS 1
#endif

//Set LOG_Q to 1 to enable extensive Queue logging for non-symbian scheduler.
//the code will log the entire queue every time any AO is added or removed,
//for both timer queue and ready queue.  It is expensive so should be off in
//production code.
#define PV_SCHED_LOG_Q 0

//Set CHECK_Q to enable Q integrity checks.  Should be off in production code.
#define PV_SCHED_CHECK_Q 0

//This allows switching between "fair scheduling" and Symbian native scheduler
//behavior in the non-symbian scheduler implementation.
//We always use fair scheduling, but for testing it can be helpful to
//swap in the symbian native behavior.
#define PV_SCHED_FAIR_SCHEDULING 1

//OSCL_PERF_SUMMARY_LOGGING is a master switch to configure scheduler
//for full performance data gathering with minimal summary logging at
//the end.  The data gathering is fairly expensive so should only be
//used for profiling tests.  The logging itself is fairly nonintrusive
//since it only happens at scheduler exit.
//------------------------------------
// logger node: "OsclSchedulerPerfStats"
// instrumentation level: PVLOGMSG_INST_PROF
// logger level: PVLOGMSG_INFO
//------------------------------------
#ifndef OSCL_PERF_SUMMARY_LOGGING
//defaults for cases where the flag is not defined in the osclconfig_proc.h
#if defined( NDEBUG)||(OSCL_RELEASE_BUILD)
//release builds
#define OSCL_PERF_SUMMARY_LOGGING 0
#else
//other debug builds
#define OSCL_PERF_SUMMARY_LOGGING 0
#endif
#endif

//Settings for minimal stats logging
#if(OSCL_PERF_SUMMARY_LOGGING)
//enable AO stats
#undef PV_SCHED_ENABLE_AO_STATS
#define PV_SCHED_ENABLE_AO_STATS 1
//enable loop stats
#undef PV_SCHED_ENABLE_LOOP_STATS
#define PV_SCHED_ENABLE_LOOP_STATS 1
//disable perf logging because it skews the results
#undef PV_SCHED_ENABLE_PERF_LOGGING
#define PV_SCHED_ENABLE_PERF_LOGGING 0
#endif //OSCL_PERF_SUMMARY_LOGGING

#endif


/*! @} */
