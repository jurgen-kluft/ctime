#include "xbase\x_target.h"
#ifdef TARGET_3DS

//==============================================================================
// INCLUDES
//==============================================================================
#include <nn/os/os_Tick.h>
#include <nn/fnd/fnd_DateTime.h>

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
		nn::fnd::DateTime dt = nn::fnd::DateTime::GetNow();
		xdatetime xdt(dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMilliSecond());
		return xdt;
	}

	u64		xdatetime::sGetSystemTime()
	{
		xdatetime xdt = sGetDateTimeNow();
		return (u64)xdt.ticks();
	}

	// 3DS has no functionality to get the file time
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
    static s64              s3dsFreqPerSec = 0;
    static s64              s3dsFreqPerMs = 0;
    static xtick            sBaseTimeTick = 0;

    //==============================================================================
    // Functions
    //==============================================================================

    //------------------------------------------------------------------------------
    void x_TimeInit(void)
    {
        s3dsFreqPerSec  = NN_HW_TICKS_PER_SECOND;
        s3dsFreqPerMs   = NN_HW_TICKS_PER_SECOND / 1000;
        sBaseTimeTick   = (s64)nn::os::Tick::GetSystemCurrent();
		if (sBaseTimeTick==0)	sBaseTimeTick = 1;
    }

    //------------------------------------------------------------------------------
    void x_TimeExit(void)
    {
		sBaseTimeTick = 0;
    }

    //------------------------------------------------------------------------------
    s64 x_GetTicksPerSecond(void)
    {
        return s3dsFreqPerSec;
    }

    //------------------------------------------------------------------------------

    s64 x_GetTicksPerMs(void)
    {
        return s3dsFreqPerMs;
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
        s64 ticks = (s64)nn::os::Tick::GetSystemCurrent() - sBaseTimeTick;
        return ticks;
    }


    //==============================================================================
    // END xCore namespace
    //==============================================================================
};

#endif /// TARGET_3DS
