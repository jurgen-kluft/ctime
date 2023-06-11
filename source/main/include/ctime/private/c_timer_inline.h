//------------------------------------------------------------------------------
inline timer_t::timer_t(void)
    : mStartTime(0), mTotalTime(0), mIsRunning(false), mNumTrips(0)
{
}

//------------------------------------------------------------------------------
inline void timer_t::start(void)
{
    if (mIsRunning)
        return;

    mStartTime = getTime();
    mIsRunning = true;
    mNumTrips++;
}

//------------------------------------------------------------------------------
inline void timer_t::reset(void)
{
    mIsRunning = false;
    mStartTime = 0;
    mTotalTime = 0;
    mNumTrips = 0;
}

//------------------------------------------------------------------------------
inline tick_t timer_t::stop(void)
{
    if (mIsRunning)
    {
        mTotalTime += getTime() - mStartTime;
        mIsRunning = false;
    }

    return mTotalTime;
}

//------------------------------------------------------------------------------
inline tick_t timer_t::read(void) const
{
    if (mIsRunning)
        return mTotalTime + (getTime() - mStartTime);

    return mTotalTime;
}

//------------------------------------------------------------------------------
inline tick_t timer_t::trip(void)
{
    tick_t ticks;

    if (mIsRunning)
    {
        tick_t currentTime = getTime();

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
    return ticksToSec(trip());
}
//------------------------------------------------------------------------------
inline f64 timer_t::tripMs(void)
{
    return ticksToMs(trip());
}
//------------------------------------------------------------------------------
inline f64 timer_t::tripUs(void)
{
    return ticksToUs(trip());
}
//------------------------------------------------------------------------------

inline f64 timer_t::readSec(void) const
{
    return ticksToSec(read());
}
//------------------------------------------------------------------------------
inline f64 timer_t::readMs(void) const
{
    return ticksToMs(read());
}
//------------------------------------------------------------------------------
inline f64 timer_t::readUs(void) const
{
    return ticksToUs(read());
}
//------------------------------------------------------------------------------
inline f64 timer_t::stopSec(void)
{
    return ticksToSec(stop());
}
//------------------------------------------------------------------------------
inline f64 timer_t::stopMs(void)
{
    return ticksToMs(stop());
}
//------------------------------------------------------------------------------
inline f64 timer_t::stopUs(void)
{
    return ticksToUs(stop());
}
//------------------------------------------------------------------------------
inline f64 timer_t::getAverageMs(void) const
{
    if (mNumTrips <= 0)
        return 0;

    return readMs() / mNumTrips;
}
