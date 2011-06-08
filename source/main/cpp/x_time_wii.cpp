#include "xbase\x_target.h"
#ifdef TARGET_WII

//==============================================================================
// INCLUDES
//==============================================================================
#include <revolution/os.h>

#include "xbase\x_types.h"
#include "xbase\x_debug.h"
#include "xbase\x_string_std.h"

#include "xtime\x_time.h"
#include "xtime\x_timespan.h"
#include "xtime\x_datetime.h"


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	static xdatetime sGetDateTimeNow()
	{
		OSCalendarTime ct;

		OSTime time = OSGetTime();
		OSTicksToCalendarTime(time, &ct);

		xdatetime xdt(1900 + ct.year, ct.mon + 1, ct.mday, ct.hour, ct.min, ct.sec);
		return xdt;
	}

	u64		xdatetime::sGetSystemTime()
	{
		xdatetime dt = sGetDateTimeNow();
		return (u64)dt.ticks();
	}

	// WII has no functionality to get the file time
	u64		xdatetime::sGetSystemTimeAsFileTime()
	{
		return sGetSystemTime();
	}

	u64		xdatetime::sGetSystemTimeFromFileTime(u64 inFileSystemTime)
	{
		return inFileSystemTime;
	}

	u64		xdatetime::sGetFileTimeFromSystemTime(u64 inSystemTime)
	{
		return inSystemTime;
	}

    //==============================================================================
    // VARIABLES
    //==============================================================================
    static s64              sWiiFreqPerSec = 0;
    static s64              sWiiFreqPerMs = 0;
    static xtick            sBaseTimeTick = 0;

    //==============================================================================
    // Functions
    //==============================================================================

    //------------------------------------------------------------------------------
    void x_TimeInit(void)
    {
        sWiiFreqPerSec  = OS_TIMER_CLOCK;
        sWiiFreqPerMs   = OS_TIMER_CLOCK / 1000.0;
        sBaseTimeTick   = (s64)OSGetTime();
    }

    //------------------------------------------------------------------------------
    void x_TimeExit(void)
    {
    }

    //------------------------------------------------------------------------------
    s64 x_GetTicksPerSecond(void)
    {
        return sWiiFreqPerSec;
    }

    //------------------------------------------------------------------------------

    s64 x_GetTicksPerMs(void)
    {
        return sWiiFreqPerMs;
    }

    //------------------------------------------------------------------------------
    //  Author:
    //      Virtuos
    //  Summary:
    //      Get elapsed time from timer initialized in second.
    //  Arguments:
    //      void
    //  Returns:
    //      Ticks that have elapsed from x_TimeInit called
    //  Description:
    //      use xcritical_section to make PerformanceCounter owned by only one thread
    //      at a time.
    //  See Also:
    //      xcritical_section
    //------------------------------------------------------------------------------
    s64 x_GetTime(void)
    {
        ASSERT(sBaseTimeTick);

        s64 ticks = OSGetTime() - sBaseTimeTick;

        return ticks;
    }

    //==============================================================================
    // END xCore namespace
    //==============================================================================
};

#endif /// TARGET_WII
