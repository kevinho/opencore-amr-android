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

//                     O S C L _ M A T H

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/*!
 * \file oscl_math.h
 * \brief Provides math functions.
 *
 */

#ifndef OSCL_MATH_H_INCLUDED
#define OSCL_MATH_H_INCLUDED

#ifndef OSCLCONFIG_UTIL_H_INCLUDED
#include "osclconfig_util.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

/**
 * Calculates the natural log of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_log(double value);
/**
 * Calculates tthe logarithm to base 10 of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_log10(double value);
/**
 * Calculates the square root of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_sqrt(double value);
/**
 * Calculates the value of x to the power of y
 *
 * @param x  base value
 * @param y  power
 */
OSCL_COND_IMPORT_REF double oscl_pow(double x, double y);
/**
 * Calculates the exponential of e for a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_exp(double value);

/**
 * Calculates the sine of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_sin(double value);

/**
 * Calculates the cosine of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_cos(double value);

/**
 * Calculates the tangential of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_tan(double value);

/**
 * Calculates the arc since of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_asin(double value);

/**
 * Calculates the arc tangent of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_atan(double value);

/**
 * Calculates the floor of a number
 *
 * @param value  source value
 */
OSCL_COND_IMPORT_REF double oscl_floor(double value);

#if (!OSCL_DISABLE_INLINES)
#include "oscl_math.inl"
#endif

#endif

/*! @} */

