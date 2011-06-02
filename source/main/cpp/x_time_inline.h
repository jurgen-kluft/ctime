//------------------------------------------------------------------------------
inline
xtimer::xtimer(void)
:mStartTime(0)
,mTotalTime(0)
,mIsRunning(xFALSE)
,mNumSamples(0)
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
    mNumSamples++;
}

//------------------------------------------------------------------------------
inline
void xtimer::reset(void)
{
    mIsRunning  = xFALSE;
    mStartTime  = 0;
    mTotalTime  = 0;
    mNumSamples = 0;
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

        mNumSamples++;
    }
    else
    {
        ticks = 0;
    }

    return ticks;
}

//------------------------------------------------------------------------------
inline
s32 xtimer::getSampleCount(void) const
{
    return mNumSamples;
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
f64 xtimer::getAverageMs(void) const
{
    if(mNumSamples <= 0) 
        return 0 ;

    return readMs() / mNumSamples;
}
//------------------------------------------------------------------------------
//  Author:
//      Tomas Arce
//  Summary:
//      Convert a period of time's measurement from xtick to millisecond.
//  Arguments:
//      Ticks that have elapsed. 
//  Returns:
//      Milliseconds referring a period of time that is equal to the ticks passed in.
//  See Also:
//      x_GetTicksPerMs
//------------------------------------------------------------------------------
inline
f64 x_TicksToMs(xtick inTicks)
{
    return ((f64)inTicks)/x_GetTicksPerMs();
}
//------------------------------------------------------------------------------
//  Author:
//      Tomas Arce
//  Summary:
//      Convert a period of time's measurement from xtick to second.
//  Arguments:
//      Ticks that have elapsed. 
//  Returns:
//      Seconds referring a period of time that is equal to the ticks passed in.
//  See Also:
//      x_GetTicksPerSecond
//------------------------------------------------------------------------------
inline
f64 x_TicksToSec(xtick inTicks)
{
    return ((f64)inTicks)/x_GetTicksPerSecond();
}
//------------------------------------------------------------------------------
//  Author:
//      Tomas Arce
//  Summary:
//      Get elapsed time from timer initialized in second.
//  Arguments:
//      void 
//  Returns:
//      Seconds that have elapsed from x_TimeInit called
//  See Also:
//      x_GetTime x_TicksToSec
//------------------------------------------------------------------------------
inline
f64 x_GetTimeSec(void)
{
    return x_TicksToSec(x_GetTime());
}

//==============================================================================
// FUNCTIONS
//==============================================================================
void    x_TimeInit          (void);
void    x_TimeKill          (void);    

//==============================================================================
// END
//==============================================================================
