#include "xbase\x_target.h"
#ifdef TARGET_3DS

//==============================================================================
// INCLUDES
//==============================================================================
#include <nn/os/os_Tick.h>
#include <nn/fnd/fnd_DateTime.h>

#include "xbase\x_types.h"
#include "xbase\x_debug.h"
#include "xbase\x_string_std.h"

#include "xtime\x_time.h"
#include "xtime\x_timespan.h"
#include "xtime\x_datetime.h"

#include "xtime\private\x_time_source.h"
#include "xtime\private\x_datetime_source.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class xdatetime_source_3ds : public xdatetime_source
	{
		xdatetime	GetDateTimeNow()
		{
			nn::fnd::DateTime dt = nn::fnd::DateTime::GetNow();
			xdatetime xdt(dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMilliSecond());
			return xdt;
		}

	public:
		virtual u64			getSystemTime()
		{
			xdatetime dt = GetDateTimeNow();
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

	//==============================================================================
	// Time source for the Nintendo 3DS
	//==============================================================================
	class xtime_source_3ds : public xtime_source
	{
		s64              m3dsFreqPerSec;
		s64              m3dsFreqPerMs;
		xtick            mBaseTimeTick;

	public:
		void			init()
		{
			m3dsFreqPerSec  = NN_HW_TICKS_PER_SECOND;
			m3dsFreqPerMs   = NN_HW_TICKS_PER_SECOND / 1000;
			mBaseTimeTick   = (s64)nn::os::Tick::GetSystemCurrent();
			if (mBaseTimeTick==0)	
				mBaseTimeTick = 1;
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
		virtual xtick	getTimeInTicks()
		{
			ASSERT(mBaseTimeTick);
			s64 ticks = (s64)nn::os::Tick::GetSystemCurrent() - mBaseTimeTick;
			return ticks;
		}

		virtual s64		getTicksPerMilliSecond()
		{
			return (s64)m3dsFreqPerMs;
		}

		virtual s64		getTicksPerSecond()
		{
			return (s64)m3dsFreqPerSec;
		}
	};

	//------------------------------------------------------------------------------
	// xtime, Init and Exit
	//------------------------------------------------------------------------------
	void x_TimeInit(void)
	{
		static xtime_source_3ds sTimeSource;
		sTimeSource.init();
		x_SetTimeSource(&sTimeSource);

		static xdatetime_source_3ds sDateTimeSource;
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

#endif /// TARGET_3DS
