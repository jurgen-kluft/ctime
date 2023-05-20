#ifndef __CTIME_H__
#define __CTIME_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
    namespace ntime
    {
        extern void init(void);
        extern void exit(void);
        typedef s64 tick_t;

        extern f64 getTimeSec(void);
        extern s64 getTicksPerSecond(void);

        extern tick_t getTime(void);
        extern f64    ticksToSec(tick_t inTicks);
        extern f64    ticksToMs(tick_t inTicks);
        extern f64    ticksToUs(tick_t inTicks);

        extern tick_t secondsToTicks(f64 inS);
        extern tick_t millisecondsToTicks(f64 inMs);
        extern tick_t microsecondsToTicks(f64 inUs);

        //==============================================================================
        // INLINE
        //==============================================================================
        /**
         * ------------------------------------------------------------------------------
         *   Author:
         *       Jurgen
         *   Summary:
         *       Convert a period of time's measurement from tick_t to second.
         *   Arguments:
         *       Ticks that have elapsed.
         *   Returns:
         *       Seconds referring a period of time that is equal to the ticks passed in.
         *   See Also:
         *       getTicksPerSecond
         * ------------------------------------------------------------------------------
         */
        inline f64 ticksToSec(tick_t inTicks) { return ((f64)inTicks) / getTicksPerSecond(); }

        /**
         * ------------------------------------------------------------------------------
         *   Author:
         *       Jurgen
         *   Summary:
         *       Convert a period of time's measurement from tick_t to millisecond.
         *   Arguments:
         *       Ticks that have elapsed.
         *   Returns:
         *       Milliseconds referring a period of time that is equal to the ticks passed in.
         *   See Also:
         *       getTicksPerMs
         * ------------------------------------------------------------------------------
         */
        inline f64 ticksToMs(tick_t inTicks)
        {
            static f64 ms = 1000.0;
            return (((f64)inTicks) / getTicksPerSecond()) * ms;
        }

        /**
         * ------------------------------------------------------------------------------
         *   Author:
         *       Jurgen
         *   Summary:
         *       Convert a period of time's measurement from tick_t to millisecond.
         *   Arguments:
         *       Ticks that have elapsed.
         *   Returns:
         *       Milliseconds referring a period of time that is equal to the ticks passed in.
         *   See Also:
         *       getTicksPerMs
         * ------------------------------------------------------------------------------
         */
        inline f64 ticksToUs(tick_t inTicks)
        {
            static f64 us = 1000000.0;
            return (((f64)inTicks) / getTicksPerSecond()) * us;
        }

        /**
         * ------------------------------------------------------------------------------
         *   Author:
         *       Jurgen
         *   Summary:
         *       Get elapsed time from timer initialized in second.
         *   Arguments:
         *       void
         *   Returns:
         *       Seconds that have elapsed from timeInit called
         *   See Also:
         *       getTime ticksToSec
         * ------------------------------------------------------------------------------
         */
        inline f64 getTimeSec(void) { return ticksToSec(getTime()); }

        inline tick_t secondsToTicks(f64 inS) { return (tick_t)(inS * getTicksPerSecond()); }

        inline tick_t millisecondsToTicks(f64 inMs)
        {
            static f64 ms = 1000.0;
            return (tick_t)(inMs * (getTicksPerSecond() / ms));
        }

        inline tick_t microsecondsToTicks(f64 inUs)
        {
            static f64 us = 1000000.0;
            return (tick_t)(inUs * (getTicksPerSecond() / us));
        }
    } // namespace ntime

}; // namespace ncore

#endif
