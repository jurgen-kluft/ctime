//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_types.h"
#include "xbase\x_debug.h"
#include "xbase\x_string_std.h"

#include "xtime\x_time.h"
#include "xtime\x_frame_rate.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	xframerate::xframerate()
		: m_fFrameRate(0.0f)
		, m_dwSecondCount(0)
		, m_dwNumFrames(0.0f)
	{
		m_tLastFPSTime = x_GetTime();
	}
	
	void		xframerate::restart()
	{
		m_fFrameRate = 0.0f;
		m_dwSecondCount = 0;
		m_dwNumFrames = 0.0f;
		m_tLastFPSTime = x_GetTime();
	}

	void		xframerate::markFrame()
	{
		m_dwNumFrames += 1.0f;

		xtick tTime = x_GetTime();

		// Only re-compute the FPS (frames per second) once per second
		if ( (tTime - m_tLastFPSTime) >= x_GetTicksPerSecond() )
		{
			m_fFrameRate = (f32)(m_dwNumFrames * x_GetTicksPerSecond()) / (f32)( tTime - m_tLastFPSTime );
			m_tLastFPSTime = tTime;
			m_dwNumFrames = 0.0f;
			m_dwSecondCount++;
		}
	}

	bool		xframerate::getFrameRate(f32& fps) const
	{
		fps = m_fFrameRate;
		return (m_dwSecondCount > 0);
	}

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
