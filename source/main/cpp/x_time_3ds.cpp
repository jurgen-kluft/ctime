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


//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	static xdatetime sGetDateTimeNow()
	{
		nn::fnd::DateTime dt = nn::fnd::DateTime::GetNow();
		xdatetime xdt(dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond(), dt.GetMilliSecond());
		return xdt;
	}

	u64		xdatetime::sGetSystemTime()
	{
		xdatetime xdt = sGetDateTimeNow();
		return (u64)xdt.ticks();
	}

	// 3DS has no functionality to get the file time
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


    //==============================================================================
    // Functions
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
    void x_TimeInit(void)
    {
		static xtime_source_3ds sTimeSource;
		sTimeSource.init();
		x_SetTimeSource(&sTimeSource);
    }

    //------------------------------------------------------------------------------
    void x_TimeExit(void)
    {
		x_SetTimeSource(NULL);
    }



    //==============================================================================
    // END xCore namespace
    //==============================================================================
};

#endif /// TARGET_3DS
