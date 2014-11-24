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

//                   O S C L _ D L L

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/**
 *  @file oscl_dll.h
 *  @brief Defines a DLL entry point.
 *
 */

#ifndef OSCL_DLL_H_INCLUDED
#define OSCL_DLL_H_INCLUDED

#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

/**
 * DLL entry/exit point.
 *
 * Allows you to define custom operations at the
 * entry and exit of the DLL.  Place this macro
 * within one souce file for each DLL.
 *
 * Functions with the custom commands for the
 * DLL entry and exit point must also be defined.
 * The entry point custom function is LocalDllEntry(),
 * and the exit point custom function is LocalDllExit().
 *
 * These functions will be called as a result of
 * executing this macro.
 *
 * Usage :
 *
 * LocalDllEntry() {
 *    custom operations...
 * }
 *
 * LocalDllExit() {
 *    custom operations...
 * }
 *
 * OSCL_DLL_ENTRY_POINT()
 *
 */
#define OSCL_DLL_ENTRY_POINT() void oscl_dll_entry_point() {}


/**
 * Default DLL entry/exit point function.
 *
 * The body of the DLL entry point is given.  The macro
 * only needs to be declared within the source file.
 *
 * Usage :
 *
 * OSCL_DLL_ENTRY_POINT_DEFAULT()
 */

#define OSCL_DLL_ENTRY_POINT_DEFAULT()



/*! @} */


#endif
