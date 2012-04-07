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

#include "xtime\private\x_time_source.h"
#include "xtime\private\x_datetime_source.h"

/**
 * xCore namespace
 */
namespace xcore
{
	class xdatetime_source_wii : public xdatetime_source
	{
		xdatetime	GetDateTimeNow()
		{
			OSCalendarTime ct;

			OSTime time = OSGetTime();
			OSTicksToCalendarTime(time, &ct);

			xdatetime xdt(1900 + ct.year, ct.mon + 1, ct.mday, ct.hour, ct.min, ct.sec);
			return xdt;
		}

	public:
		virtual u64			getSystemTimeUtc()
		{
			xdatetime dt = GetDateTimeNow();
			return (u64)dt.ticks();
		}

		virtual u64			getSystemTimeLocal()
		{
			xdatetime dt = GetDateTimeNow();
			return (u64)dt.ticks();
		}

		virtual s64			getSystemTimeZone()
		{
			// NOT implemented yet
			return 0;
		}

		virtual u64			getSystemTimeAsFileTime()
		{
			return getSystemTime();
		}

		virtual u64			getSystemTimeFromFileTime(u64 inFileSystemTime)
		{
			return inFileSystemTime;
		}

		virtual u64			getFileTimeFromSystemTime(u64 inSystemTime)
		{
			return inSystemTime;
		}
	};


    /**
     * Time source for the Nintendo WII
     */
	class xtime_source_wii : public xtime_source
	{
		f64				mFreqPerSec;
		xtick			mBaseTimeTick;
		xtick			mLastTicks;

	public:
		void			init()
		{
			mFreqPerSec   = OS_TIMER_CLOCK;
			mBaseTimeTick = (s64)OSGetTime();
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
		 *       Ticks that have elapsed from x_TimeInit called
		 *   Description:
		 *       use xcritical_section to make PerformanceCounter owned by only one thread
		 *       at a time.
		 *   See Also:
		 *       xcritical_section
		 * ------------------------------------------------------------------------------
		 */
		virtual xtick	getTimeInTicks()
		{
			ASSERT(mBaseTimeTick);
			s64 ticks = OSGetTime() - mBaseTimeTick;
			return ticks;
		}

		virtual s64		getTicksPerSecond()
		{
			return mFreqPerSec;
		}
	};

	/**
	 * xtime, Init and Exit
	 */
	void x_TimeInit(void)
	{
		static xtime_source_wii sTimeSource;
		sTimeSource.init();
		x_SetTimeSource(&sTimeSource);

		static xdatetime_source_wii sDateTimeSource;
		x_SetDateTimeSource(&sDateTimeSource);
	}

	void x_TimeExit(void)
	{
		x_SetTimeSource(NULL);
		x_SetDateTimeSource(NULL);
	}


    //==============================================================================
    // END xCore namespace
    //==============================================================================
};

#endif /// TARGET_WII
