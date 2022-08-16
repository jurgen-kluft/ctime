#include "xbase/x_debug.h"

#include "xtime/x_time.h"
#include "xtime/x_timespan.h"

namespace ncore
{
    const u64 timespan_t::sTicksPerDay         = X_CONSTANT_64(864000000000);
    const u64 timespan_t::sTicksPerHour        = X_CONSTANT_64(36000000000);
    const u64 timespan_t::sTicksPerMillisecond = 10000;
    const u64 timespan_t::sTicksPerMinute      = 600000000;
    const u64 timespan_t::sTicksPerSecond      = 10000000;

    const s32 timespan_t::sMillisPerDay    = 86400000;
    const s32 timespan_t::sMillisPerHour   = 3600000;
    const s32 timespan_t::sMillisPerMinute = 60000;
    const s32 timespan_t::sMillisPerSecond = 1000;

    const timespan_t timespan_t::sMaxValue(X_CONSTANT_64(0x2bca2875f4373fff));
    const timespan_t timespan_t::sMinValue(0);
    const timespan_t timespan_t::sZero(0);

    const s64 MaxMilliSeconds = X_CONSTANT_64(922337203685477);
    const s64 MinMilliSeconds = X_CONSTANT_64(-922337203685477);

#ifdef D_ASSERT
    const s64 MaxSeconds = X_CONSTANT_64(922337203685);
    const s64 MinSeconds = X_CONSTANT_64(-922337203685);
#endif

    static timespan_t sInterval(u64 value, s32 scale)
    {
        s64 num = value * scale;
        ASSERTS((num <= MaxMilliSeconds) && (num >= MinMilliSeconds), "timespan_t::Overflow::TooLong");
        return timespan_t(((s64)num) * timespan_t::sTicksPerMillisecond);
    }

    /**
     *  Summary:
     *      Initializes a new System.timespan_t to the specified number of ticks.
     *
     *  Parameters:
     *    ticks:
     *      A time period expressed in 100-nanosecond units.
     */
    timespan_t::timespan_t(u64 ticks)
        : mTicks(ticks)
    {
    }

    /**
     *  Summary:
     *      Initializes a new System.timespan_t to a specified number of hours, minutes,
     *      and seconds.
     *
     *  Parameters:
     *    seconds:
     *      Number of seconds.
     *
     *    hours:
     *      Number of hours.
     *
     *    minutes:
     *      Number of minutes.
     */
    timespan_t::timespan_t(s32 hours, s32 minutes, s32 seconds) { mTicks = sTimeToTicks(hours, minutes, seconds); }

    /**
     *  Summary:
     *      Initializes a new System.timespan_t to a specified number of days, hours, minutes,
     *      and seconds.
     *
     *  Parameters:
     *    seconds:
     *      Number of seconds.
     *
     *    hours:
     *      Number of hours.
     *
     *    minutes:
     *      Number of minutes.
     *
     *    days:
     *      Number of days.
     */
    timespan_t::timespan_t(s32 days, s32 hours, s32 minutes, s32 seconds) { mTicks = sTimeToTicks(days, hours, minutes, seconds, 0); }

    /**
     *  Summary:
     *      Initializes a new System.timespan_t to a specified number of days, hours, minutes,
     *      seconds, and milliseconds.
     *
     *  Parameters:
     *    seconds:
     *      Number of seconds.
     *
     *    hours:
     *      Number of hours.
     *
     *    minutes:
     *      Number of minutes.
     *
     *    days:
     *      Number of days.
     *
     *    milliseconds:
     *      Number of milliseconds.
     */
    timespan_t::timespan_t(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds) { mTicks = sTimeToTicks(days, hours, minutes, seconds, milliseconds); }

    /**
     *  Summary:
     *      Gets the number of whole days represented by the current System.timespan_t
     *      structure.
     *
     *  Returns:
     *      The day component of this instance. The return value can be positive or negative.
     */
    s32 timespan_t::days() const { return (s32)(mTicks / sTicksPerDay); }

    /**
     *  Summary:
     *      Gets the number of whole hours represented by the current System.timespan_t
     *      structure.
     *
     *  Returns:
     *      The hour component of the current System.timespan_t structure. The return value
     *      ranges from -23 through 23.
     */
    s32 timespan_t::hours() const { return (s32)((mTicks / sTicksPerHour) % ((s64)0x18)); }

    /**
     *  Summary:
     *      Gets the number of whole milliseconds represented by the current System.timespan_t
     *      structure.
     *
     *  Returns:
     *      The millisecond component of the current System.timespan_t structure. The return
     *      value ranges from -999 through 999.
     */
    s32 timespan_t::milliseconds() const { return (s32)((mTicks / ((s64)sTicksPerMillisecond)) % ((s64)sMillisPerSecond)); }

    /**
     *  Summary:
     *      Gets the number of whole minutes represented by the current System.timespan_t
     *      structure.
     *
     *  Returns:
     *      The minute component of the current System.timespan_t structure. The return
     *      value ranges from -59 through 59.
     */
    s32 timespan_t::minutes() const { return (s32)((mTicks / ((s64)sTicksPerMinute)) % ((s64)60)); }

    /**
     *  Summary:
     *      Gets the number of whole seconds represented by the current System.timespan_t
     *      structure.
     *
     *  Returns:
     *      The second component of the current System.timespan_t structure. The return
     *      value ranges from -59 through 59.
     */
    s32 timespan_t::seconds() const { return (s32)((mTicks / ((s64)sTicksPerSecond)) % ((s64)60)); }

    /**
     *  Summary:
     *      Gets the number of ticks that represent the value of the current System.timespan_t
     *      structure.
     *
     *  Returns:
     *      The number of ticks contained in this instance.
     */
    u64 timespan_t::ticks() const { return mTicks; }

    /**
     *  Summary:
     *      Gets the value of the current System.timespan_t structure expressed in whole
     *      and fractional days.
     *
     *  Returns:
     *      The total number of days represented by this instance.
     */
    u64 timespan_t::totalDays() const { return (mTicks / sTicksPerDay); }

    /**
     *  Summary:
     *      Gets the value of the current System.timespan_t structure expressed in whole
     *      and fractional hours.
     *
     *  Returns:
     *      The total number of hours represented by this instance.
     */
    u64 timespan_t::totalHours() const { return (mTicks / sTicksPerHour); }

    /**
     *  Summary:
     *      Gets the value of the current System.timespan_t structure expressed in whole
     *      and fractional milliseconds.
     *
     *  Returns:
     *      The total number of milliseconds represented by this instance.
     */
    u64 timespan_t::totalMilliseconds() const
    {
        s64 num = mTicks / sTicksPerMillisecond;
        if (num > MaxMilliSeconds)
        {
            return MaxMilliSeconds;
        }
        if (num < MinMilliSeconds)
        {
            return (u64)MinMilliSeconds;
        }
        return num;
    }

    /**
     *  Summary:
     *      Gets the value of the current System.timespan_t structure expressed in whole
     *      and fractional minutes.
     *
     *  Returns:
     *      The total number of minutes represented by this instance.
     */
    u64 timespan_t::totalMinutes() const { return (mTicks / sTicksPerMinute); }

    /**
     *  Summary:
     *      Gets the value of the current System.timespan_t structure expressed in whole
     *      and fractional seconds.
     *
     *  Returns:
     *      The total number of seconds represented by this instance.
     */
    u64 timespan_t::totalSeconds() const { return (mTicks / sTicksPerSecond); }

    /**
     *  Summary:
     *      Adds the specified System.timespan_t to this instance.
     *
     *  Parameters:
     *    ts:
     *      A System.timespan_t.
     *
     *  Returns:
     *      A System.timespan_t that represents the value of this instance plus the value
     *      of ts.
     */
    timespan_t& timespan_t::add(const timespan_t& ts)
    {
        s64 ticks = mTicks + ts.mTicks;
        //		ASSERTS(((mTicks >> 63) == (ts.mTicks >> 63)) && ((mTicks >> 63) != (mTicks >> 63)), "timespan_t::Overflow::TooLong");
        mTicks = ticks;
        return *this;
    }

    /**
     *  Summary:
     *      Compares two System.timespan_t values and returns an integer that indicates
     *      their relationship.
     *
     *  Parameters:
     *    t2:
     *      A System.timespan_t.
     *
     *    t1:
     *      A System.timespan_t.
     *
     *  Returns:
     *      Value Condition -1 t1 is less than t2
     * 						0 t1 is equal to t2
     * 						1 t1 is greater than t2
     */
    s32 timespan_t::sCompare(const timespan_t& t1, const timespan_t& t2)
    {
        if (t1.mTicks > t2.mTicks)
        {
            return 1;
        }
        if (t1.mTicks < t2.mTicks)
        {
            return -1;
        }
        return 0;
    }

    /**
     *  Summary:
     *      Returns a new System.timespan_t object whose value is the absolute value of
     *      the current System.timespan_t object.
     *
     *  Returns:
     *      A new System.timespan_t whose value is the absolute value of the current System.timespan_t
     *      object.
     */
    timespan_t timespan_t::duration() const
    {
        ASSERTS(mTicks != sMinValue.mTicks, "Overflow_Duration");
        return timespan_t((mTicks >= 0) ? __ticks() : -__ticks());
    }

    /**
     *  Summary:
     *      Returns a System.timespan_t that represents a specified number of days, where
     *      the specification is accurate to the nearest millisecond.
     *
     *  Parameters:
     *    value:
     *      A number of days, accurate to the nearest millisecond.
     *
     *  Returns:
     *      A System.timespan_t that represents value.
     *
     *  Exceptions:
     *    System.ArgumentException:
     *      value is equal to System.Double.NaN.
     *
     *    System.OverflowException:
     *      value is less than System.timespan_t.MinValue or greater than System.timespan_t.MaxValue.
     */
    timespan_t timespan_t::sFromDays(u64 value) { return sInterval(value, sMillisPerDay); }

    /**
     *  Summary:
     *      Returns a System.timespan_t that represents a specified number of hours, where
     *      the specification is accurate to the nearest millisecond.
     *
     *  Parameters:
     *    value:
     *      A number of hours accurate to the nearest millisecond.
     *
     *  Returns:
     *      A System.timespan_t that represents value.
     *
     *  Exceptions:
     *    System.ArgumentException:
     *      value is equal to System.Double.NaN.
     *
     *    System.OverflowException:
     *      value is less than System.timespan_t.MinValue or greater than System.timespan_t.MaxValue.
     */
    timespan_t timespan_t::sFromHours(u64 value) { return sInterval(value, sMillisPerHour); }

    /**
     *  Summary:
     *      Returns a System.timespan_t that represents a specified number of milliseconds.
     *
     *  Parameters:
     *    value:
     *      A number of milliseconds.
     *
     *  Returns:
     *      A System.timespan_t that represents value.
     *
     *  Exceptions:
     *    System.ArgumentException:
     *      value is equal to System.Double.NaN.
     *
     *    System.OverflowException:
     *      value is less than System.timespan_t.MinValue or greater than System.timespan_t.MaxValue.
     */
    timespan_t timespan_t::sFromMilliseconds(u64 value) { return sInterval(value, 1); }

    /**
     *  Summary:
     *      Returns a System.timespan_t that represents a specified number of minutes,
     *      where the specification is accurate to the nearest millisecond.
     *
     *  Parameters:
     *    value:
     *      A number of minutes, accurate to the nearest millisecond.
     *
     *  Returns:
     *      A System.timespan_t that represents value.
     *
     *  Exceptions:
     *    System.ArgumentException:
     *      value is equal to System.Double.NaN.
     *
     *    System.OverflowException:
     *      value is less than System.timespan_t.MinValue or greater than System.timespan_t.MaxValue.
     */
    timespan_t timespan_t::sFromMinutes(u64 value) { return sInterval(value, sMillisPerMinute); }

    /**
     *  Summary:
     *      Returns a System.timespan_t that represents a specified number of seconds,
     *      where the specification is accurate to the nearest millisecond.
     */
    timespan_t timespan_t::sFromSeconds(u64 value) { return sInterval(value, sMillisPerSecond); }

    timespan_t timespan_t::sFromTicks(u64 value) { return timespan_t(value); }

    u64 timespan_t::sTimeToTicks(s32 hours, s32 minutes, s32 seconds) { return sTimeToTicks(0, hours, minutes, seconds, 0); }

    u64 timespan_t::sTimeToTicks(s32 hours, s32 minutes, s32 seconds, s32 milliseconds) { return sTimeToTicks(0, hours, minutes, seconds, milliseconds); }

    u64 timespan_t::sTimeToTicks(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds)
    {
        s64 num = ((((((s64(days) * 24 + s64(hours)) * 3600)) + (s64(minutes) * 60)) + s64(seconds)) * sMillisPerSecond) + s64(milliseconds);
        ASSERTS((num <= MaxMilliSeconds) && (num >= MinMilliSeconds), "Overflow_TimeSpanTooLong");
        return (u64)(num * sTicksPerMillisecond);
    }

    /**
     *  Summary:
     *      Returns a System.timespan_t whose value is the negated value of this instance.
     *
     *  Returns:
     *      The same numeric value as this instance, but with the opposite sign.
     *
     *  Exceptions:
     *    System.OverflowException:
     *      The negated value of this instance cannot be represented by a System.timespan_t;
     *      that is, the value of this instance is System.timespan_t.MinValue.
     */
    timespan_t& timespan_t::negate()
    {
        ASSERTS(mTicks != sMinValue.mTicks, "Overflow_NegateTwosCompNum");
        mTicks = -__ticks();
        return *this;
    }

    /**
     *
     *  Summary:
     *      Subtracts the specified System.timespan_t from this instance.
     *
     *  Parameters:
     *    ts:
     *      A System.timespan_t.
     *
     *  Returns:
     *      A System.timespan_t whose value is the result of the value of this instance
     *      minus the value of ts.
     *
     *  Exceptions:
     *    System.OverflowException:
     *      The return value is less than System.timespan_t.MinValue or greater than System.timespan_t.MaxValue.
     */
    timespan_t& timespan_t::substract(const timespan_t& ts)
    {
        s64 ticks = __ticks() - ts.__ticks();
        ASSERTS(((mTicks >> 0x3f) == (ts.mTicks >> 0x3f)) == ((mTicks >> 0x3f) == (mTicks >> 0x3f)), "Overflow_TimeSpanTooLong");
        mTicks = ticks;
        return *this;
    }

    timespan_t operator-(const timespan_t& t1, const timespan_t& t2)
    {
        timespan_t s(t1);
        s.substract(t2);
        return s;
    }
    timespan_t operator+(const timespan_t& t1, const timespan_t& t2)
    {
        timespan_t s(t1);
        s.add(t2);
        return s;
    }
    bool operator<(const timespan_t& t1, const timespan_t& t2) { return t1.ticks() < t2.ticks(); }
    bool operator>(const timespan_t& t1, const timespan_t& t2) { return t1.ticks() > t2.ticks(); }
    bool operator<=(const timespan_t& t1, const timespan_t& t2) { return t1.ticks() <= t2.ticks(); }
    bool operator>=(const timespan_t& t1, const timespan_t& t2) { return t1.ticks() >= t2.ticks(); }
    bool operator==(const timespan_t& t1, const timespan_t& t2) { return t1.ticks() == t2.ticks(); }
    bool operator!=(const timespan_t& t1, const timespan_t& t2) { return t1.ticks() != t2.ticks(); }

    //==============================================================================
    // END xCore namespace
    //==============================================================================
}; // namespace ncore
