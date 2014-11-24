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





#ifndef OSCL_PROCSTATUS_H_INCLUDED

#define OSCL_PROCSTATUS_H_INCLUDED



/**

 * Class OsclProcStatus

 */

class OsclProcStatus
{





    public:



// Enumerated Error values



        /**

         * List of enums which contain error codes

         */

        enum eOsclProcError
        {
            SUCCESS_ERROR = 0,                  // Function returned succesfully
            OTHER_ERROR,                        // Function returned some error
            TOO_MANY_THREADS_ERROR,             // Cannot create another thread
            BAD_THREADID_ADDR_ERROR,            // The address for the thread id is not correct
            MAX_THRDS_REACHED_ERROR,            // No more threads can be created in the system
            INVALID_THREAD_ID_ERROR,            // Invalid thread identifier
            NOT_ENOUGH_MEMORY_ERROR,            // Not enough storage is available to process this command
            OUTOFMEMORY_ERROR,                  // Not enough storage is available to complete this operation
            NOT_ENOUGH_RESOURCES_ERROR,         // Not Enough resources available
            THREAD_1_INACTIVE_ERROR,            // The signal handler cannot be set
            ALREADY_SUSPENDED_ERROR,            // Thread is already suspended
            NOT_SUSPENDED_ERROR,                // Thread cannot be suspended
            INVALID_THREAD_ERROR,               // Referring to invalid thread
            INVALID_PARAM_ERROR,                // Invalid parameter passed in the API
            NO_PERMISSION_ERROR,                // Operation not permitted
            INVALID_PRIORITY_ERROR,             // Priority is not a valid OSCL priority
            PSHARED_NOT_ZERO_ERROR,             // Trying to create global semaphore
            EXCEED_MAX_COUNT_VARIABLE_ERROR,    // Exceeding maximum possible value for semaphore count
            THREAD_BLOCK_ERROR,                 // Thread is blocked
            EXCEED_MAX_SEM_COUNT_ERROR,         // Out of range of semaphore count
            INVALID_HANDLE_ERROR,               // Invalid handle
            INVALID_OPERATION_ERROR,            // Invalid operation
            INVALID_FUNCTION_ERROR,
            INVALID_ACCESS_ERROR,
            INVALID_ARGUMENT_ERROR,
            SYSTEM_RESOURCES_UNAVAILABLE_ERROR,
            INVALID_POINTER_ERROR,
            RELOCK_MUTEX_ERROR,
            THREAD_NOT_OWN_MUTEX_ERROR,
            MUTEX_LOCKED_ERROR,
            WAIT_ABANDONED_ERROR,//thread terminated while waiting
            WAIT_TIMEOUT_ERROR, //timed wait expired without acquiring
            SEM_NOT_SIGNALED_ERROR, //trywait returned without acquiring
            PSHARED_ATTRIBUTE_SETTING_ERROR,   // Error at setting pthread attributes
            NOT_IMPLEMENTED //function not implemented on this platform.
        } ;



};



#endif

