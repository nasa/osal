/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 *
 * Declarations and prototypes for osapi-clock module
 */

#ifndef OSAPI_CLOCK_H
#define OSAPI_CLOCK_H

#include "osconfig.h"
#include "common_types.h"

/**
 * @brief OSAL time interval structure
 *
 * This is used to represent a basic time interval.
 *
 * When used with OS_GetLocalTime/OS_SetLocalTime, this represents the
 * interval from the OS's epoch point, typically 01 Jan 1970 00:00:00 UTC
 * on systems that have a persistent real time clock (RTC), or the system
 * boot time if there is no RTC available.
 *
 * Applications should not directly access fields within this structure,
 * as the definition may change in future versions of OSAL.  Instead,
 * applications should use the accessor/conversion methods defined below.
 */
typedef struct
{
    int64 ticks; /**< Ticks elapsed since reference point */
} OS_time_t;

/**
 * @brief Multipliers/divisors to convert ticks into standardized units
 *
 * Various fixed conversion factor constants used by the conversion routines
 *
 * A 100ns tick time allows max intervals of about +/- 14000 years in
 * a 64-bit signed integer value.
 *
 * @note Applications should not directly use these values, but rather use
 * conversion routines below to obtain standardized units (seconds/microseconds/etc).
 */
enum
{
    OS_TIME_TICK_RESOLUTION_NS = 100,
    OS_TIME_TICKS_PER_SECOND   = 1000000000 / OS_TIME_TICK_RESOLUTION_NS,
    OS_TIME_TICKS_PER_MSEC     = 1000000 / OS_TIME_TICK_RESOLUTION_NS,
    OS_TIME_TICKS_PER_USEC     = 1000 / OS_TIME_TICK_RESOLUTION_NS
};

/** @defgroup OSAPIClock OSAL Real Time Clock APIs
 * @{
 */

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get the local time
 *
 * This function gets the local time from the underlying OS.
 *
 * @note Mission time management typically uses the cFE Time Service
 *
 * @param[out]  time_struct An OS_time_t that will be set to the current time @nonnull
 *
 * @return Get local time status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if time_struct is null
 */
int32 OS_GetLocalTime(OS_time_t *time_struct);

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Set the local time
 *
 * This function sets the local time on the underlying OS.
 *
 * @note Mission time management typically uses the cFE Time Services
 *
 * @param[in]  time_struct An OS_time_t containing the current time @nonnull
 *
 * @return Set local time status, see @ref OSReturnCodes
 * @retval #OS_SUCCESS @copybrief OS_SUCCESS
 * @retval #OS_INVALID_POINTER if time_struct is null
 */
int32 OS_SetLocalTime(const OS_time_t *time_struct);

/*-------------------------------------------------------------------------------------*/
/*
 * Accessor / Unit Conversion routines for OS_time_t
 *
 * These routines allow the user to simply interpret OS_time_t intervals into
 * in normalized units of whole seconds, milliseconds, microseconds, or nanoseconds.
 */
/*-------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get interval from an OS_time_t object normalized to whole number of seconds
 *
 * Extracts the number of whole seconds from a given OS_time_t object, discarding
 * any fractional component.
 *
 * This may also replace a direct read of the "seconds" field from
 * the OS_time_t object from previous versions of OSAL, where the
 * structure was defined with separate seconds/microseconds fields.
 *
 * @sa OS_TimeGetMicrosecondsPart()
 * @sa OS_TimeFromTotalSeconds()
 *
 * @param[in] tm    Time interval value
 * @returns   Whole number of seconds in time interval
 */
static inline int64 OS_TimeGetTotalSeconds(OS_time_t tm)
{
    return (tm.ticks / OS_TIME_TICKS_PER_SECOND);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get an OS_time_t interval object from an integer number of seconds
 *
 * This is the inverse operation of OS_TimeGetTotalSeconds(), converting the
 * total number of seconds into an OS_time_t value.
 *
 * @sa OS_TimeGetTotalSeconds()
 *
 * @param[in] tm    Time interval value, in seconds
 * @returns   OS_time_t value representing the interval
 */
static inline OS_time_t OS_TimeFromTotalSeconds(int64 tm)
{
    return (OS_time_t) {.ticks = (tm * OS_TIME_TICKS_PER_SECOND)};
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get interval from an OS_time_t object normalized to millisecond units
 *
 * Note this refers to the complete interval, not just the fractional part.
 *
 * @sa OS_TimeFromTotalMilliseconds()
 *
 * @param[in] tm    Time interval value
 * @returns   Whole number of milliseconds in time interval
 */
static inline int64 OS_TimeGetTotalMilliseconds(OS_time_t tm)
{
    return (tm.ticks / OS_TIME_TICKS_PER_MSEC);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get an OS_time_t interval object from a integer number of milliseconds
 *
 * This is the inverse operation of OS_TimeGetTotalMilliseconds(), converting the
 * total number of milliseconds into an OS_time_t value.
 *
 * @sa OS_TimeGetTotalMilliseconds()
 *
 * @param[in] tm    Time interval value, in milliseconds
 * @returns   OS_time_t value representing the interval
 */
static inline OS_time_t OS_TimeFromTotalMilliseconds(int64 tm)
{
    return (OS_time_t) {.ticks = (tm * OS_TIME_TICKS_PER_MSEC)};
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get interval from an OS_time_t object normalized to microsecond units
 *
 * Note this refers to the complete interval, not just the fractional part.
 *
 * @sa OS_TimeFromTotalMicroseconds()
 *
 * @param[in] tm    Time interval value
 * @returns   Whole number of microseconds in time interval
 */
static inline int64 OS_TimeGetTotalMicroseconds(OS_time_t tm)
{
    return (tm.ticks / OS_TIME_TICKS_PER_USEC);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get an OS_time_t interval object from a integer number of microseconds
 *
 * This is the inverse operation of OS_TimeGetTotalMicroseconds(), converting the
 * total number of microseconds into an OS_time_t value.
 *
 * @sa OS_TimeGetTotalMicroseconds()
 *
 * @param[in] tm    Time interval value, in microseconds
 * @returns   OS_time_t value representing the interval
 */
static inline OS_time_t OS_TimeFromTotalMicroseconds(int64 tm)
{
    return (OS_time_t) {.ticks = (tm * OS_TIME_TICKS_PER_USEC)};
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get interval from an OS_time_t object normalized to nanosecond units
 *
 * Note this refers to the complete interval, not just the fractional part.
 *
 * @note There is no protection against overflow of the 64-bit return value.
 * Applications must use caution to ensure that the interval does not exceed the
 * representable range of a signed 64 bit integer - approximately 140 years.
 *
 * @sa OS_TimeFromTotalNanoseconds
 *
 * @param[in] tm    Time interval value
 * @returns   Whole number of microseconds in time interval
 */
static inline int64 OS_TimeGetTotalNanoseconds(OS_time_t tm)
{
    return (tm.ticks * OS_TIME_TICK_RESOLUTION_NS);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get an OS_time_t interval object from a integer number of nanoseconds
 *
 * This is the inverse operation of OS_TimeGetTotalNanoseconds(), converting the
 * total number of nanoseconds into an OS_time_t value.
 *
 * @sa OS_TimeGetTotalNanoseconds()
 *
 * @param[in] tm    Time interval value, in nanoseconds
 * @returns   OS_time_t value representing the interval
 */
static inline OS_time_t OS_TimeFromTotalNanoseconds(int64 tm)
{
    return (OS_time_t) {.ticks = (tm / OS_TIME_TICK_RESOLUTION_NS)};
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get subseconds portion (fractional part only) from an OS_time_t object
 *
 * Extracts the fractional part from a given OS_time_t object.
 * Units returned are in ticks, not normalized to any standard time unit.
 *
 * @param[in] tm    Time interval value
 * @returns Fractional/subsecond portion of time interval in ticks
 */
static inline int64 OS_TimeGetFractionalPart(OS_time_t tm)
{
    return (tm.ticks % OS_TIME_TICKS_PER_SECOND);
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get 32-bit normalized subseconds (fractional part only) from an OS_time_t object
 *
 * Extracts the fractional part from a given OS_time_t object in maximum precision,
 * with units of 2^(-32) sec.  This is a base-2 fixed-point fractional value
 * with the point left-justified in the 32-bit value (i.e. left of MSB).
 *
 * This is (mostly) compatible with the CFE "subseconds"  value, where 0x80000000 represents
 * exactly one half second, and 0 represents a full second.
 *
 * @param[in] tm    Time interval value
 * @returns Fractional/subsecond portion of time interval as 32-bit fixed point value
 */
static inline uint32 OS_TimeGetSubsecondsPart(OS_time_t tm)
{
    /*
     * This computation avoids a 32-bit left shift which may not be implemented.
     *
     * It also must round up, otherwise this may result in a value one
     * less than the original when converted back to usec again.
     */
    int64 frac = (OS_TimeGetFractionalPart(tm) << 30) + (OS_TIME_TICKS_PER_SECOND >> 2);
    return (uint32)((frac - 1) / (OS_TIME_TICKS_PER_SECOND >> 2));
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get milliseconds portion (fractional part only) from an OS_time_t object
 *
 * Extracts the fractional part from a given OS_time_t object normalized
 * to units of milliseconds.
 *
 * @sa OS_TimeGetTotalSeconds()
 *
 * @param[in] tm    Time interval value
 * @returns Number of milliseconds in time interval
 */
static inline uint32 OS_TimeGetMillisecondsPart(OS_time_t tm)
{
    return (uint32)OS_TimeGetFractionalPart(tm) / OS_TIME_TICKS_PER_MSEC;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get microseconds portion (fractional part only) from an OS_time_t object
 *
 * Extracts the fractional part from a given OS_time_t object normalized
 * to units of microseconds.
 *
 * This function may be used to adapt applications initially implemented
 * using an older OSAL version where OS_time_t was a structure containing
 * a "seconds" and "microsecs" field.
 *
 * This function will obtain a value that is compatible with the "microsecs" field of
 * OS_time_t as it was defined in previous versions of OSAL, as well as the "tv_usec"
 * field of POSIX-style "struct timeval" values.
 *
 * @sa OS_TimeGetTotalSeconds()
 *
 * @param[in] tm    Time interval value
 * @returns Number of microseconds in time interval
 */
static inline uint32 OS_TimeGetMicrosecondsPart(OS_time_t tm)
{
    return (uint32)OS_TimeGetFractionalPart(tm) / OS_TIME_TICKS_PER_USEC;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Get nanoseconds portion (fractional part only) from an OS_time_t object
 *
 * Extracts the only number of nanoseconds from a given OS_time_t object.
 *
 * This function will obtain a value that is compatible with the "tv_nsec" field
 * of POSIX-style "struct timespec" values.
 *
 * @sa OS_TimeGetTotalSeconds()
 *
 * @param[in] tm    Time interval value
 * @returns Number of nanoseconds in time interval
 */
static inline uint32 OS_TimeGetNanosecondsPart(OS_time_t tm)
{
    return (uint32)OS_TimeGetFractionalPart(tm) * OS_TIME_TICK_RESOLUTION_NS;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Assemble/Convert a number of seconds + nanoseconds into an OS_time_t interval
 *
 * This creates an OS_time_t value using a whole number of seconds and a fractional
 * part in units of nanoseconds.  This is the inverse of OS_TimeGetTotalSeconds()
 * and OS_TimeGetNanosecondsPart(), and should recreate the original OS_time_t
 * value from these separate values (aside from any potential conversion losses
 * due to limited resolution of the data types/units).
 *
 * @sa OS_TimeGetTotalSeconds(), OS_TimeGetNanosecondsPart()
 *
 * @param[in] seconds       Whole number of seconds
 * @param[in] nanoseconds   Number of nanoseconds (fractional part only)
 * @returns The input arguments represented as an OS_time_t interval
 */
static inline OS_time_t OS_TimeAssembleFromNanoseconds(int64 seconds, uint32 nanoseconds)
{
    OS_time_t result;
    result.ticks = seconds * OS_TIME_TICKS_PER_SECOND;
    result.ticks += nanoseconds / OS_TIME_TICK_RESOLUTION_NS;
    return result;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Assemble/Convert a number of seconds + microseconds into an OS_time_t interval
 *
 * This creates an OS_time_t value using a whole number of seconds and a fractional
 * part in units of microseconds.  This is the inverse of OS_TimeGetTotalSeconds()
 * and OS_TimeGetMicrosecondsPart(), and should recreate the original OS_time_t
 * value from these separate values (aside from any potential conversion losses
 * due to limited resolution of the data types/units).
 *
 * @sa OS_TimeGetTotalSeconds(), OS_TimeGetMicrosecondsPart()
 *
 * @param[in] seconds       Whole number of seconds
 * @param[in] microseconds  Number of microseconds (fractional part only)
 * @returns The input arguments represented as an OS_time_t interval
 */
static inline OS_time_t OS_TimeAssembleFromMicroseconds(int64 seconds, uint32 microseconds)
{
    OS_time_t result;
    result.ticks = seconds * OS_TIME_TICKS_PER_SECOND;
    result.ticks += microseconds * OS_TIME_TICKS_PER_USEC;
    return result;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Assemble/Convert a number of seconds + milliseconds into an OS_time_t interval
 *
 * This creates an OS_time_t value using a whole number of seconds and a fractional
 * part in units of milliseconds.  This is the inverse of OS_TimeGetTotalSeconds()
 * and OS_TimeGetMillisecondsPart(), and should recreate the original OS_time_t
 * value from these separate values (aside from any potential conversion losses
 * due to limited resolution of the data types/units).
 *
 * @sa OS_TimeGetTotalSeconds(), OS_TimeGetMillisecondsPart()
 *
 * @param[in] seconds       Whole number of seconds
 * @param[in] milliseconds  Number of milliseconds (fractional part only)
 * @returns The input arguments represented as an OS_time_t interval
 */
static inline OS_time_t OS_TimeAssembleFromMilliseconds(int64 seconds, uint32 milliseconds)
{
    OS_time_t result;
    result.ticks = seconds * OS_TIME_TICKS_PER_SECOND;
    result.ticks += milliseconds * OS_TIME_TICKS_PER_MSEC;
    return result;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Assemble/Convert a number of seconds + subseconds into an OS_time_t interval
 *
 * This creates an OS_time_t value using a whole number of seconds and a fractional
 * part in units of sub-seconds (1/2^32).  This is the inverse of OS_TimeGetTotalSeconds()
 * and OS_TimeGetSubsecondsPart(), and should recreate the original OS_time_t
 * value from these separate values (aside from any potential conversion losses
 * due to limited resolution of the data types/units).
 *
 * @sa OS_TimeGetTotalSeconds(), OS_TimeGetNanosecondsPart()
 * @param[in] seconds       Whole number of seconds
 * @param[in] subseconds    Number of subseconds (32 bit fixed point fractional part)
 * @returns The input arguments represented as an OS_time_t interval
 */
static inline OS_time_t OS_TimeAssembleFromSubseconds(int64 seconds, uint32 subseconds)
{
    OS_time_t result;
    result.ticks = seconds * OS_TIME_TICKS_PER_SECOND;
    /* this should not round in any way, as the 32-bit input value has higher precision */
    result.ticks += ((int64)subseconds * (OS_TIME_TICKS_PER_SECOND >> 2)) >> 30;
    return result;
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Computes the sum of two time intervals
 *
 * @param[in]  time1 The first interval
 * @param[in]  time2 The second interval
 *
 * @return The sum of the two intervals (time1 + time2)
 */
static inline OS_time_t OS_TimeAdd(OS_time_t time1, OS_time_t time2)
{
    return ((OS_time_t) {time1.ticks + time2.ticks});
}

/*-------------------------------------------------------------------------------------*/
/**
 * @brief Computes the difference between two time intervals
 *
 * @param[in]  time1 The first interval
 * @param[in]  time2 The second interval
 *
 * @return The difference of the two intervals (time1 - time2)
 */
static inline OS_time_t OS_TimeSubtract(OS_time_t time1, OS_time_t time2)
{
    return ((OS_time_t) {time1.ticks - time2.ticks});
}

/**@}*/

#endif /* OSAPI_CLOCK_H */
