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

//                     O S C L _ E X C E P T I O N

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclerror OSCL Error
 *
 * @{
 */


/** \file oscl_exception.h
    \brief contains all the exception handling macros and classes
*/


#ifndef OSCL_EXCEPTION_H_INCLUDED
#define OSCL_EXCEPTION_H_INCLUDED

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef OSCL_ERROR_H_INCLUDED
#include "oscl_error.h"
#endif

#ifndef OSCL_ERROR_IMP_H_INCLUDED
#include "oscl_error_imp.h"
#endif


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//! oscl_exception.h contains all the exception handling macros and classes


//! This template class provides the base exception class that all exceptions derive from
/*!
   All PacketVideo exception classes will be derived from the OsclException class.
   Each derived class will have a static function where the leave code
   can be obtained. This avoids the issue of having static members in a DLL.
   The function needs to be static so it can be called without an instance
   of the class
*/
template <int LeaveCode> class OsclException
{
    public:
        OsclException() {}
        static int getLeaveCode()
        {
            return LeaveCode;
        };
};


//! Use this macro to cause a Leave. It terminates the execution of the current active function
/*!
    It also tries to cleanup the items on the cleanup stack.
   \param oscl_leave_status tells the cause for the Leave
*/
#define OSCL_LEAVE(_leave_status) OsclError::Leave(_leave_status)


//! This macro will be used to set up a try block
/*!
   The try block identifies a block of code that might throw exceptions
   (or leave)
   \param oscl_leave_status oscl_leave_status will receive the result of any
   OSCL_LEAVE (which will get called from a OSCL_THROW) on systems that do not
   support exception handling.This is unused on systems that do support
   exception handling
   \param statements is a statement or block of statements that could throw
   exceptions and will be executed in the try block
*/
#define OSCL_TRY(_leave_status,_statements) _PV_TRAP(_leave_status,_statements)
//Same as above, but avoids the TLS lookup.
// param __trampimp is the OsclErrorTrapImp* for the current thread.
#define OSCL_TRY_NO_TLS(__trapimp,_leave_status,_statements) _PV_TRAP_NO_TLS(__trapimp,_leave_status,_statements)

//! This section defines the macros to be used in the catch block following the try block

//! Use this macro to call a function that handles all exception types thrown in the preceding try block
/*!
   \param _leave_status
   \param _statements is a statement or block of statements that will
   catch all the exception types thrown by the preceding try block
   This is a standalone macro and should not be used with any of the macros above
*/
#define OSCL_FIRST_CATCH_ANY(_leave_status, _statements) \
   if (_leave_status!=OsclErrNone) { _statements; }

//! Use this macro to define a block of code that catches the first exception type thrown in the preceding try block
/*!
   \param oscl_leave_status is the leave code that was returned by OSCL_THROW
   \param exceptiontype is the exception handled by this catch block
   This macro MUST be used in conjunction with either OSCL_LAST_CATCH or OSCL_CATCH_ANY
*/
#define OSCL_FIRST_CATCH( _leave_status, _catch_value, _statements) \
    if (_leave_status!=OsclErrNone && _leave_status == _catch_value){_statements;}

//! Use this macro to define a block of code for catching additional exception types
/*!
   OSCL_FIRST_CATCH can be used to catch one exception type. Then the
   OSCL_CATCH macro can be used to catch each subsequent type. The catch
   block must end with OSCL_LAST_CATCH or OSCL_CATCH_ANY
   \param oscl_leave_status is the result of any OSCL_THROW
   \param exceptiontype is the exception handled by this catch block
*/
#define OSCL_CATCH( _leave_status, _catch_value, _statements) \
    else if (_leave_status!=OsclErrNone && _leave_status == _catch_value){_statements;}

//! Use this macro to call a function that will catch all remaining exception types
/*!
   \param _leave_status
   \param _statements is a statement or block of statements to
   handle all remaining exception types.
   This macro ends the try block.
*/
#define OSCL_CATCH_ANY(_leave_status,_statements) \
    else if (_leave_status!=OsclErrNone){ _statements;}

//! Use this macro if OSCL_CATCH_ANY has not been used. It will mark the end of the catch block
/*!
   \param _leave_status will be propagated up the call stack
   This macro will do an OSCL_LEAVE if the leave has not been handled by the calls above.
   This macro ends the try block.
*/

#define OSCL_LAST_CATCH(_leave_status) \
    else if (_leave_status!=OsclErrNone){OSCL_LEAVE(_leave_status);}


#endif // INCLUDED_OSCL_EXCEPTION_H



/*! @} */
