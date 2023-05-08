#ifndef __CTIME_FRAME_RATE_H__
#define __CTIME_FRAME_RATE_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
    typedef s64 tick_t;

    /**
     * ------------------------------------------------------------------------------
     *  Description:
     *      The framerate_t class is a handy way to measure the frame-rate of an application.
     *      The frame rate is computed as a number of frames per second and this class computes
     *      the frame rate every second.
     *
     *  Example:
     * <CODE>
     *       void main(void)
     *       {
     *           framerate_t fps;
     *
     *           while (game_loop)
     *           {
     *                *  Game logic
     * 				fps.markFrame();
     * 				 *  Game rendering
     * 				 *  Display frame rate
     * 				f32 frame_rate;
     * 				if (fps.getFrameRate(frame_rate))
     * 					x_printf("Frame rate = %0.02f fps", frame_rate);
     *           }
     *       }
     * </CODE>
     * ------------------------------------------------------------------------------
     */
    class framerate_t
    {
    public:
        framerate_t();

        void restart();

        void markFrame();
        bool getFrameRate(f32& fps) const; ///< Return true when the frame-rate is up-to-date

    private:
        f32    m_fFrameRate;
        u64    m_dwSecondCount;
        f32    m_dwNumFrames;
        tick_t m_tLastFPSTime;
    };

}; // namespace ncore

#endif
