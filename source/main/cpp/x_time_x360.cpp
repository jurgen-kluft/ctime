#include "xbase\x_target.h"
#ifdef TARGET_360

//==============================================================================
// INCLUDES
//==============================================================================
#include <time.h>
#include <Xtl.h>

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
    //------------------------------------------------------------------------------
    u64		xdatetime::sGetSystemTime()
    {
        struct ::tm localTime;
        ::time_t aclock;
        ::time( &aclock );
        ::localtime_s( &localTime, &aclock );

        xdatetime dt(1900 + localTime.tm_year, localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        return (u64)dt.ticks();
    }

    //------------------------------------------------------------------------------
    u64		xdatetime::sGetSystemTimeAsFileTime()
    {
         ::FILETIME system_time;
         ::GetSystemTimeAsFileTime(&system_time);
         u64 time = ((u64)system_time.dwHighDateTime << 32) + (u64)system_time.dwLowDateTime;
        return time;
    }

    //------------------------------------------------------------------------------
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

    //------------------------------------------------------------------------------


	//==============================================================================
	// VARIABLES
	//==============================================================================
	static f64              sPCFreqPerSec = 0;
	static f64              sPCFreqPerMs = 0;
	static xtick            sBaseTimeTick = 0;

	//==============================================================================
	// Functions
	//==============================================================================

	//------------------------------------------------------------------------------

	void x_TimeInit(void)
	{
		LARGE_INTEGER counter;
		LARGE_INTEGER clockFreq;

		QueryPerformanceCounter(&counter);
		QueryPerformanceFrequency(&clockFreq); 

		sPCFreqPerMs	= (f64)clockFreq.QuadPart / 1000.0;
		sPCFreqPerSec	= (f64)clockFreq.QuadPart;
		sBaseTimeTick	= (xtick)counter.QuadPart;
	}
	//------------------------------------------------------------------------------
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

	//------------------------------------------------------------------------------
	void xtime_utc::updateTime (void)
	{
		__time64_t ltime;

		_time64(&ltime);
		miTime = ltime;
	}
	//------------------------------------------------------------------------------
	void xtime_utc::clear (void)
	{
		miTime = 0;
	}

	//------------------------------------------------------------------------------
	s32 xtime_utc::getSeconds(void) const
	{
		struct tm today;
		_localtime64_s(&today, (__time64_t*)&miTime);
		return today.tm_sec;
	}

	//------------------------------------------------------------------------------
	s32 xtime_utc::getMinutes(void) const
	{
		struct tm today;
		_localtime64_s(&today, (__time64_t*)&miTime);
		return today.tm_min;
	}

	//------------------------------------------------------------------------------
	s32 xtime_utc::getHours24(void) const
	{
		struct tm today;
		_localtime64_s(&today, (__time64_t*)&miTime);
		return today.tm_hour;
	}

	//------------------------------------------------------------------------------
	s32 xtime_utc::getHours12(void) const
	{
		struct tm today;
		_localtime64_s(&today, (__time64_t*)&miTime);
		return today.tm_hour%12;
	}

	//------------------------------------------------------------------------------
	xdatetime xtime_utc::getDate(void) const
	{
		struct tm today;
		_localtime64_s(&today, (__time64_t*)&miTime);
		xdatetime date(today.tm_year, today.tm_mon, today.tm_mday);
		return date;
	}

	//------------------------------------------------------------------------------
	xbool xtime_utc::operator <  (const xtime_utc inTime) const
	{
		return xbool(miTime < inTime.miTime);
	}

	//------------------------------------------------------------------------------
	xbool xtime_utc::operator <= (const xtime_utc inTime) const
	{
		return xbool(miTime <= inTime.miTime);
	}

	//------------------------------------------------------------------------------
	xbool xtime_utc::operator >  (const xtime_utc inTime) const
	{
		return xbool(miTime > inTime.miTime);
	}

	//------------------------------------------------------------------------------
	xbool xtime_utc::operator >= (const xtime_utc inTime) const
	{
		return xbool(miTime >= inTime.miTime);
	}

	//------------------------------------------------------------------------------
	xbool xtime_utc::operator == (const xtime_utc inTime) const
	{
		return xbool(miTime == inTime.miTime);
	}

	//------------------------------------------------------------------------------
	xtime_utc xtime_utc::operator + (const xtime_utc inTime) const
	{
		xtime_utc sumTime;
		sumTime.miTime = miTime + inTime.miTime;
		return sumTime;
	}

	//------------------------------------------------------------------------------
	xtime_utc xtime_utc::operator - (const xtime_utc inTime) const
	{
		xtime_utc subtractTime;
		subtractTime.miTime = miTime - inTime.miTime;
		return subtractTime;
	}

	//------------------------------------------------------------------------------
	const xtime_utc& xtime_utc::operator += (const xtime_utc inTime)
	{
		miTime += inTime.miTime;
		return *this;
	}

	//------------------------------------------------------------------------------
	const xtime_utc& xtime_utc::operator -= (const xtime_utc inTime)
	{
		miTime -= inTime.miTime;
		return *this;
	}

//==============================================================================
// END xCore namespace
//==============================================================================
};

#endif /// TARGET_360
