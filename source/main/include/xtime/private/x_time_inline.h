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
    return ((f64)inTicks)/x_GetTicksPerSecond();
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
    return (((f64)inTicks)/x_GetTicksPerSecond()) * ms;
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
    return (((f64)inTicks)/x_GetTicksPerSecond()) * us;
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



//==============================================================================
// END
//==============================================================================
