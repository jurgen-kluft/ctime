#include "xbase\x_target.h"
#ifdef TARGET_PS3

//==============================================================================
// INCLUDES
//==============================================================================
#include <time.h>
#include <sys/sys_time.h>

#include "xbase\x_types.h"
#include "xbase\x_debug.h"
#include "xbase\x_string_std.h"

#include "xtime\x_time.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	u64		xdatetime::sGetSystemTime()
	{
		time_t	curTime;
		time(&curTime);
		tm		localTime = (*localtime(&curTime));
		
		xdatetime dt(1900 + localTime.tm_year, localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
		return (u64)dt.ticks();
	}

	// ps3 has no functionality to get the file time
	u64		xdatetime::sGetSystemTimeAsFileTime()
	{
		u64 systemTime = sGetSystemTime();
		return sGetFileTimeFromSystemTime(systemTime);
	}

	u64		xdatetime::sGetSystemTimeFromFileTime(u64 inTime)
	{
		u32 second, minute, hour, year, month, day;

		second	= inTime % 60;
		inTime	/= 60;
		minute	= inTime % 60;
		inTime	/= 60;
		hour	= inTime % 24;
		inTime	/= 24;

		u32	uYear	= 1970;
		u32	uMonth	= 0;
		u32	uDay	= 0;

		while (xTRUE)
		{
			xbool	boLeap			= false;
			u32		uNumDaysInYear	= 365;

			if((uYear % 4) == 0)
			{
				uNumDaysInYear	= 366;
				boLeap			= true;
			}

			if(inTime >= uNumDaysInYear)
			{
				inTime	-= uNumDaysInYear;
				uYear++;
			}
			else
			{
				for(u32 uI = 0; uI < 12; uI++)
				{
					u32	uNumDaysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

					if(uI == 1)
					{
						if(boLeap)
						{
							uNumDaysPerMonth[uI] = 29;
						}
						else
						{
							uNumDaysPerMonth[uI] = 28;
						}
					}

					if(inTime >= uNumDaysPerMonth[uI])
					{
						inTime -= uNumDaysPerMonth[uI];
						uMonth++;
					}
					else
					{
						uDay	= inTime;
						break;
					}
				}

				break;
			}
		}

		day		= uDay + 1;
		month	= uMonth;
		year	= uYear;

		xdatetime dt = xdatetime(year, month, day, hour, minute, second);
		return dt.ticks();
	}

	u64		xdatetime::sGetFileTimeFromSystemTime(u64 inSystemTime)
	{
		return inSystemTime;
	}

    //==============================================================================
    // VARIABLES
    //==============================================================================
    static s64              sPs3FreqPerSec = 0;
    static s64              sPs3FreqPerMs = 0;
    static xtick            sBaseTimeTick = 0;

    //==============================================================================
    // Functions
    //==============================================================================

    //------------------------------------------------------------------------------
    void x_TimeInit(void)
    {
        sPs3FreqPerSec  = CLOCKS_PER_SEC;
        sPs3FreqPerMs   = sPs3FreqPerSec / 1000;
		sBaseTimeTick = clock();
    }

    //------------------------------------------------------------------------------
    void x_TimeKill(void)
    {
    }

    //------------------------------------------------------------------------------
    s64 x_GetTicksPerSecond(void)
    {
        return sPs3FreqPerSec;
    }

    //------------------------------------------------------------------------------

    s64 x_GetTicksPerMs(void)
    {
        return sPs3FreqPerMs;
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

        s64 ticks;
		
		ticks = clock();
		ticks -= sBaseTimeTick;

        return ticks;
    }

    //------------------------------------------------------------------------------
    void xtime_utc::updateTime (void)
    {
        time((time_t*)(&miTime));
    }
    //------------------------------------------------------------------------------
    void xtime_utc::clear (void)
    {
        miTime = 0;
    }

    //------------------------------------------------------------------------------
    s32 xtime_utc::getSeconds(void) const
    {
        tm today = *(localtime((time_t*)(&miTime)));
        return today.tm_sec;
    }

    //------------------------------------------------------------------------------
    s32 xtime_utc::getMinutes(void) const
	{
		tm today = *(localtime((time_t*)(&miTime)));
		return today.tm_min;
    }

    //------------------------------------------------------------------------------
    s32 xtime_utc::getHours24(void) const
	{
		tm today = *(localtime((time_t*)(&miTime)));
		return today.tm_hour;
    }

    //------------------------------------------------------------------------------
    s32 xtime_utc::getHours12(void) const
	{
		tm today = *(localtime((time_t*)(&miTime)));
		return (today.tm_hour % 12);
    }

    //------------------------------------------------------------------------------
    xdatetime xtime_utc::getDate(void) const
	{
		tm today = *(localtime((time_t*)(&miTime)));
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

#endif /// TARGET_WII