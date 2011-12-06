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

#include "xtime\private\x_time_source.h"
#include "xtime\private\x_datetime_source.h"

/**
 * xCore namespace
 */
namespace xcore
{
	class xdatetime_source_psp : public xdatetime_source
	{
	public:
		virtual u64			getSystemTime()
		{
			ScePspDateTime localTime;
			sceRtcGetCurrentClockLocalTime(&localTime);

			xdatetime dt(1900 + localTime.year, localTime.month, localTime.day, localTime.hour, localTime.minute, localTime.second);
			return (u64)dt.ticks();
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
	 * Time source for the Sony PSP
	 */
	class xtime_source_psp : public xtime_source
	{
		f32				mFreqPerSec;
		f32				mFreqPerMs;
		xtick			mBaseTimeTick;
		xtick			mLastTicks;

	public:
		xtime_source_psp()
			: mFreqPerSec(0)
			, mFreqPerMs(0)
			, mBaseTimeTick(0)
			, mLastTicks(0)
		{

		}

		void			init()
		{
			const u64		PSP_FREQ_PER_SEC = 1000000;

			mFreqPerSec  = PSP_FREQ_PER_SEC;
			mFreqPerMs   = PSP_FREQ_PER_SEC / 1000.0f;
			mBaseTimeTick= sceKernelGetSystemTimeWide();
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
			s64 ticks = sceKernelGetSystemTimeWide() - mBaseTimeTick;
			return ticks;
		}

		virtual s64		getTicksPerMilliSecond()
		{
			return mFreqPerMs;
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
		static xtime_source_psp sTimeSource;
		sTimeSource.init();
		x_SetTimeSource(&sTimeSource);

		static xdatetime_source_psp sDateTimeSource;
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

#endif /// TARGET_PSP
