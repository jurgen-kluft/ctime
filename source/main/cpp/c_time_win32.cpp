#include "cbase/c_target.h"
#ifdef TARGET_PC

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOMB
#define NOKANJI

#include <windows.h>
#include <mmsystem.h>
#include <time.h>

#include "cbase/c_debug.h"

#include "ctime/c_time.h"
#include "ctime/c_timespan.h"
#include "ctime/c_datetime.h"

#include "ctime/private/c_time_source.h"
#include "ctime/private/c_datetime_source.h"

namespace ncore
{
	class datetime_source_win32 : public datetime_source_t
	{
	public:
		virtual u64			getSystemTimeUtc()
		{
			::time_t rawtime;
			::time( &rawtime );

			::tm gmTime;
			gmtime_s(&gmTime, &rawtime);

			datetime_t dt(gmTime.tm_year + 1900, gmTime.tm_mon + 1, gmTime.tm_mday, gmTime.tm_hour, gmTime.tm_min, gmTime.tm_sec);
			return (u64)dt.ticks();
		}

		// Time difference between local and UTC
		virtual s64			getSystemTimeZone()
		{
			TIME_ZONE_INFORMATION tzi;
			GetTimeZoneInformation(&tzi);
			return tzi.Bias;
		}

		virtual u64			getSystemTimeLocal()
		{
			SYSTEMTIME st;
			//GetSystemTime(&st);
			GetLocalTime(&st);
			datetime_t dt(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
#if 0
			::time_t rawtime;
			::time( &rawtime );
			::tm localTime;
			::localtime_s( &localTime, &rawtime );
			datetime_t dt(localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
#endif
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
			u64 systemTime = getSystemTimeLocal();
			u64 fileTime = getSystemTimeAsFileTime();
			return inFileSystemTime - (fileTime - systemTime);
		}

		virtual u64			getFileTimeFromSystemTime(u64 inSystemTime)
		{
			u64 systemTime = getSystemTimeLocal();
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
	class time_source_win32 : public time_source_t
	{
		f64				mPCFreqPerSec;
		tick_t			mBaseTimeTick;
		tick_t			mLastTicks;

	public:
		void			init()
		{
			LARGE_INTEGER counter;
			LARGE_INTEGER clockFreq;

			QueryPerformanceCounter(&counter);
			QueryPerformanceFrequency(&clockFreq); 

			mPCFreqPerSec   = (f64)clockFreq.QuadPart;
			mBaseTimeTick   = (tick_t)counter.QuadPart;
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
		virtual tick_t	getTimeInTicks()
		{
			ASSERT(mBaseTimeTick != 0);

			LARGE_INTEGER   counter;
			QueryPerformanceCounter(&counter);
			tick_t ticks = (tick_t)(counter.QuadPart) - mBaseTimeTick;

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

	namespace ctime
	{
		void init(void)
		{
			static ncore::time_source_win32 sTimeSource;
			sTimeSource.init();
			ncore::g_SetTimeSource(&sTimeSource);

			static ncore::datetime_source_win32 sDateTimeSource;
			ncore::g_SetDateTimeSource(&sDateTimeSource);
		}

		void exit(void)
		{
			ncore::g_SetTimeSource(nullptr);
			ncore::g_SetDateTimeSource(nullptr);
		}
	}

};


#endif // TARGET_PC
