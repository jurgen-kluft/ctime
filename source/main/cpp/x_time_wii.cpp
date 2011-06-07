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
	u64		xdatetime::sGetSystemTime()
	{
		OSCalendarTime ct;

		OSTime time = OSGetTime();
		OSTicksToCalendarTime(time, &ct);

		xdatetime dt(1900 + ct.year, ct.mon + 1, ct.mday, ct.hour, ct.min, ct.sec);
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

    //------------------------------------------------------------------------------
    void xtime_utc::updateTime (void)
    {
        miTime = (s64)OSGetTime();
    }
    //------------------------------------------------------------------------------
    void xtime_utc::clear (void)
    {
        miTime = 0;
    }

    //------------------------------------------------------------------------------
    s32 xtime_utc::getSeconds(void) const
    {
        struct OSCalendarTime today;
        OSTicksToCalendarTime((s64)miTime, &today);
        return today.sec;
    }

    //------------------------------------------------------------------------------
    s32 xtime_utc::getMinutes(void) const
    {
        struct OSCalendarTime today;
        OSTicksToCalendarTime((s64)miTime, &today);
        return today.min;
    }

    //------------------------------------------------------------------------------
    s32 xtime_utc::getHours24(void) const
    {
        struct OSCalendarTime today;
        OSTicksToCalendarTime((s64)miTime, &today);
        return today.hour;
    }

    //------------------------------------------------------------------------------
    s32 xtime_utc::getHours12(void) const
    {
        struct OSCalendarTime today;
        OSTicksToCalendarTime((s64)miTime, &today);
        return (today.hour % 12);
    }

    //------------------------------------------------------------------------------
    xdatetime xtime_utc::getDate(void) const
    {
        struct OSCalendarTime today;
        OSTicksToCalendarTime((s64)miTime, &today);
        xdatetime date(today.year, today.mon, today.mday);
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