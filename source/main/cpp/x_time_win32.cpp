#include "xbase\x_target.h"
#ifdef TARGET_PC

//==============================================================================
// INCLUDES
//==============================================================================
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMB
#define NOKANJI

#include <windows.h>
#include <mmsystem.h>
#include <time.h>

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
	u64		xdatetime::sGetSystemTime()
	{
		struct ::tm localTime;
		::time_t aclock;
		::time( &aclock );
		::localtime_s( &localTime, &aclock );

		xdatetime dt(localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
		return (u64)dt.ticks();
	}

	u64		xdatetime::sGetSystemTimeAsFileTime()
	{
		::FILETIME system_time;
		::GetSystemTimeAsFileTime(&system_time);
		u64 time = ((u64)system_time.dwHighDateTime << 32) + (u64)system_time.dwLowDateTime;
		return time;
	}

	u64		xdatetime::sGetSystemTimeFromFileTime(u64 inFileSystemTime)
	{
		u64 systemTime = sGetSystemTime();
		u64 fileTime = sGetSystemTimeAsFileTime();
		return inFileSystemTime - (fileTime - systemTime);
	}

	u64		xdatetime::sGetFileTimeFromSystemTime(u64 inSystemTime)
	{
		u64 systemTime = sGetSystemTime();
		u64 fileTime = sGetSystemTimeAsFileTime();
		return inSystemTime + (fileTime - systemTime);
	}

	//==============================================================================
	//  VARIABLES
	//==============================================================================
	//------------------------------------------------------------------------------
	//  Author:
	//      Virtuos
	//  Summary:
	//      The three static variables in the following are represented system timer's
	//      important states. They are all depend on the hardware installed. And here
	//      what we used is High-Resolution Timer, suppose that the system we used 
	//      include a high-resolution performance counter that provides high-resolution
	//      elapsed times.      
	//  Description:
	//      The three static variables are initialized in function x_TimeInit.
	//  See Also:
	//      x_TimeInit x_GetTicksPerSecond x_GetTicksPerMs x_GetTime
	//------------------------------------------------------------------------------
	static f64              sPCFreqPerSec = 0;
	static f64              sPCFreqPerMs = 0;
	static xtick            sBaseTimeTick = 0;

	//==============================================================================
	// Functions
	//==============================================================================

	//------------------------------------------------------------------------------
	//  Author:
	//      Virtuos
	//  Summary:
	//      Set sPCFreqPerMs,sPCFreqPerSec,sBaseTimeTick 's values, which are processor
	//      dependent.
	//  Arguments:
	//      void
	//  Returns:
	//      void
	//  Description:
	//      Local variable counter is used to refer to an incrementing variable,
	//      and represents the current value of the high-resolution performance counter.
	//      Often used to compute the elapsed counters while a section of code executed.
	//      sBaseTimeTick is the start counter of the timer.
	//
	//<P>   sPCFreqPerSec and sPCFreqPerMs both are frequecy of the counter above, but
	//      with different units persecond and permilliseconde.Their values are 
	//      processor dependent.On some processors, for example, their values might be
	//      the cycle rate of the processor clock. So there is a local variable named
	//      clockFreq.
	//  See Also:
	//      QueryPerformanceCounter QueryPerformanceFrequency
	//------------------------------------------------------------------------------
	void x_TimeInit(void)
	{
		LARGE_INTEGER counter;
		LARGE_INTEGER clockFreq;

		QueryPerformanceCounter(&counter);
		QueryPerformanceFrequency(&clockFreq); 

		sPCFreqPerMs    = (f64)clockFreq.QuadPart / 1000.0;
		sPCFreqPerSec   = (f64)clockFreq.QuadPart;
		sBaseTimeTick   = (xtick)counter.QuadPart;
	}
	
	void x_TimeExit(void)
	{

	}

	//------------------------------------------------------------------------------
	s64 x_GetTicksPerSecond(void)
	{
		return (s64)sPCFreqPerSec;
	}
	//------------------------------------------------------------------------------
	s64 x_GetTicksPerMs(void)
	{
		return (s64)sPCFreqPerMs ;
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
		ASSERT(sBaseTimeTick != 0);

		LARGE_INTEGER   counter;
		QueryPerformanceCounter(&counter);

		xtick ticks = (xtick)(counter.QuadPart) - sBaseTimeTick;

		static xtick sLastTicks = 0;
		// Try to help the PC bug
		if(ticks < sLastTicks)     
			ticks = sLastTicks + 1;

		sLastTicks = ticks;

		return ticks;
	}

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};

#endif // TARGET_PC
