//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_debug.h"
#include "xbase\x_string_std.h"

#include "xtime\x_time.h"
#include "xtime\x_timespan.h"

/**
 * xCore namespace
 */
namespace xcore
{
	/**
	 * xtimespan
	 */
	const u64		xtimespan::sTicksPerDay			= X_CONSTANT_64(864000000000);
	const u64		xtimespan::sTicksPerHour		= X_CONSTANT_64(36000000000);
	const u64		xtimespan::sTicksPerMillisecond	= 10000;
	const u64		xtimespan::sTicksPerMinute		= 600000000;
	const u64		xtimespan::sTicksPerSecond		= 10000000;

	const s32		xtimespan::sMillisPerDay		= 86400000;
	const s32		xtimespan::sMillisPerHour		= 3600000;
	const s32		xtimespan::sMillisPerMinute		= 60000;
	const s32		xtimespan::sMillisPerSecond		= 1000;

	const xtimespan	xtimespan::sMaxValue(X_CONSTANT_64(0x2bca2875f4373fff));
	const xtimespan	xtimespan::sMinValue(0);
	const xtimespan	xtimespan::sZero(0);

	const s64		MaxMilliSeconds					= X_CONSTANT_64(922337203685477);
	const s64		MinMilliSeconds					= X_CONSTANT_64(-922337203685477);

#ifdef X_ASSERT
	const s64		MaxSeconds						= X_CONSTANT_64(922337203685);
	const s64		MinSeconds						= X_CONSTANT_64(-922337203685);
#endif

	static xtimespan	sInterval(u64 value, s32 scale)
	{
		s64 num = value * scale;
		ASSERTS((num <= MaxMilliSeconds) && (num >= MinMilliSeconds), "xtimespan::Overflow::TooLong");
		return xtimespan(((s64)num) * xtimespan::sTicksPerMillisecond);
	}


	/** 
	 *  Summary:
	 *      Initializes a new System.xtimespan to the specified number of ticks.
	 * 
	 *  Parameters:
	 *    ticks:
	 *      A time period expressed in 100-nanosecond units.
     */
	xtimespan::xtimespan(u64 ticks)
		: mTicks(ticks)
	{
	}

	/** 
	 *  Summary:
	 *      Initializes a new System.xtimespan to a specified number of hours, minutes,
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
	xtimespan::xtimespan(s32 hours, s32 minutes, s32 seconds)
	{
		mTicks = sTimeToTicks(hours, minutes, seconds);
	}

	/** 
	 *  Summary:
	 *      Initializes a new System.xtimespan to a specified number of days, hours, minutes,
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
	xtimespan::xtimespan(s32 days, s32 hours, s32 minutes, s32 seconds)
	{
		mTicks = sTimeToTicks(days, hours, minutes, seconds, 0);
	}

	/** 
	 *  Summary:
	 *      Initializes a new System.xtimespan to a specified number of days, hours, minutes,
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
	xtimespan::xtimespan(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds)
	{
		mTicks = sTimeToTicks(days, hours, minutes, seconds, milliseconds);
	}


	/**
     *  Summary:
	 *      Gets the number of whole days represented by the current System.xtimespan
	 *      structure.
	 * 
	 *  Returns:
	 *      The day component of this instance. The return value can be positive or negative.
	 */
	s32					xtimespan::days() const
	{
		return (s32) (mTicks / sTicksPerDay);
	}

	/** 
	 *  Summary:
	 *      Gets the number of whole hours represented by the current System.xtimespan
	 *      structure.
	 * 
	 *  Returns:
	 *      The hour component of the current System.xtimespan structure. The return value
	 *      ranges from -23 through 23.
	 */
	s32					xtimespan::hours() const
	{
		return (s32) ((mTicks / sTicksPerHour) % ((s64) 0x18));
	}

	/** 
	 *  Summary:
	 *      Gets the number of whole milliseconds represented by the current System.xtimespan
	 *      structure.
	 * 
	 *  Returns:
	 *      The millisecond component of the current System.xtimespan structure. The return
	 *      value ranges from -999 through 999.
	 */
	s32					xtimespan::milliseconds() const
	{
		return (s32) ((mTicks / ((s64) sTicksPerMillisecond)) % ((s64) sMillisPerSecond));
	}

	/** 
	 *  Summary:
	 *      Gets the number of whole minutes represented by the current System.xtimespan
	 *      structure.
	 * 
	 *  Returns:
	 *      The minute component of the current System.xtimespan structure. The return
	 *      value ranges from -59 through 59.
	 */
	s32					xtimespan::minutes() const
	{
		return (s32) ((mTicks / ((s64) sTicksPerMinute)) % ((s64) 60));
	}

	/** 
	 *  Summary:
	 *      Gets the number of whole seconds represented by the current System.xtimespan
	 *      structure.
	 * 
	 *  Returns:
	 *      The second component of the current System.xtimespan structure. The return
	 *      value ranges from -59 through 59.
	 */
	s32					xtimespan::seconds() const
	{
		return (s32) ((mTicks / ((s64) sTicksPerSecond)) % ((s64) 60));
	}

	/** 
	 *  Summary:
	 *      Gets the number of ticks that represent the value of the current System.xtimespan
	 *      structure.
	 * 
	 *  Returns:
	 *      The number of ticks contained in this instance.
	 */
	u64				xtimespan::ticks() const
	{
		return mTicks;
	}

	/** 
	 *  Summary:
	 *      Gets the value of the current System.xtimespan structure expressed in whole
	 *      and fractional days.
	 * 
	 *  Returns:
	 *      The total number of days represented by this instance.
	 */
	u64				xtimespan::totalDays() const
	{
		return (mTicks / sTicksPerDay);
	}

	/** 
	 *  Summary:
	 *      Gets the value of the current System.xtimespan structure expressed in whole
	 *      and fractional hours.
	 * 
	 *  Returns:
	 *      The total number of hours represented by this instance.
     */
	u64				xtimespan::totalHours() const
	{
		return (mTicks / sTicksPerHour);
	}

	/** 
	 *  Summary:
	 *      Gets the value of the current System.xtimespan structure expressed in whole
	 *      and fractional milliseconds.
	 * 
	 *  Returns:
	 *      The total number of milliseconds represented by this instance.
	 */
	u64				xtimespan::totalMilliseconds() const
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
	 *      Gets the value of the current System.xtimespan structure expressed in whole
	 *      and fractional minutes.
	 * 
	 *  Returns:
	 *      The total number of minutes represented by this instance.
	 */
	u64				xtimespan::totalMinutes() const
	{
		return (mTicks / sTicksPerMinute);
	}

	/** 
	 *  Summary:
	 *      Gets the value of the current System.xtimespan structure expressed in whole
	 *      and fractional seconds.
	 * 
	 *  Returns:
	 *      The total number of seconds represented by this instance.
	 */
	u64				xtimespan::totalSeconds() const
	{
		return (mTicks / sTicksPerSecond);
	}

	/**
     *  Summary:
	 *      Adds the specified System.xtimespan to this instance.
	 * 
	 *  Parameters:
	 *    ts:
	 *      A System.xtimespan.
	 * 
	 *  Returns:
	 *      A System.xtimespan that represents the value of this instance plus the value
	 *      of ts.
	 */
	xtimespan&			xtimespan::add(const xtimespan& ts)
	{
		s64 ticks = mTicks + ts.mTicks;
//		ASSERTS(((mTicks >> 63) == (ts.mTicks >> 63)) && ((mTicks >> 63) != (mTicks >> 63)), "xtimespan::Overflow::TooLong");
		mTicks = ticks;
		return *this;
	}

	/** 
	 *  Summary:
	 *      Compares two System.xtimespan values and returns an integer that indicates
	 *      their relationship.
	 * 
	 *  Parameters:
	 *    t2:
	 *      A System.xtimespan.
	 * 
	 *    t1:
	 *      A System.xtimespan.
	 * 
	 *  Returns:
	 *      Value Condition -1 t1 is less than t2
	 * 						0 t1 is equal to t2
	 * 						1 t1 is greater than t2
	 */
	s32					xtimespan::sCompare(const xtimespan& t1, const xtimespan& t2)
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
	 *      Returns a new System.xtimespan object whose value is the absolute value of
	 *      the current System.xtimespan object.
	 * 
	 *  Returns:
	 *      A new System.xtimespan whose value is the absolute value of the current System.xtimespan
	 *      object.
	 */
	xtimespan			xtimespan::duration() const
	{
		ASSERTS(mTicks != sMinValue.mTicks, "Overflow_Duration");
		return xtimespan((mTicks >= 0) ? __ticks() : -__ticks());
	}

	/** 
	 *  Summary:
	 *      Returns a System.xtimespan that represents a specified number of days, where
	 *      the specification is accurate to the nearest millisecond.
	 * 
	 *  Parameters:
	 *    value:
	 *      A number of days, accurate to the nearest millisecond.
	 * 
	 *  Returns:
	 *      A System.xtimespan that represents value.
	 * 
	 *  Exceptions:
	 *    System.ArgumentException:
	 *      value is equal to System.Double.NaN.
	 * 
	 *    System.OverflowException:
	 *      value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	 */
	xtimespan			xtimespan::sFromDays(u64 value)
	{
		return sInterval(value, sMillisPerDay);
	}

	/** 
	 *  Summary:
	 *      Returns a System.xtimespan that represents a specified number of hours, where
	 *      the specification is accurate to the nearest millisecond.
	 * 
	 *  Parameters:
	 *    value:
	 *      A number of hours accurate to the nearest millisecond.
	 * 
	 *  Returns:
	 *      A System.xtimespan that represents value.
	 * 
	 *  Exceptions:
	 *    System.ArgumentException:
	 *      value is equal to System.Double.NaN.
	 * 
	 *    System.OverflowException:
	 *      value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	 */
	xtimespan			xtimespan::sFromHours(u64 value)
	{
		return sInterval(value, sMillisPerHour);
	}

	/** 
	 *  Summary:
	 *      Returns a System.xtimespan that represents a specified number of milliseconds.
	 * 
	 *  Parameters:
	 *    value:
	 *      A number of milliseconds.
	 * 
	 *  Returns:
	 *      A System.xtimespan that represents value.
	 * 
	 *  Exceptions:
	 *    System.ArgumentException:
	 *      value is equal to System.Double.NaN.
	 * 
	 *    System.OverflowException:
	 *      value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	 */
	xtimespan			xtimespan::sFromMilliseconds(u64 value)
	{
		return sInterval(value, 1);
	}

	/** 
	 *  Summary:
	 *      Returns a System.xtimespan that represents a specified number of minutes,
	 *      where the specification is accurate to the nearest millisecond.
	 * 
	 *  Parameters:
	 *    value:
	 *      A number of minutes, accurate to the nearest millisecond.
	 * 
	 *  Returns:
	 *      A System.xtimespan that represents value.
	 * 
	 *  Exceptions:
	 *    System.ArgumentException:
	 *      value is equal to System.Double.NaN.
	 * 
	 *    System.OverflowException:
	 *      value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	 */
	xtimespan			xtimespan::sFromMinutes(u64 value)
	{
		return sInterval(value, sMillisPerMinute);
	}

	/** 
	 *  Summary:
	 *      Returns a System.xtimespan that represents a specified number of seconds,
	 *      where the specification is accurate to the nearest millisecond.
	 */
	xtimespan			xtimespan::sFromSeconds(u64 value)
	{
		return sInterval(value, sMillisPerSecond);
	}

	xtimespan			xtimespan::sFromTicks(u64 value)
	{
		return xtimespan(value);
	}

	u64				xtimespan::sTimeToTicks(s32 hours, s32 minutes, s32 seconds)
	{
        return sTimeToTicks(0, hours, minutes, seconds, 0);
	}

	u64				xtimespan::sTimeToTicks(s32 hours, s32 minutes, s32 seconds, s32 milliseconds)
	{
		return sTimeToTicks(0, hours, minutes, seconds, milliseconds);
	}

	u64				xtimespan::sTimeToTicks(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds)
	{
		s64 num = ((((((s64(days)*24+s64(hours)) * 3600)) + (s64(minutes) * 60)) + s64(seconds)) * sMillisPerSecond) + s64(milliseconds);
		ASSERTS((num <= MaxMilliSeconds) && (num >= MinMilliSeconds), "Overflow_TimeSpanTooLong");
		return (u64)(num * sTicksPerMillisecond);
	}


	/** 
	 *  Summary:
	 *      Returns a System.xtimespan whose value is the negated value of this instance.
	 * 
	 *  Returns:
	 *      The same numeric value as this instance, but with the opposite sign.
	 * 
	 *  Exceptions:
	 *    System.OverflowException:
	 *      The negated value of this instance cannot be represented by a System.xtimespan;
	 *      that is, the value of this instance is System.xtimespan.MinValue.
	 */
	xtimespan&			xtimespan::negate()
	{
		ASSERTS(mTicks != sMinValue.mTicks, "Overflow_NegateTwosCompNum");
		mTicks = -__ticks();
		return *this;
	}

	/**
     * 
	 *  Summary:
	 *      Subtracts the specified System.xtimespan from this instance.
	 * 
	 *  Parameters:
	 *    ts:
	 *      A System.xtimespan.
	 * 
	 *  Returns:
	 *      A System.xtimespan whose value is the result of the value of this instance
	 *      minus the value of ts.
	 * 
	 *  Exceptions:
	 *    System.OverflowException:
	 *      The return value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	 */
	xtimespan&			xtimespan::substract(const xtimespan& ts)
	{
		s64 ticks = __ticks() - ts.__ticks();
		ASSERTS(((mTicks >> 0x3f) == (ts.mTicks >> 0x3f)) == ((mTicks >> 0x3f) == (mTicks >> 0x3f)), "Overflow_TimeSpanTooLong");
		mTicks = ticks;
		return *this;
	}


	xtimespan			operator  -(const xtimespan& t1, const xtimespan& t2)					{ xtimespan s(t1); s.substract(t2); return s; }
	xtimespan			operator  +(const xtimespan& t1, const xtimespan& t2)					{ xtimespan s(t1); s.add(t2); return s; }
	xbool				operator  <(const xtimespan& t1, const xtimespan& t2)					{ return t1.ticks() < t2.ticks(); }
	xbool				operator  >(const xtimespan& t1, const xtimespan& t2)					{ return t1.ticks() > t2.ticks(); }
	xbool				operator <=(const xtimespan& t1, const xtimespan& t2)					{ return t1.ticks() <= t2.ticks(); }
	xbool				operator >=(const xtimespan& t1, const xtimespan& t2)					{ return t1.ticks() >= t2.ticks(); }
	xbool				operator ==(const xtimespan& t1, const xtimespan& t2)					{ return t1.ticks() == t2.ticks(); }
	xbool				operator !=(const xtimespan& t1, const xtimespan& t2)					{ return t1.ticks() != t2.ticks(); }

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};
