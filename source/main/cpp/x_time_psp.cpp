#include "xbase\x_target.h"
#ifdef TARGET_PSP

//==============================================================================
// INCLUDES
//==============================================================================
#include <psptypes.h>
#include <rtcsvc.h>
#include <kernel.h>
#include <powersvc.h>

#include "xbase\x_types.h"
#include "xbase\x_debug.h"
#include "xbase\x_time.h"
#include "xbase\x_string_std.h"

#include "xtime\x_time.h"
#include "xtime\x_timespan.h"
#include "xtime\x_datetime.h"


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	u64		xdatetime::sGetSystemTime()
	{
		ScePspDateTime localTime;
		sceRtcGetCurrentClockLocalTime(&localTime);

		xdatetime dt(1900 + localTime.year, localTime.month, localTime.day, localTime.hour, localTime.minute, localTime.second);
		return (u64)dt.ticks();
	}

	// ps3 has no functionality to get the file time
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
	static const u64		PSP_FREQ_PER_SEC	=	1000000;

	static f32              sPspFreqPerSec		=	PSP_FREQ_PER_SEC;
	static f32              sPspFreqPerMs		=	PSP_FREQ_PER_SEC / 1000.0f;
	static xtick            sBaseTimeTick		=	0;

	//==============================================================================
	// Functions
	//==============================================================================

	//------------------------------------------------------------------------------
	void x_TimeInit(void)
	{
		sBaseTimeTick	= sceKernelGetSystemTimeWide();
	}

	//------------------------------------------------------------------------------
	void x_TimeExit(void)
	{
	}

	//------------------------------------------------------------------------------
	s64 x_GetTicksPerSecond(void)
	{
		return sPspFreqPerSec;
	}

	//------------------------------------------------------------------------------

	s64 x_GetTicksPerMs(void)
	{
		return sPspFreqPerMs;
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
		s64 ticks = sceKernelGetSystemTimeWide() - sBaseTimeTick;
		ticks -= sBaseTimeTick;
		return ticks;
	}

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif /// TARGET_PSP
