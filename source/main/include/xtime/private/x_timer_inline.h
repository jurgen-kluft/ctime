//------------------------------------------------------------------------------
inline timer_t::timer_t(void)
    : mStartTime(0), mTotalTime(0), mIsRunning(False), mNumTrips(0)
{
}

//------------------------------------------------------------------------------
inline void timer_t::start(void)
{
    if (mIsRunning)
        return;

    mStartTime = x_GetTime();
    mIsRunning = xTRUE;
    mNumTrips++;
}

//------------------------------------------------------------------------------
inline void timer_t::reset(void)
{
    mIsRunning = False;
    mStartTime = 0;
    mTotalTime = 0;
    mNumTrips = 0;
}

//------------------------------------------------------------------------------
inline tick_t timer_t::stop(void)
{
    if (mIsRunning)
    {
        mTotalTime += x_GetTime() - mStartTime;
        mIsRunning = False;
    }

    return mTotalTime;
}

//------------------------------------------------------------------------------
inline tick_t timer_t::read(void) const
{
    if (mIsRunning)
        return mTotalTime + (x_GetTime() - mStartTime);

    return mTotalTime;
}

//------------------------------------------------------------------------------
inline tick_t timer_t::trip(void)
{
    tick_t ticks;

    if (mIsRunning)
    {
        tick_t currentTime = x_GetTime();

        ticks = mTotalTime + (currentTime - mStartTime);

        mTotalTime = 0;
        mStartTime = currentTime;

        mNumTrips++;
    }
    else
    {
        ticks = 0;
    }

    return ticks;
}

//------------------------------------------------------------------------------
inline s32 timer_t::getNumTrips(void) const
{
    return mNumTrips;
}

//------------------------------------------------------------------------------
inline bool timer_t::isRunning(void) const
{
    return mIsRunning;
}

//------------------------------------------------------------------------------
inline f64 timer_t::tripSec(void)
{
    return x_TicksToSec(trip());
}
//------------------------------------------------------------------------------
inline f64 timer_t::tripMs(void)
{
    return x_TicksToMs(trip());
}
//------------------------------------------------------------------------------
inline f64 timer_t::tripUs(void)
{
    return x_TicksToUs(trip());
}
//------------------------------------------------------------------------------

inline f64 timer_t::readSec(void) const
{
    return x_TicksToSec(read());
}
//------------------------------------------------------------------------------
inline f64 timer_t::readMs(void) const
{
    return x_TicksToMs(read());
}
//------------------------------------------------------------------------------
inline f64 timer_t::readUs(void) const
{
    return x_TicksToUs(read());
}
//------------------------------------------------------------------------------
inline f64 timer_t::stopSec(void)
{
    return x_TicksToSec(stop());
}
//------------------------------------------------------------------------------
inline f64 timer_t::stopMs(void)
{
    return x_TicksToMs(stop());
}
//------------------------------------------------------------------------------
inline f64 timer_t::stopUs(void)
{
    return x_TicksToUs(stop());
}
//------------------------------------------------------------------------------
inline f64 timer_t::getAverageMs(void) const
{
    if (mNumTrips <= 0)
        return 0;

    return readMs() / mNumTrips;
}
