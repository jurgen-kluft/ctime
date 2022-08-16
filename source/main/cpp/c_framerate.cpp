#include "cbase/c_debug.h"

#include "ctime/c_time.h"
#include "ctime/c_frame_rate.h"

namespace ncore
{
	framerate_t::framerate_t()
		: m_fFrameRate(0.0f)
		, m_dwSecondCount(0)
		, m_dwNumFrames(0.0f)
	{
		m_tLastFPSTime = x_GetTime();
	}

	void		framerate_t::restart()
	{
		m_fFrameRate = 0.0f;
		m_dwSecondCount = 0;
		m_dwNumFrames = 0.0f;
		m_tLastFPSTime = x_GetTime();
	}

	void		framerate_t::markFrame()
	{
		m_dwNumFrames += 1.0f;

		tick_t tTime = x_GetTime();

		// Only re-compute the FPS (frames per second) once per second
		if ( (tTime - m_tLastFPSTime) >= x_GetTicksPerSecond() )
		{
			m_fFrameRate = (f32)(m_dwNumFrames * x_GetTicksPerSecond()) / (f32)( tTime - m_tLastFPSTime );
			m_tLastFPSTime = tTime;
			m_dwNumFrames = 0.0f;
			m_dwSecondCount++;
		}
	}

	bool		framerate_t::getFrameRate(f32& fps) const
	{
		fps = m_fFrameRate;
		return (m_dwSecondCount > 0);
	}

};
