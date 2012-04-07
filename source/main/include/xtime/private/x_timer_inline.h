//------------------------------------------------------------------------------
inline
xtimer::xtimer(void)
:mStartTime(0)
,mTotalTime(0)
,mIsRunning(xFALSE)
,mNumTrips(0)
{
}

//------------------------------------------------------------------------------
inline
void xtimer::start(void)
{
    if (mIsRunning)
        return;

    mStartTime = x_GetTime();
    mIsRunning = xTRUE;        
    mNumTrips++;
}

//------------------------------------------------------------------------------
inline
void xtimer::reset(void)
{
    mIsRunning  = xFALSE;
    mStartTime  = 0;
    mTotalTime  = 0;
    mNumTrips = 0;
}

//------------------------------------------------------------------------------
inline
xtick xtimer::stop(void)
{
    if(mIsRunning)
    {
        mTotalTime += x_GetTime() - mStartTime;
        mIsRunning  = xFALSE;
    }

    return mTotalTime;
}

//------------------------------------------------------------------------------
inline
xtick xtimer::read(void) const
{
    if(mIsRunning) 
        return mTotalTime + (x_GetTime() - mStartTime);

    return mTotalTime;
}

//------------------------------------------------------------------------------
inline
xtick xtimer::trip(void)
{
    xtick ticks;

    if(mIsRunning)
    {
        xtick currentTime = x_GetTime();

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
inline
s32 xtimer::getNumTrips(void) const
{
    return mNumTrips;
}

//------------------------------------------------------------------------------
inline
xbool xtimer::isRunning(void) const
{
    return mIsRunning; 
} 

//------------------------------------------------------------------------------
inline
f64 xtimer::tripSec(void)
{
    return x_TicksToSec(trip());
}
//------------------------------------------------------------------------------
inline
f64 xtimer::tripMs(void)
{
    return x_TicksToMs(trip());
}
//------------------------------------------------------------------------------
inline
f64 xtimer::tripUs(void)
{
    return x_TicksToUs(trip());
}
//------------------------------------------------------------------------------

inline
f64 xtimer::readSec(void) const
{
    return x_TicksToSec(read());
}
//------------------------------------------------------------------------------
inline
f64 xtimer::readMs(void) const
{
    return x_TicksToMs(read());
}
//------------------------------------------------------------------------------
inline
f64 xtimer::readUs(void) const
{
    return x_TicksToUs(read());
}
//------------------------------------------------------------------------------
inline
f64 xtimer::stopSec(void)
{
    return x_TicksToSec(stop());
}
//------------------------------------------------------------------------------
inline
f64 xtimer::stopMs(void)
{
    return x_TicksToMs(stop());
}
//------------------------------------------------------------------------------
inline
f64 xtimer::stopUs(void)
{
    return x_TicksToUs(stop());
}
//------------------------------------------------------------------------------
inline
f64 xtimer::getAverageMs(void) const
{
    if(mNumTrips <= 0) 
        return 0 ;

    return readMs() / mNumTrips;
}
