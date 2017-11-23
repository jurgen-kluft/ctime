#ifndef __X_TIME_H__
#define __X_TIME_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
//  INCLUDES
//==============================================================================


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//==============================================================================
	// Library initialization
	//==============================================================================
	extern void    x_TimeInit (void);
	extern void    x_TimeExit (void);

	//==============================================================================
	// Types
	//==============================================================================
	typedef		s64		xtick;

	//==============================================================================
	// GLOBAL FUNCTIONS
	//==============================================================================

	extern f64		x_GetTimeSec        (void);
	extern s64		x_GetTicksPerSecond (void);

	extern xtick	x_GetTime           (void);
	extern f64		x_TicksToSec        (xtick inTicks);
	extern f64		x_TicksToMs         (xtick inTicks);
	extern f64		x_TicksToUs         (xtick inTicks);

	extern xtick	x_SecondsToTicks		(f64 inS);
	extern xtick	x_MillisecondsToTicks	(f64 inMs);
	extern xtick	x_MicrosecondsToTicks	(f64 inUs);

	//==============================================================================
	// INLINE
	//==============================================================================
	/**
	* ------------------------------------------------------------------------------
	*   Author:
	*       Virtuos
	*   Summary:
	*       Convert a period of time's measurement from xtick to second.
	*   Arguments:
	*       Ticks that have elapsed.
	*   Returns:
	*       Seconds referring a period of time that is equal to the ticks passed in.
	*   See Also:
	*       x_GetTicksPerSecond
	* ------------------------------------------------------------------------------
	*/
	inline f64		x_TicksToSec(xtick inTicks)
	{
		return ((f64)inTicks) / x_GetTicksPerSecond();
	}

	/**
	* ------------------------------------------------------------------------------
	*   Author:
	*       Virtuos
	*   Summary:
	*       Convert a period of time's measurement from xtick to millisecond.
	*   Arguments:
	*       Ticks that have elapsed.
	*   Returns:
	*       Milliseconds referring a period of time that is equal to the ticks passed in.
	*   See Also:
	*       x_GetTicksPerMs
	* ------------------------------------------------------------------------------
	*/
	inline f64		x_TicksToMs(xtick inTicks)
	{
		static f64 ms = 1000.0;
		return (((f64)inTicks) / x_GetTicksPerSecond()) * ms;
	}


	/**
	* ------------------------------------------------------------------------------
	*   Author:
	*       Virtuos
	*   Summary:
	*       Convert a period of time's measurement from xtick to millisecond.
	*   Arguments:
	*       Ticks that have elapsed.
	*   Returns:
	*       Milliseconds referring a period of time that is equal to the ticks passed in.
	*   See Also:
	*       x_GetTicksPerMs
	* ------------------------------------------------------------------------------
	*/
	inline f64		x_TicksToUs(xtick inTicks)
	{
		static f64 us = 1000000.0;
		return (((f64)inTicks) / x_GetTicksPerSecond()) * us;
	}


	/**
	* ------------------------------------------------------------------------------
	*   Author:
	*       Virtuos
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

	inline xtick	x_SecondsToTicks(f64 inS)
	{
		return (xtick)(inS * x_GetTicksPerSecond());
	}

	inline xtick	x_MillisecondsToTicks(f64 inMs)
	{
		static f64 ms = 1000.0;
		return (xtick)(inMs * (x_GetTicksPerSecond() / ms));
	}

	inline xtick	x_MicrosecondsToTicks(f64 inUs)
	{
		static f64 us = 1000000.0;
		return (xtick)(inUs * (x_GetTicksPerSecond() / us));
	}

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


//==============================================================================
// END __X_TIME_H__
//==============================================================================
#endif
