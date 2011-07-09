//==============================================================================
// INCLUDES
//==============================================================================
#include "xbase\x_types.h"
#include "xbase\x_debug.h"
#include "xbase\x_string_std.h"
#include "xtime\x_time.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	static const s32 DaysPer100Years		= 36524;
	static const s32 DaysPer400Years		= 146097;
	static const s32 DaysPer4Years			= 1461;
	static const s32 DaysPerYear			= 365;

	//static const s32 DaysTo10000			= 3652059;
	//static const s32 DaysTo1601				= 584388;
	//static const s32 DaysTo1899				= 693593;

	//static const u64 MinTicks				= 0;
	static const u64 MaxTicks				= X_CONSTANT_64(0x2bca2875f4373fff);

	static const s64 MaxMillis				= X_CONSTANT_64(0x11efae44cb400);

	static const s64 TicksPerDay			= X_CONSTANT_64(0xc92a69c000);
	static const s64 TicksPerHour			= X_CONSTANT_64(0x861c46800);
	static const s64 TicksPerMillisecond	= 10000;
	static const s64 TicksPerMinute			= 600000000;
	static const s64 TicksPerSecond			= 10000000;

	//static const u64 TicksMask				= X_CONSTANT_64(0x3fffffffffffffff);

	static const s32 MillisPerDay			= 86400000;
	static const s32 MillisPerHour			= 3600000;
	static const s32 MillisPerMinute		= 60000;
	static const s32 MillisPerSecond		= 1000;

	static const s32 sDaysToMonth365[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	static const s32 sDaysToMonth366[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

	static s64	sDateToTicks(s32 year, s32 month, s32 day)
	{
		if (((year >= 1) && (year <= 9999)) && ((month >= 1) && (month <= 12)))
		{
			const s32* daysToMonth = xdatetime::sIsLeapYear(year) ? sDaysToMonth366 : sDaysToMonth365;
			if ((day >= 1) && (day <= (daysToMonth[month] - daysToMonth[month - 1])))
			{
				s32 yearMinusOne = year - 1;
				s32 numDays = ((((((yearMinusOne * DaysPerYear) + (yearMinusOne / 4)) - (yearMinusOne / 100)) + 
					(yearMinusOne / 400)) + daysToMonth[month - 1]) + day) - 1;
				return (numDays * TicksPerDay);
			}
		}
		ASSERTS(false, "Invalid input!");
		return 0;
	}


	static s64	sTimeToTicks(s32 hour, s32 minute, s32 second)
	{
		ASSERTS(((((hour >= 0) && (hour < 24)) && ((minute >= 0) && (minute < 60))) && ((second >= 0) && (second < 60))), "Invalid input!");
		return xtimespan::sTimeToTicks(hour, minute, second);
	}

	enum EDatePart
	{
		DatePartYear = 0,
		DatePartDayOfYear = 1,
		DatePartMonth = 2,
		DatePartDay = 3,
	};
	s32					sGetDatePart(EDatePart part, s64 ticks)
	{
		s32 num2 = (s32) (ticks / TicksPerDay);
		s32 num3 = num2 / DaysPer400Years;
		num2 -= num3 * DaysPer400Years;
		s32 num4 = num2 / DaysPer100Years;
		if (num4 == 4)
		{
			num4 = 3;
		}
		num2 -= num4 * DaysPer100Years;
		s32 num5 = num2 / DaysPer4Years;
		num2 -= num5 * DaysPer4Years;
		s32 num6 = num2 / DaysPerYear;
		if (num6 == 4)
		{
			num6 = 3;
		}
		if (part == DatePartYear)
		{
			return (((((num3 * 400) + (num4 * 100)) + (num5 * 4)) + num6) + 1);
		}
		num2 -= num6 * DaysPerYear;
		if (part == DatePartDayOfYear)
		{
			return (num2 + 1);
		}
		const s32* numArray = ((num6 == 3) && ((num5 != 0x18) || (num4 == 3))) ? sDaysToMonth366 : sDaysToMonth365;
		s32 index = num2 >> 6;
		while (num2 >= numArray[index])
		{
			index++;
		}
		if (part == DatePartMonth)
		{
			return index;
		}
		return ((num2 - numArray[index - 1]) + 1);
	}


	const xdatetime	xdatetime::sMaxValue(MaxTicks);
	const xdatetime	xdatetime::sMinValue(0);


	xdatetime::xdatetime()
	:mTicks(sMinValue.mTicks)
	{
		ASSERTS(mTicks<=MaxTicks, "Error: out of range!");
	}

	xdatetime::xdatetime(u64 ticks)
	:mTicks(ticks)
	{
		ASSERTS(mTicks<=MaxTicks, "Error: out of range!");
	}


	//   month: 		The month (1 through 12).
	//   day:			The day (1 through the number of days in month).
	//   year:			The year (1 through 9999).
	xdatetime::xdatetime(s32 year, s32 month, s32 day)
	{
		mTicks = sDateToTicks(year, month, day);
	}


	//   month:			The month (1 through 12).
	//   day:			The day (1 through the number of days in month).
	//   minute:		The minutes (0 through 59).
	//   year:			The year (1 through 9999).
	//   hour:			The hours (0 through 23).
	//   second:		The seconds (0 through 59).
	xdatetime::xdatetime(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second)
	{
		mTicks = (s64) (sDateToTicks(year, month, day) + sTimeToTicks(hour, minute, second));
	}


	//   month:			The month (1 through 12).
	//   millisecond:	The milliseconds (0 through 999).
	//   day:			The day (1 through the number of days in month).
	//   minute:		The minutes (0 through 59).
	//   year:			The year (1 through 9999).
	//   hour:			The hours (0 through 23).
	//   second:		The seconds (0 through 59).
	xdatetime::xdatetime(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second, s32 millisecond)
	{
		s64 num = sDateToTicks(year, month, day) + sTimeToTicks(hour, minute, second);
		ASSERTS((millisecond >= 0) && (millisecond < MillisPerSecond), "Invalid input!");
		num += millisecond * TicksPerMillisecond;
		ASSERTS((num >= 0) && (num < MaxTicks), "Out of range!");
		mTicks = num;
	}


	// Summary:
	//     Gets the date component of this instance.
	//
	// Returns:
	//     A new System.xdatetime with the same date as this instance, and the time value
	//     set to 12:00:00 midnight (00:00:00).
	xdatetime			xdatetime::date() const
	{
		s64 t = __ticks();
		return xdatetime(((u64)(t - (t % TicksPerDay))));
	}

	//
	// Summary:
	//     Gets the day of the month represented by this instance.
	//
	// Returns:
	//     The day component, expressed as a value between 1 and 31.
	s32					xdatetime::day() const
	{
		return sGetDatePart(DatePartDay, __ticks());
	}

	//
	// Summary:
	//     Gets the day of the week represented by this instance.
	//
	// Returns:
	//     A System.DayOfWeek enumerated constant that indicates the day of the week.
	//     This property value ranges from zero, indicating Sunday, to six, indicating
	//     Saturday.
	EDayOfWeek			xdatetime::dayOfWeek() const
	{
		s32 days = (s32)((__ticks() / TicksPerDay) + 1);
		EDayOfWeek dayOfWeek = (EDayOfWeek)(days % 7);
		return dayOfWeek;
	}
	EDayOfWeek			xdatetime::dayOfWeekShort() const
	{
		s32 days = (s32)((__ticks() / TicksPerDay) + 1);
		EDayOfWeek dayOfWeek = (EDayOfWeek)(days % 7);
		return (EDayOfWeek)(dayOfWeek + DaysPerWeek);
	}

	//
	// Summary:
	//     Gets the day of the year represented by this instance.
	//
	// Returns:
	//     The day of the year, expressed as a value between 1 and 366.
	s32					xdatetime::dayOfYear() const
	{
		return sGetDatePart(DatePartDayOfYear, __ticks());
	}

	//
	// Summary:
	//     Gets the hour component of the date represented by this instance.
	//
	// Returns:
	//     The hour component, expressed as a value between 0 and 23.
	s32					xdatetime::hour() const
	{
		return (s32) ((__ticks() / TicksPerHour) % ((s64) 24));
	}

	//
	// Summary:
	//     Gets the milliseconds component of the date represented by this instance.
	//
	// Returns:
	//     The milliseconds component, expressed as a value between 0 and 999.
	s32					xdatetime::millisecond() const
	{
		return (s32) ((__ticks() / ((s64) TicksPerMillisecond)) % ((s64) MillisPerSecond));
	}

	//
	// Summary:
	//     Gets the minute component of the date represented by this instance.
	//
	// Returns:
	//     The minute component, expressed as a value between 0 and 59.
	s32					xdatetime::minute() const
	{
		return (s32) ((__ticks() / ((s64) TicksPerMinute)) % ((s64) 60));
	}

	//
	// Summary:
	//     Gets the month component of the date represented by this instance.
	//
	// Returns:
	//     The month component, expressed as a value between 1 and 12.
	EMonth				xdatetime::month() const
	{
		return (EMonth)sGetDatePart(DatePartMonth, __ticks());
	}
	EMonth				xdatetime::monthShort() const
	{
		return (EMonth)(sGetDatePart(DatePartMonth, __ticks()) + MonthsPerYear);
	}
	//
	// Summary:
	//     Gets a System.xdatetime object that is set to the current date and time on
	//     this computer, expressed as the local time.
	//
	// Returns:
	//     A System.xdatetime whose value is the current local date and time.
	xdatetime			xdatetime::sNow()
	{
		return xdatetime(sGetSystemTime());
	}
	//
	// Summary:
	//     Gets the seconds component of the date represented by this instance.
	//
	// Returns:
	//     The seconds, between 0 and 59.
	s32					xdatetime::second() const
	{
		return (s32) ((__ticks() / ((s64) TicksPerSecond)) % ((s64) 60));
	}

	//
	// Summary:
	//     Gets the number of ticks that represent the date and time of this instance.
	//
	// Returns:
	//     The number of ticks that represent the date and time of this instance. The
	//     value is between System.xdatetime.MinValue and System.xdatetime.MaxValue.
	u64				xdatetime::ticks() const
	{
		return __ticks();
	}

	//
	// Summary:
	//     Gets the time of day for this instance.
	//
	// Returns:
	//     A System.xtimespan that represents the fraction of the day that has elapsed
	//     since midnight.
	xtimespan			xdatetime::timeOfDay() const
	{
		return xtimespan(__ticks() % TicksPerDay);
	}

	//
	// Summary:
	//     Gets the current date.
	//
	// Returns:
	//     A System.xdatetime set to today's date, with the time component set to 00:00:00.
	xdatetime			xdatetime::sToday()
	{
		return sNow().date();
	}

	//
	// Summary:
	//     Gets the year component of the date represented by this instance.
	//
	// Returns:
	//     The year, between 1 and 9999.
	s32					xdatetime::year() const
	{
		return sGetDatePart(DatePartYear, __ticks());
	}


	// Summary:
	//     Adds the value of the specified System.xtimespan to the value of this instance.
	//
	// Parameters:
	//   value:
	//     A System.xtimespan that contains the interval to add.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and the time interval represented by value.
	//
	xdatetime&			xdatetime::add(const xtimespan& value)
	{
		return addTicks(value.ticks());
	}

	//
	// Summary:
	//     Adds the specified number of days to the value of this instance.
	//
	// Parameters:
	//   value:
	//     A number of whole and fractional days. The value parameter can be negative
	//     or positive.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and the number of days represented by value.
	//
	xdatetime&			xdatetime::addDays(s32 value)
	{
		return add(value, MillisPerDay);
	}

	//
	// Summary:
	//     Adds the specified number of hours to the value of this instance.
	//
	// Parameters:
	//   value:
	//     A number of whole and fractional hours. The value parameter can be negative
	//     or positive.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and the number of hours represented by value.
	//
	xdatetime&			xdatetime::addHours(s32 value)
	{
		return add(value, MillisPerHour);
	}

	//
	// Summary:
	//     Adds the specified number of milliseconds to the value of this instance.
	//
	// Parameters:
	//   value:
	//     A number of whole and fractional milliseconds. The value parameter can be
	//     negative or positive.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and the number of milliseconds represented by value.
	//
	xdatetime&			xdatetime::addMilliseconds(s32 value)
	{
		return add(value, 1);
	}

	//
	// Summary:
	//     Adds the specified number of minutes to the value of this instance.
	//
	// Parameters:
	//   value:
	//     A number of whole and fractional minutes. The value parameter can be negative
	//     or positive.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and the number of minutes represented by value.
	//
	xdatetime&			xdatetime::addMinutes(s32 value)
	{
		return add(value, MillisPerMinute);
	}

	//
	// Summary:
	//     Adds the specified number of months to the value of this instance.
	//
	// Parameters:
	//   months:
	//     A number of months. The months parameter can be negative or positive.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and months.
	//

	xdatetime&			xdatetime::addMonths(s32 months)
	{
		ASSERTS((months >= -120000) && (months <= 120000), "ArgumentOutOfRange_DateTimeBadMonths");

		s32 year  = sGetDatePart(DatePartYear, __ticks());
		s32 month = sGetDatePart(DatePartMonth, __ticks());
		s32 day   = sGetDatePart(DatePartDay, __ticks());
		s32 num4 = (month - 1) + months;
		if (num4 >= 0)
		{
			month = (num4 % 12) + 1;
			year += num4 / 12;
		}
		else
		{
			month = 12 + ((num4 + 1) % 12);
			year += (num4 - 11) / 12;
		}
		ASSERTS((year >= 1) && (year <= 9999), "ArgumentOutOfRange_DateArithmetic");

		s32 num5 = sDaysInMonth(year, month);
		if (day > num5)
		{
			day = num5;
		}
		mTicks = (((u64) (sDateToTicks(year, month, day) + (__ticks() % TicksPerDay))));
		return *this;
	}

	//
	// Summary:
	//     Adds the specified number of seconds to the value of this instance.
	//
	// Parameters:
	//   value:
	//     A number of whole and fractional seconds. The value parameter can be negative
	//     or positive.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and the number of seconds represented by value.
	//
	xdatetime&			xdatetime::addSeconds(s32 value)
	{
		return add(value, MillisPerSecond);
	}

	//
	// Summary:
	//     Adds the specified number of ticks to the value of this instance.
	//
	// Parameters:
	//   value:
	//     A number of 100-nanosecond ticks. The value parameter can be positive or
	//     negative.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and the time represented by value.
	//
	xdatetime&			xdatetime::addTicks(u64 value)
	{
		s64 ticks = __ticks();
		ASSERTS(((s64)value <= (s64)(MaxTicks - ticks)) && ((s64)value >= -ticks), "ArgumentOutOfRange_DateArithmetic");
		mTicks = ((u64) (ticks + value));
		return *this;
	}

	//
	// Summary:
	//     Adds the specified number of years to the value of this instance.
	//
	// Parameters:
	//   value:
	//     A number of years. The value parameter can be negative or positive.
	//
	// Returns:
	//     A System.xdatetime whose value is the sum of the date and time represented
	//     by this instance and the number of years represented by value.
	//
	xdatetime&			xdatetime::addYears(s32 value)
	{
		ASSERTS((value >= -10000) && (value <= TicksPerMillisecond), "ArgumentOutOfRange_DateTimeBadYears");
		return addMonths(value * 12);
	}

	//
	// Summary:
	//     Compares two instances of System.xdatetime and returns an indication of their
	//     relative values.
	//
	// Parameters:
	//   t2:
	//     The second System.xdatetime.
	//
	//   t1:
	//     The first System.xdatetime.
	//
	// Returns:
	//     A signed number indicating the relative values of t1 and t2.Value Type Condition
	//     Less than zero t1 is less than t2. Zero t1 equals t2. Greater than zero t1
	//     is greater than t2.
	s32					xdatetime::sCompare(const xdatetime& t1, const xdatetime& t2)
	{
		s64 __ticks = t1.__ticks();
		s64 num2 = t2.__ticks();
		if (__ticks > num2)
			return 1;
		else if (__ticks < num2)
			return -1;
		else
			return 0;
	}

	//
	// Summary:
	//     Returns the number of days in the specified month and year.
	//
	// Parameters:
	//   month:
	//     The month (a number ranging from 1 to 12).
	//
	//   year:
	//     The year.
	//
	// Returns:
	//     The number of days in month for the specified year.For example, if month
	//     equals 2 for February, the return value is 28 or 29 depending upon whether
	//     year is a leap year.
	//
	s32					xdatetime::sDaysInMonth(s32 year, s32 month)
	{
		ASSERTS((month >= 1) && (month <= 12), "ArgumentOutOfRange_Month");
		const s32* numArray = sIsLeapYear(year) ? sDaysToMonth366 : sDaysToMonth365;
		return (numArray[month] - numArray[month - 1]);
	}


	//
	// Summary:
	//     Returns the number of days in the specified year.
	//
	// Parameters:
	//   month:
	//     The month (a number ranging from 1 to 12).
	//
	//   year:
	//     The year.
	//
	// Returns:
	//     The number of days for the specified year.
	//
	s32					xdatetime::sDaysInYear(s32 year)
	{
		return sIsLeapYear(year) ? (DaysPerYear+1) : DaysPerYear;
	}

	//
	// Summary:
	//     Converts the specified Windows file time to an equivalent local time.
	//
	// Parameters:
	//   fileTime:
	//     A Windows file time expressed in ticks.
	//
	// Returns:
	//     A System.xdatetime object that represents a local time equivalent to the date
	//     and time represented by the fileTime parameter.
	//
	// Exceptions:
	//   System.ArgumentOutOfRangeException:
	//     fileTime is less than 0 or represents a time greater than System.xdatetime.MaxValue.
	xdatetime			xdatetime::sFromFileTime(u64 fileTime)
	{
		ASSERTS(fileTime <= MaxTicks, "ArgumentOutOfRange_FileTimeInvalid");
		u64 systemTime = sGetSystemTimeFromFileTime(fileTime);
		return xdatetime(systemTime);
	}


	//
	// Summary:
	//     Returns an indication whether the specified year is a leap year.
	//
	// Parameters:
	//   year:
	//     A 4-digit year.
	//
	// Returns:
	//     true if year is a leap year; otherwise, false.
	//
	xbool				xdatetime::sIsLeapYear(s32 year)
	{
		ASSERTS((year >= 1) || (year <= 9999), "ArgumentOutOfRange_Year");
		if ((year % 4) != 0)
		{
			return false;
		}
		if ((year % 100) == 0)
		{
			return ((year % 400) == 0);
		}
		return true;
	}

	//
	// Summary:
	//     Subtracts the specified date and time from this instance.
	//
	// Parameters:
	//   value:
	//     An instance of System.xdatetime.
	//
	// Returns:
	//     A System.xtimespan interval equal to the date and time represented by this
	//     instance minus the date and time represented by value.
	//
	xtimespan			xdatetime::subtract(xdatetime value) const
	{
		return xtimespan(__ticks() - value.__ticks());
	}

	//
	// Summary:
	//     Subtracts the specified duration from this instance.
	//
	// Parameters:
	//   value:
	//     An instance of System.xtimespan.
	//
	// Returns:
	//     A System.xdatetime equal to the date and time represented by this instance
	//     minus the time interval represented by value.
	//
	xdatetime&			xdatetime::subtract(xtimespan value)
	{
		s64 ticks = __ticks();
		s64 num2 = value.ticks();
		ASSERTS((ticks >= num2) && ((s64)(ticks - MaxTicks) <= num2), "ArgumentOutOfRange_DateArithmetic");
		mTicks = ((u64) (ticks - num2));
		return *this;
	}

	//
	// Summary:
	//     Serializes the current System.xdatetime object to a 64-bit binary value that
	//     subsequently can be used to recreate the System.xdatetime object.
	//
	// Returns:
	//     A 64-bit signed integer that encodes the System.xdatetime.Kind and System.xdatetime.ticks
	//     properties.
	u64				xdatetime::toBinary() const
	{
		return __ticks();
	}

	//
	// Summary:
	//     Converts the value of the current System.xdatetime object to a Windows file
	//     time.
	//
	// Returns:
	//     The value of the current System.xdatetime object expressed as a Windows file
	//     time.
	//
	u64				xdatetime::toFileTime() const
	{
		s64 fileTime = sGetFileTimeFromSystemTime(__ticks());
		ASSERTS((fileTime >= 0) && (fileTime <= MaxTicks), "ArgumentOutOfRange_FileTimeInvalid");
		return (u64)fileTime;
	}

	xdatetime&		xdatetime::add(s32 value, s32 scale)
	{
		s64 num = (s64)value * scale;
		ASSERTS((num > X_CONSTANT_64(-315537897600000)) && (num < MaxMillis), "Argument Out Of Range, AddValue");
		return addTicks(num * TicksPerMillisecond);
	}



	// Global operators
	xdatetime			operator  -(const xdatetime&  d, const xtimespan&  t)
	{
		s64 ticks = d.ticks() - t.ticks();
		return xdatetime(ticks);
	}

	xdatetime			operator  +(const xdatetime&  d, const xtimespan&  t)
	{
		s64 ticks = d.ticks() + t.ticks();
		return xdatetime(ticks);
	}

	xtimespan			operator  -(const xdatetime& d1, const xdatetime& d2)
	{
		s64 ticks = d1.ticks() - d2.ticks();
		return xtimespan(ticks);
	}

	xbool				operator  <(const xdatetime& t1, const xdatetime& t2)
	{
		return t1.ticks() < t2.ticks();
	}

	xbool				operator  >(const xdatetime& t1, const xdatetime& t2)
	{
		return t1.ticks() > t2.ticks();
	}

	xbool				operator <=(const xdatetime& t1, const xdatetime& t2)
	{
		return t1.ticks() <= t2.ticks();
	}

	xbool				operator >=(const xdatetime& t1, const xdatetime& t2)
	{
		return t1.ticks() >= t2.ticks();
	}

	xbool				operator !=(const xdatetime& d1, const xdatetime& d2)
	{
		return d1.ticks() != d2.ticks();
	}

	xbool				operator ==(const xdatetime& d1, const xdatetime& d2)
	{
		return d1.ticks() == d2.ticks();
	}





	const u64		xtimespan::sTicksPerDay = X_CONSTANT_64(864000000000);
	const u64		xtimespan::sTicksPerHour = X_CONSTANT_64(36000000000);
	const u64		xtimespan::sTicksPerMillisecond = 10000;
	const u64		xtimespan::sTicksPerMinute = 600000000;
	const u64		xtimespan::sTicksPerSecond = 10000000;

	const xtimespan	xtimespan::sMaxValue(X_CONSTANT_64(0x2bca2875f4373fff));
	const xtimespan	xtimespan::sMinValue(0);
	const xtimespan	xtimespan::sZero(0);

	const s64		MaxMilliSeconds = X_CONSTANT_64(922337203685477);
	const s64		MinMilliSeconds = X_CONSTANT_64(-922337203685477);

	const s64		MaxSeconds = X_CONSTANT_64(922337203685);
	const s64		MinSeconds = X_CONSTANT_64(-922337203685);

	static xtimespan	sInterval(u64 value, s32 scale)
	{
		s64 num = value * scale;
		ASSERTS((num <= MaxMilliSeconds) && (num >= MinMilliSeconds), "xtimespan::Overflow::TooLong");
		return xtimespan(((s64)num) * xtimespan::sTicksPerMillisecond);
	}


	//
	// Summary:
	//     Initializes a new System.xtimespan to the specified number of ticks.
	//
	// Parameters:
	//   ticks:
	//     A time period expressed in 100-nanosecond units.
	xtimespan::xtimespan(u64 ticks)
		: mTicks(ticks)
	{
	}

	//
	// Summary:
	//     Initializes a new System.xtimespan to a specified number of hours, minutes,
	//     and seconds.
	//
	// Parameters:
	//   seconds:
	//     Number of seconds.
	//
	//   hours:
	//     Number of hours.
	//
	//   minutes:
	//     Number of minutes.
	//
	xtimespan::xtimespan(s32 hours, s32 minutes, s32 seconds)
	{
		mTicks = sTimeToTicks(hours, minutes, seconds);
	}

	//
	// Summary:
	//     Initializes a new System.xtimespan to a specified number of days, hours, minutes,
	//     and seconds.
	//
	// Parameters:
	//   seconds:
	//     Number of seconds.
	//
	//   hours:
	//     Number of hours.
	//
	//   minutes:
	//     Number of minutes.
	//
	//   days:
	//     Number of days.
	//
	xtimespan::xtimespan(s32 days, s32 hours, s32 minutes, s32 seconds)
	{
		mTicks = sTimeToTicks(days, hours, minutes, seconds, 0);
	}

	//
	// Summary:
	//     Initializes a new System.xtimespan to a specified number of days, hours, minutes,
	//     seconds, and milliseconds.
	//
	// Parameters:
	//   seconds:
	//     Number of seconds.
	//
	//   hours:
	//     Number of hours.
	//
	//   minutes:
	//     Number of minutes.
	//
	//   days:
	//     Number of days.
	//
	//   milliseconds:
	//     Number of milliseconds.
	//
	xtimespan::xtimespan(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds)
	{
		mTicks = sTimeToTicks(days, hours, minutes, seconds, milliseconds);
	}


	// Summary:
	//     Gets the number of whole days represented by the current System.xtimespan
	//     structure.
	//
	// Returns:
	//     The day component of this instance. The return value can be positive or negative.
	s32					xtimespan::days() const
	{
		return (s32) (mTicks / sTicksPerDay);
	}

	//
	// Summary:
	//     Gets the number of whole hours represented by the current System.xtimespan
	//     structure.
	//
	// Returns:
	//     The hour component of the current System.xtimespan structure. The return value
	//     ranges from -23 through 23.
	s32					xtimespan::hours() const
	{
		return (s32) ((mTicks / sTicksPerHour) % ((s64) 0x18));
	}

	//
	// Summary:
	//     Gets the number of whole milliseconds represented by the current System.xtimespan
	//     structure.
	//
	// Returns:
	//     The millisecond component of the current System.xtimespan structure. The return
	//     value ranges from -999 through 999.
	s32					xtimespan::milliseconds() const
	{
		return (s32) ((mTicks / ((s64) sTicksPerMillisecond)) % ((s64) MillisPerSecond));
	}

	//
	// Summary:
	//     Gets the number of whole minutes represented by the current System.xtimespan
	//     structure.
	//
	// Returns:
	//     The minute component of the current System.xtimespan structure. The return
	//     value ranges from -59 through 59.
	s32					xtimespan::minutes() const
	{
		return (s32) ((mTicks / ((s64) sTicksPerMinute)) % ((s64) 60));
	}

	//
	// Summary:
	//     Gets the number of whole seconds represented by the current System.xtimespan
	//     structure.
	//
	// Returns:
	//     The second component of the current System.xtimespan structure. The return
	//     value ranges from -59 through 59.
	s32					xtimespan::seconds() const
	{
		return (s32) ((mTicks / ((s64) sTicksPerSecond)) % ((s64) 60));
	}

	//
	// Summary:
	//     Gets the number of ticks that represent the value of the current System.xtimespan
	//     structure.
	//
	// Returns:
	//     The number of ticks contained in this instance.
	u64				xtimespan::ticks() const
	{
		return mTicks;
	}

	//
	// Summary:
	//     Gets the value of the current System.xtimespan structure expressed in whole
	//     and fractional days.
	//
	// Returns:
	//     The total number of days represented by this instance.
	u64				xtimespan::totalDays() const
	{
		return (mTicks / sTicksPerDay);
	}

	//
	// Summary:
	//     Gets the value of the current System.xtimespan structure expressed in whole
	//     and fractional hours.
	//
	// Returns:
	//     The total number of hours represented by this instance.
	u64				xtimespan::totalHours() const
	{
		return (mTicks / sTicksPerHour);
	}

	//
	// Summary:
	//     Gets the value of the current System.xtimespan structure expressed in whole
	//     and fractional milliseconds.
	//
	// Returns:
	//     The total number of milliseconds represented by this instance.
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

	//
	// Summary:
	//     Gets the value of the current System.xtimespan structure expressed in whole
	//     and fractional minutes.
	//
	// Returns:
	//     The total number of minutes represented by this instance.
	u64				xtimespan::totalMinutes() const
	{
		return (mTicks / sTicksPerMinute);
	}

	//
	// Summary:
	//     Gets the value of the current System.xtimespan structure expressed in whole
	//     and fractional seconds.
	//
	// Returns:
	//     The total number of seconds represented by this instance.
	u64				xtimespan::totalSeconds() const
	{
		return (mTicks / sTicksPerSecond);
	}

	// Summary:
	//     Adds the specified System.xtimespan to this instance.
	//
	// Parameters:
	//   ts:
	//     A System.xtimespan.
	//
	// Returns:
	//     A System.xtimespan that represents the value of this instance plus the value
	//     of ts.
	//
	xtimespan&			xtimespan::add(const xtimespan& ts)
	{
		s64 ticks = mTicks + ts.mTicks;
//		ASSERTS(((mTicks >> 63) == (ts.mTicks >> 63)) && ((mTicks >> 63) != (mTicks >> 63)), "xtimespan::Overflow::TooLong");
		mTicks = ticks;
		return *this;
	}

	//
	// Summary:
	//     Compares two System.xtimespan values and returns an integer that indicates
	//     their relationship.
	//
	// Parameters:
	//   t2:
	//     A System.xtimespan.
	//
	//   t1:
	//     A System.xtimespan.
	//
	// Returns:
	//     Value Condition -1 t1 is less than t2
	//						0 t1 is equal to t2
	//						1 t1 is greater than t2
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

	//
	// Summary:
	//     Returns a new System.xtimespan object whose value is the absolute value of
	//     the current System.xtimespan object.
	//
	// Returns:
	//     A new System.xtimespan whose value is the absolute value of the current System.xtimespan
	//     object.
	//
	xtimespan			xtimespan::duration() const
	{
		ASSERTS(mTicks != sMinValue.mTicks, "Overflow_Duration");
		return xtimespan((mTicks >= 0) ? __ticks() : -__ticks());
	}

	//
	// Summary:
	//     Returns a System.xtimespan that represents a specified number of days, where
	//     the specification is accurate to the nearest millisecond.
	//
	// Parameters:
	//   value:
	//     A number of days, accurate to the nearest millisecond.
	//
	// Returns:
	//     A System.xtimespan that represents value.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     value is equal to System.Double.NaN.
	//
	//   System.OverflowException:
	//     value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	xtimespan			xtimespan::sFromDays(u64 value)
	{
		return sInterval(value, MillisPerDay);
	}

	//
	// Summary:
	//     Returns a System.xtimespan that represents a specified number of hours, where
	//     the specification is accurate to the nearest millisecond.
	//
	// Parameters:
	//   value:
	//     A number of hours accurate to the nearest millisecond.
	//
	// Returns:
	//     A System.xtimespan that represents value.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     value is equal to System.Double.NaN.
	//
	//   System.OverflowException:
	//     value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	xtimespan			xtimespan::sFromHours(u64 value)
	{
		return sInterval(value, MillisPerHour);
	}

	//
	// Summary:
	//     Returns a System.xtimespan that represents a specified number of milliseconds.
	//
	// Parameters:
	//   value:
	//     A number of milliseconds.
	//
	// Returns:
	//     A System.xtimespan that represents value.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     value is equal to System.Double.NaN.
	//
	//   System.OverflowException:
	//     value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	xtimespan			xtimespan::sFromMilliseconds(u64 value)
	{
		return sInterval(value, 1);
	}

	//
	// Summary:
	//     Returns a System.xtimespan that represents a specified number of minutes,
	//     where the specification is accurate to the nearest millisecond.
	//
	// Parameters:
	//   value:
	//     A number of minutes, accurate to the nearest millisecond.
	//
	// Returns:
	//     A System.xtimespan that represents value.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     value is equal to System.Double.NaN.
	//
	//   System.OverflowException:
	//     value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
	xtimespan			xtimespan::sFromMinutes(u64 value)
	{
		return sInterval(value, MillisPerMinute);
	}

	//
	// Summary:
	//     Returns a System.xtimespan that represents a specified number of seconds,
	//     where the specification is accurate to the nearest millisecond.
	//
	xtimespan			xtimespan::sFromSeconds(u64 value)
	{
		return sInterval(value, MillisPerSecond);
	}

	xtimespan			xtimespan::sFromTicks(u64 value)
	{
		return xtimespan(value);
	}

	u64				xtimespan::sTimeToTicks(s32 hours, s32 minutes, s32 seconds)
	{
		//s64 num = ((hours * 0xe10) + (minutes * 60)) + seconds;
		//ASSERTS((num <= MaxSeconds) && (num >= MinSeconds), "Overflow_TimeSpanTooLong");
		//return (u64)(num * sTicksPerSecond);
        return sTimeToTicks(0, hours, minutes, seconds, 0);
	}

	u64				xtimespan::sTimeToTicks(s32 hours, s32 minutes, s32 seconds, s32 milliseconds)
	{
		return sTimeToTicks(0, hours, minutes, seconds, milliseconds);
	}

	u64				xtimespan::sTimeToTicks(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds)
	{
		s64 num = ((((((s64(days)*24+s64(hours)) * 3600)) + (s64(minutes) * 60)) + s64(seconds)) * MillisPerSecond) + s64(milliseconds);
		ASSERTS((num <= MaxMilliSeconds) && (num >= MinMilliSeconds), "Overflow_TimeSpanTooLong");
		return (u64)(num * sTicksPerMillisecond);
	}


	//
	// Summary:
	//     Returns a System.xtimespan whose value is the negated value of this instance.
	//
	// Returns:
	//     The same numeric value as this instance, but with the opposite sign.
	//
	// Exceptions:
	//   System.OverflowException:
	//     The negated value of this instance cannot be represented by a System.xtimespan;
	//     that is, the value of this instance is System.xtimespan.MinValue.
	xtimespan&			xtimespan::negate()
	{
		ASSERTS(mTicks != sMinValue.mTicks, "Overflow_NegateTwosCompNum");
		mTicks = -__ticks();
		return *this;
	}

	//
	// Summary:
	//     Subtracts the specified System.xtimespan from this instance.
	//
	// Parameters:
	//   ts:
	//     A System.xtimespan.
	//
	// Returns:
	//     A System.xtimespan whose value is the result of the value of this instance
	//     minus the value of ts.
	//
	// Exceptions:
	//   System.OverflowException:
	//     The return value is less than System.xtimespan.MinValue or greater than System.xtimespan.MaxValue.
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
