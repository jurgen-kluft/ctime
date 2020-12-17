#ifndef __X_TIME_H__
#define __X_TIME_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

namespace xtime
{
	extern void    x_Init(void);
	extern void    x_Exit(void);
}

namespace xcore
{
	typedef		s64		tick_t;

	extern f64		x_GetTimeSec        (void);
	extern s64		x_GetTicksPerSecond (void);

	extern tick_t	x_GetTime           (void);
	extern f64		x_TicksToSec        (tick_t inTicks);
	extern f64		x_TicksToMs         (tick_t inTicks);
	extern f64		x_TicksToUs         (tick_t inTicks);

	extern tick_t	x_SecondsToTicks		(f64 inS);
	extern tick_t	x_MillisecondsToTicks	(f64 inMs);
	extern tick_t	x_MicrosecondsToTicks	(f64 inUs);

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
	*       x_GetTicksPerSecond
	* ------------------------------------------------------------------------------
	*/
	inline f64		x_TicksToSec(tick_t inTicks)
	{
		return ((f64)inTicks) / x_GetTicksPerSecond();
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
	*       x_GetTicksPerMs
	* ------------------------------------------------------------------------------
	*/
	inline f64		x_TicksToMs(tick_t inTicks)
	{
		static f64 ms = 1000.0;
		return (((f64)inTicks) / x_GetTicksPerSecond()) * ms;
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
	*       x_GetTicksPerMs
	* ------------------------------------------------------------------------------
	*/
	inline f64		x_TicksToUs(tick_t inTicks)
	{
		static f64 us = 1000000.0;
		return (((f64)inTicks) / x_GetTicksPerSecond()) * us;
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
	*       Seconds that have elapsed from x_TimeInit called
	*   See Also:
	*       x_GetTime x_TicksToSec
	* ------------------------------------------------------------------------------
	*/
	inline f64		x_GetTimeSec(void)
	{
		return x_TicksToSec(x_GetTime());
	}

	inline tick_t	x_SecondsToTicks(f64 inS)
	{
		return (tick_t)(inS * x_GetTicksPerSecond());
	}

	inline tick_t	x_MillisecondsToTicks(f64 inMs)
	{
		static f64 ms = 1000.0;
		return (tick_t)(inMs * (x_GetTicksPerSecond() / ms));
	}

	inline tick_t	x_MicrosecondsToTicks(f64 inUs)
	{
		static f64 us = 1000000.0;
		return (tick_t)(inUs * (x_GetTicksPerSecond() / us));
	}

};

#endif
