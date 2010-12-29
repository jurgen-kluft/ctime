#include "../x_target.h"
#ifdef TARGET_PSP

//==============================================================================
// INCLUDES
//==============================================================================
#include <psptypes.h>
#include <rtcsvc.h>
#include <kernel.h>
#include <powersvc.h>

#include "../x_types.h"
#include "../x_debug.h"
#include "../x_time.h"
#include "../x_string.h"
#include "../x_thread.h"

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
	void x_TimeKill(void)
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
	//      Tomas Arce
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

	//------------------------------------------------------------------------------
	void xtime_utc::updateTime (void)
	{
		miTime = sceKernelGetSystemTimeWide();
	}
	//------------------------------------------------------------------------------
	void xtime_utc::clear (void)
	{
		miTime = 0;
	}

	//------------------------------------------------------------------------------
	s32 xtime_utc::getSeconds(void) const
	{
		ScePspDateTime today;
		sceRtcConvertTickToDateTime((SceRtcTick*)(&miTime), &today);
		return today.second;
	}

	//------------------------------------------------------------------------------
	s32 xtime_utc::getMinutes(void) const
	{
		ScePspDateTime today;
		sceRtcConvertTickToDateTime((SceRtcTick*)(&miTime), &today);
		return today.minute;
	}

	//------------------------------------------------------------------------------
	s32 xtime_utc::getHours24(void) const
	{
		ScePspDateTime today;
		sceRtcConvertTickToDateTime((SceRtcTick*)(&miTime), &today);
		return today.hour;
	}

	//------------------------------------------------------------------------------
	s32 xtime_utc::getHours12(void) const
	{
		ScePspDateTime today;
		sceRtcConvertTickToDateTime((SceRtcTick*)(&miTime), &today);
		return today.hour % 12;
	}

	//------------------------------------------------------------------------------
	xdatetime xtime_utc::getDate(void) const
	{
		ScePspDateTime today;
		sceRtcConvertTickToDateTime((SceRtcTick*)(&miTime), &today);
		xdatetime date(today.year, today.month, today.day);
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

#endif /// TARGET_WII