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

#include "xtime\private\x_time_source.h"
#include "xtime\private\x_datetime_source.h"

/**
 * xCore namespace
 */
namespace xcore
{
	class xdatetime_source_x360 : public xdatetime_source
	{
	public:
		virtual u64			getSystemTimeUtc()
		{
			struct ::tm* gmTime;
			::time_t rawtime;
			::time( &rawtime );
			gmTime = ::gmtime( &rawtime );

			xdatetime dt(gmTime->tm_year + 1900, gmTime->tm_mon + 1, gmTime->tm_mday, gmTime->tm_hour, gmTime->tm_min, gmTime->tm_sec);
			return (u64)dt.ticks();
		}

		// Time difference between local and UTC
		virtual s64			getSystemTimeZone()
		{
			struct tm *local_time, *gmt_time;
			time_t t = time(NULL);
			local_time = localtime(&t);
			s64 y = mktime(local_time);
			gmt_time = gmtime(&t);
			s64 x = mktime(gmt_time);       
			s64 tzone_diff = y - x;
			return tzone_diff;
		}

		virtual u64			getSystemTimeLocal()
		{
			struct ::tm localTime;
			::time_t rawtime;
			::time( &rawtime );
			::localtime_s( &localTime, &rawtime );

			xdatetime dt(localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
			return (u64)dt.ticks();
		}

		virtual u64			getSystemTimeAsFileTime()
		{
			::FILETIME system_time;
			::GetSystemTimeAsFileTime(&system_time);
			u64 time = ((u64)system_time.dwHighDateTime << 32) + (u64)system_time.dwLowDateTime;
			return time;
		}

		virtual u64			getSystemTimeFromFileTime(u64 inFileSystemTime)
		{
			u64 systemTime = getSystemTime();
			u64 fileTime = getSystemTimeAsFileTime();
			return inFileSystemTime - (fileTime - systemTime);
		}

		virtual u64			getFileTimeFromSystemTime(u64 inSystemTime)
		{
			u64 systemTime = getSystemTime();
			u64 fileTime = getSystemTimeAsFileTime();
			return inSystemTime + (fileTime - systemTime);
		}
	};

	/**
     * ------------------------------------------------------------------------------
	 *   Author:
	 *       Virtuos
	 *   Summary:
	 *       Set sPCFreqPerMs,sPCFreqPerSec,sBaseTimeTick 's values, which are processor
	 *       dependent.
	 *   Arguments:
	 *       void
	 *   Returns:
	 *       void
	 *   Description:
	 *       Local variable counter is used to refer to an incrementing variable,
	 *       and represents the current value of the high-resolution performance counter.
	 *       Often used to compute the elapsed counters while a section of code executed.
	 *       sBaseTimeTick is the start counter of the timer.
	 * 
	 * <P>   sPCFreqPerSec and sPCFreqPerMs both are frequency of the counter above, but
	 *       with different units per second and permilliseconde.Their values are 
	 *       processor dependent.On some processors, for example, their values might be
	 *       the cycle rate of the processor clock. So there is a local variable named
	 *       clockFreq.
	 *   See Also:
	 *       QueryPerformanceCounter QueryPerformanceFrequency
	 * ------------------------------------------------------------------------------
	 */
	class xtime_source_x360 : public xtime_source
	{
		f64				mPCFreqPerSec;
		xtick			mBaseTimeTick;
		xtick			mLastTicks;

	public:
		void			init()
		{
			LARGE_INTEGER counter;
			LARGE_INTEGER clockFreq;

			QueryPerformanceCounter(&counter);
			QueryPerformanceFrequency(&clockFreq); 

			mPCFreqPerSec   = (f64)clockFreq.QuadPart;
			mBaseTimeTick   = (xtick)counter.QuadPart;
			mLastTicks		= 0;
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
			ASSERT(mBaseTimeTick != 0);

			LARGE_INTEGER   counter;
			QueryPerformanceCounter(&counter);
			xtick ticks = (xtick)(counter.QuadPart) - mBaseTimeTick;

			// Try to help the PC bug
			if (ticks < mLastTicks)     
				ticks = mLastTicks + 1;
			mLastTicks = ticks;
			return ticks;
		}

		virtual s64		getTicksPerSecond()
		{
			return (s64)mPCFreqPerSec;
		}
	};

	/**
	 * xtime, Init and Exit
	 */
	void x_TimeInit(void)
	{
		static xtime_source_x360 sTimeSource;
		sTimeSource.init();
		x_SetTimeSource(&sTimeSource);

		static xdatetime_source_x360 sDateTimeSource;
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

#endif /// TARGET_360
