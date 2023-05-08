#include "ccore/c_debug.h"

#include "ctime/c_time.h"
#include "ctime/c_timespan.h"
#include "ctime/c_datetime.h"

#include "ctime/private/c_time_source.h"
#include "ctime/private/c_datetime_source.h"

namespace ncore
{
    namespace ntime
    {
        static time_source_t* sTimeSource;
    };

    void g_SetTimeSource(time_source_t* src) { ntime::sTimeSource = src; }

    tick_t g_GetTime(void) { return ntime::sTimeSource->getTimeInTicks(); }

    s64 g_GetTicksPerSecond(void) { return ntime::sTimeSource->getTicksPerSecond(); }

    /**
     * datetime_t source
     */
    static datetime_source_t* sDateTimeSource = nullptr;
    void                      g_SetDateTimeSource(datetime_source_t* src) { sDateTimeSource = src; }

    /**
     * datetime_t
     */

    static const s32 DaysPer100Years = 36524;
    static const s32 DaysPer400Years = 146097;
    static const s32 DaysPer4Years   = 1461;
    static const s32 DaysPerYear     = 365;

    // static const s32 DaysTo10000			= 3652059;
    // static const s32 DaysTo1601			= 584388;
    // static const s32 DaysTo1899			= 693593;

    // static const u64 MinTicks				= 0;
    static const u64 MaxTicks = D_CONSTANT_64(0x2bca2875f4373fff);

#ifdef D_ASSERT
    static const s64 MaxMillis = D_CONSTANT_64(0x11efae44cb400);
#endif

    static const s64 TicksPerDay         = D_CONSTANT_64(0xc92a69c000);
    static const s64 TicksPerHour        = D_CONSTANT_64(0x861c46800);
    static const s64 TicksPerMicrosecond = 10;
    static const s64 TicksPerMillisecond = 10000;
    static const s64 TicksPerMinute      = 600000000;
    static const s64 TicksPerSecond      = 10000000;

    static const s32 sDaysToMonth365[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    static const s32 sDaysToMonth366[] = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366};

    static s64 sDateToTicks(s32 year, s32 month, s32 day)
    {
        if (((year >= 1) && (year <= 9999)) && ((month >= 1) && (month <= 12)))
        {
            const s32* daysToMonth = datetime_t::sIsLeapYear(year) ? sDaysToMonth366 : sDaysToMonth365;
            if ((day >= 1) && (day <= (daysToMonth[month] - daysToMonth[month - 1])))
            {
                s32 yearMinusOne = year - 1;
                s32 numDays      = ((((((yearMinusOne * DaysPerYear) + (yearMinusOne / 4)) - (yearMinusOne / 100)) + (yearMinusOne / 400)) + daysToMonth[month - 1]) + day) - 1;
                return (numDays * TicksPerDay);
            }
        }
        ASSERTS(false, "Invalid input!");
        return 0;
    }

    static s64 sTimeToTicks(s32 hour, s32 minute, s32 second)
    {
        ASSERTS(((((hour >= 0) && (hour < 24)) && ((minute >= 0) && (minute < 60))) && ((second >= 0) && (second < 60))), "Invalid input!");
        return timespan_t::sTimeToTicks(hour, minute, second);
    }

    enum EDatePart
    {
        DatePartYear      = 0,
        DatePartDayOfYear = 1,
        DatePartMonth     = 2,
        DatePartDay       = 3,
    };
    s32 sGetDatePart(EDatePart part, s64 ticks)
    {
        s32 num2 = (s32)(ticks / TicksPerDay);
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
        s32        index    = num2 >> 6;
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

    const datetime_t datetime_t::sMaxValue(MaxTicks);
    const datetime_t datetime_t::sMinValue(0);

    datetime_t::datetime_t()
        : mTicks(sMinValue.mTicks)
    {
        ASSERTS(mTicks <= MaxTicks, "Error: out of range!");
    }

    datetime_t::datetime_t(u64 ticks)
        : mTicks(ticks)
    {
        ASSERTS(mTicks <= MaxTicks, "Error: out of range!");
    }

    /**
     *   month: 		The month (1 through 12).
     *   day:			The day (1 through the number of days in month).
     *   year:			The year (1 through 9999).
     */
    datetime_t::datetime_t(s32 year, s32 month, s32 day) { mTicks = sDateToTicks(year, month, day); }

    /**
     *    month:			The month (1 through 12).
     *    day:			The day (1 through the number of days in month).
     *    minute:		The minutes (0 through 59).
     *    year:			The year (1 through 9999).
     *    hour:			The hours (0 through 23).
     *    second:		The seconds (0 through 59).
     */
    datetime_t::datetime_t(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second) { mTicks = (s64)(sDateToTicks(year, month, day) + sTimeToTicks(hour, minute, second)); }

    /**
     *    month:			The month (1 through 12).
     *    millisecond:	The milliseconds (0 through 999).
     *    day:			The day (1 through the number of days in month).
     *    minute:		The minutes (0 through 59).
     *    year:			The year (1 through 9999).
     *    hour:			The hours (0 through 23).
     *    second:		The seconds (0 through 59).
     */
    datetime_t::datetime_t(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second, s32 millisecond)
    {
        s64 num = sDateToTicks(year, month, day) + sTimeToTicks(hour, minute, second);
        ASSERTS((millisecond >= 0) && (millisecond < timespan_t::sMillisPerSecond), "Invalid input!");
        num += millisecond * TicksPerMillisecond;
        ASSERTS((num >= 0) && (num < MaxTicks), "Out of range!");
        mTicks = num;
    }

    /**
     *  Summary:
     *      Gets the date component of this instance.
     *
     *  Returns:
     *      A new System.datetime_t with the same date as this instance, and the time value
     *      set to 12:00:00 midnight (00:00:00).
     */
    datetime_t datetime_t::date() const
    {
        s64 t = __ticks();
        return datetime_t(((u64)(t - (t % TicksPerDay))));
    }

    /**
     *  Summary:
     *      Gets the day of the month represented by this instance.
     *
     *  Returns:
     *      The day component, expressed as a value between 1 and 31.
     */
    s32 datetime_t::day() const { return sGetDatePart(DatePartDay, __ticks()); }

    /**
     *  Summary:
     *      Gets the day of the week represented by this instance.
     *
     *  Returns:
     *      A System.DayOfWeek enumerated constant that indicates the day of the week.
     *      This property value ranges from zero, indicating Sunday, to six, indicating
     *      Saturday.
     */
    EDayOfWeek datetime_t::dayOfWeek() const
    {
        s32        days      = (s32)((__ticks() / TicksPerDay) + 1);
        EDayOfWeek dayOfWeek = (EDayOfWeek)(days % 7);
        return dayOfWeek;
    }
    EDayOfWeek datetime_t::dayOfWeekShort() const
    {
        s32        days      = (s32)((__ticks() / TicksPerDay) + 1);
        EDayOfWeek dayOfWeek = (EDayOfWeek)(days % 7);
        return (EDayOfWeek)(dayOfWeek + DaysPerWeek);
    }

    /**
     *  Summary:
     *      Gets the day of the year represented by this instance.
     *
     *  Returns:
     *      The day of the year, expressed as a value between 1 and 366.
     */
    s32 datetime_t::dayOfYear() const { return sGetDatePart(DatePartDayOfYear, __ticks()); }

    /**
     *  Summary:
     *      Gets the hour component of the date represented by this instance.
     *
     *  Returns:
     *      The hour component, expressed as a value between 0 and 23.
     */
    s32 datetime_t::hour() const { return (s32)((__ticks() / TicksPerHour) % ((s64)24)); }

    /**
     *  Summary:
     *      Gets the milliseconds component of the date represented by this instance.
     *
     *  Returns:
     *      The milliseconds component, expressed as a value between 0 and 999.
     */
    s32 datetime_t::millisecond() const { return (s32)((__ticks() / ((s64)TicksPerMillisecond)) % ((s64)timespan_t::sMillisPerSecond)); }

    /**
     *  Summary:
     *      Gets the minute component of the date represented by this instance.
     *
     *  Returns:
     *      The minute component, expressed as a value between 0 and 59.
     */
    s32 datetime_t::minute() const { return (s32)((__ticks() / ((s64)TicksPerMinute)) % ((s64)60)); }

    /**
     *  Summary:
     *      Gets the month component of the date represented by this instance.
     *
     *  Returns:
     *      The month component, expressed as a value between 1 and 12.
     */
    EMonth datetime_t::month() const { return (EMonth)sGetDatePart(DatePartMonth, __ticks()); }
    EMonth datetime_t::monthShort() const { return (EMonth)(sGetDatePart(DatePartMonth, __ticks()) + MonthsPerYear); }
    /**
     *  Summary:
     *      Gets a System.datetime_t object that is set to the current date and time on
     *      this computer, expressed as the local time.
     *
     *  Returns:
     *      A System.datetime_t whose value is the current local date and time.
     */
    datetime_t datetime_t::sNow() { return datetime_t(sDateTimeSource->getSystemTimeLocal()); }

    /**
     *  Summary:
     *      Gets a System.datetime_t object that is set to the current date and time on
     *      this computer, expressed as the UTC time.
     *
     *  Returns:
     *      A System.datetime_t whose value is the UTC local date and time.
     */
    datetime_t datetime_t::sNowUtc() { return datetime_t(sDateTimeSource->getSystemTimeUtc()); }

    /**
     *  Summary:
     *      Gets the seconds component of the date represented by this instance.
     *
     *  Returns:
     *      The seconds, between 0 and 59.
     */
    s32 datetime_t::second() const { return (s32)((__ticks() / ((s64)TicksPerSecond)) % ((s64)60)); }

    /**
     *  Summary:
     *      Gets the number of ticks that represent the date and time of this instance.
     *
     *  Returns:
     *      The number of ticks that represent the date and time of this instance. The
     *      value is between System.datetime_t.MinValue and System.datetime_t.MaxValue.
     */
    u64 datetime_t::ticks() const { return __ticks(); }

    /**
     *  Summary:
     *      Gets the time of day for this instance.
     *
     *  Returns:
     *      A System.timespan_t that represents the fraction of the day that has elapsed
     *      since midnight.
     */
    timespan_t datetime_t::timeOfDay() const { return timespan_t(__ticks() % TicksPerDay); }

    /**
     *  Summary:
     *      Gets the current date.
     *
     *  Returns:
     *      A System.datetime_t set to today's date, with the time component set to 00:00:00.
     */
    datetime_t datetime_t::sToday() { return sNow().date(); }

    /**
     *  Summary:
     *      Gets the year component of the date represented by this instance.
     *
     *  Returns:
     *      The year, between 1 and 9999.
     */
    s32 datetime_t::year() const { return sGetDatePart(DatePartYear, __ticks()); }

    /**
     *  Summary:
     *      Adds the value of the specified System.timespan_t to the value of this instance.
     *
     *  Parameters:
     *    value:
     *      A System.timespan_t that contains the interval to add.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and the time interval represented by value.
     */
    datetime_t& datetime_t::add(const timespan_t& value) { return addTicks(value.ticks()); }

    /**
     *  Summary:
     *      Adds the specified number of days to the value of this instance.
     *
     *  Parameters:
     *    value:
     *      A number of whole and fractional days. The value parameter can be negative
     *      or positive.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and the number of days represented by value.
     */
    datetime_t& datetime_t::addDays(s32 value) { return add(value, timespan_t::sMillisPerDay); }

    /**
     *  Summary:
     *      Adds the specified number of hours to the value of this instance.
     *
     *  Parameters:
     *    value:
     *      A number of whole and fractional hours. The value parameter can be negative
     *      or positive.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and the number of hours represented by value.
     */
    datetime_t& datetime_t::addHours(s32 value) { return add(value, timespan_t::sMillisPerHour); }

    /**
     *  Summary:
     *      Adds the specified number of milliseconds to the value of this instance.
     *
     *  Parameters:
     *    value:
     *      A number of whole and fractional milliseconds. The value parameter can be
     *      negative or positive.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and the number of milliseconds represented by value.
     */
    datetime_t& datetime_t::addMilliseconds(s32 value) { return add(value, 1); }

    /**
     *  Summary:
     *      Adds the specified number of minutes to the value of this instance.
     *
     *  Parameters:
     *    value:
     *      A number of whole and fractional minutes. The value parameter can be negative
     *      or positive.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and the number of minutes represented by value.
     */
    datetime_t& datetime_t::addMinutes(s32 value) { return add(value, timespan_t::sMillisPerMinute); }

    /**
     *  Summary:
     *      Adds the specified number of months to the value of this instance.
     *
     *  Parameters:
     *    months:
     *      A number of months. The months parameter can be negative or positive.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and months.
     */

    datetime_t& datetime_t::addMonths(s32 months)
    {
        ASSERTS((months >= -120000) && (months <= 120000), "ArgumentOutOfRange_DateTimeBadMonths");

        s32 year  = sGetDatePart(DatePartYear, __ticks());
        s32 month = sGetDatePart(DatePartMonth, __ticks());
        s32 day   = sGetDatePart(DatePartDay, __ticks());
        s32 num4  = (month - 1) + months;
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
        mTicks = (((u64)(sDateToTicks(year, month, day) + (__ticks() % TicksPerDay))));
        return *this;
    }

    /**
     *  Summary:
     *      Adds the specified number of seconds to the value of this instance.
     *
     *  Parameters:
     *    value:
     *      A number of whole and fractional seconds. The value parameter can be negative
     *      or positive.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and the number of seconds represented by value.
     */
    datetime_t& datetime_t::addSeconds(s32 value) { return add(value, timespan_t::sMillisPerSecond); }

    /**
     *  Summary:
     *      Adds the specified number of ticks to the value of this instance.
     *
     *  Parameters:
     *    value:
     *      A number of 100-nanosecond ticks. The value parameter can be positive or
     *      negative.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and the time represented by value.
     */
    datetime_t& datetime_t::addTicks(u64 value)
    {
        s64 ticks = __ticks();
        ASSERTS(((s64)value <= (s64)(MaxTicks - ticks)) && ((s64)value >= -ticks), "ArgumentOutOfRange_DateArithmetic");
        mTicks = ((u64)(ticks + value));
        return *this;
    }

    /**
     *  Summary:
     *      Adds the specified number of years to the value of this instance.
     *
     *  Parameters:
     *    value:
     *      A number of years. The value parameter can be negative or positive.
     *
     *  Returns:
     *      A System.datetime_t whose value is the sum of the date and time represented
     *      by this instance and the number of years represented by value.
     */
    datetime_t& datetime_t::addYears(s32 value)
    {
        ASSERTS((value >= -10000) && (value <= TicksPerMillisecond), "ArgumentOutOfRange_DateTimeBadYears");
        return addMonths(value * 12);
    }

    /**
     *  Summary:
     *      Compares two instances of System.datetime_t and returns an indication of their
     *      relative values.
     *
     *  Parameters:
     *    t2:
     *      The second System.datetime_t.
     *
     *    t1:
     *      The first System.datetime_t.
     *
     *  Returns:
     *      A signed number indicating the relative values of t1 and t2.Value Type Condition
     *      Less than zero t1 is less than t2. Zero t1 equals t2. Greater than zero t1
     *      is greater than t2.
     */
    s32 datetime_t::sCompare(const datetime_t& t1, const datetime_t& t2)
    {
        s64 __ticks = t1.__ticks();
        s64 num2    = t2.__ticks();
        if (__ticks > num2)
            return 1;
        else if (__ticks < num2)
            return -1;
        else
            return 0;
    }

    /**
     *  Summary:
     *      Returns the number of days in the specified month and year.
     *
     *  Parameters:
     *    month:
     *      The month (a number ranging from 1 to 12).
     *
     *    year:
     *      The year.
     *
     *  Returns:
     *      The number of days in month for the specified year.For example, if month
     *      equals 2 for February, the return value is 28 or 29 depending upon whether
     *      year is a leap year.
     */
    s32 datetime_t::sDaysInMonth(s32 year, s32 month)
    {
        ASSERTS((month >= 1) && (month <= 12), "ArgumentOutOfRange_Month");
        const s32* numArray = sIsLeapYear(year) ? sDaysToMonth366 : sDaysToMonth365;
        return (numArray[month] - numArray[month - 1]);
    }

    /**
     *  Summary:
     *      Returns the number of days in the specified year.
     *
     *  Parameters:
     *    month:
     *      The month (a number ranging from 1 to 12).
     *
     *    year:
     *      The year.
     *
     *  Returns:
     *      The number of days for the specified year.
     */
    s32 datetime_t::sDaysInYear(s32 year) { return sIsLeapYear(year) ? (DaysPerYear + 1) : DaysPerYear; }

    /**
     *  Summary:
     *      Converts the specified Windows file time to an equivalent local time.
     *
     *  Parameters:
     *    fileTime:
     *      A Windows file time expressed in ticks.
     *
     *  Returns:
     *      A System.datetime_t object that represents a local time equivalent to the date
     *      and time represented by the fileTime parameter.
     *
     *  Exceptions:
     *    System.ArgumentOutOfRangeException:
     *      fileTime is less than 0 or represents a time greater than System.datetime_t.MaxValue.
     */
    datetime_t datetime_t::sFromFileTime(u64 fileTime)
    {
        ASSERTS(fileTime <= MaxTicks, "ArgumentOutOfRange_FileTimeInvalid");
        u64 systemTime = sDateTimeSource->getSystemTimeFromFileTime(fileTime);
        return datetime_t(systemTime);
    }

    /**
     *  Summary:
     *      Returns an indication whether the specified year is a leap year.
     *
     *  Parameters:
     *    year:
     *      A 4-digit year.
     *
     *  Returns:
     *      true if year is a leap year; otherwise, false.
     */
    bool datetime_t::sIsLeapYear(s32 year)
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

    /**
     *  Summary:
     *      Subtracts the specified date and time from this instance.
     *
     *  Parameters:
     *    value:
     *      An instance of System.datetime_t.
     *
     *  Returns:
     *      A System.timespan_t interval equal to the date and time represented by this
     *      instance minus the date and time represented by value.
     */
    timespan_t datetime_t::subtract(datetime_t value) const { return timespan_t(__ticks() - value.__ticks()); }

    /**
     *  Summary:
     *      Subtracts the specified duration from this instance.
     *
     *  Parameters:
     *    value:
     *      An instance of System.timespan_t.
     *
     *  Returns:
     *      A System.datetime_t equal to the date and time represented by this instance
     *      minus the time interval represented by value.
     */
    datetime_t& datetime_t::subtract(timespan_t value)
    {
        s64 ticks = __ticks();
        s64 num2  = value.ticks();
        ASSERTS((ticks >= num2) && ((s64)(ticks - MaxTicks) <= num2), "ArgumentOutOfRange_DateArithmetic");
        mTicks = ((u64)(ticks - num2));
        return *this;
    }

    /**
     *  Summary:
     *      Serializes the current System.datetime_t object to a 64-bit binary value that
     *      subsequently can be used to recreate the System.datetime_t object.
     *
     *  Returns:
     *      A 64-bit signed integer that encodes the System.datetime_t.Kind and System.datetime_t.ticks
     *      properties.
     */
    u64 datetime_t::toBinary() const { return __ticks(); }

    /**
     *  Summary:
     *      Converts the value of the current System.datetime_t object to a Windows file
     *      time.
     *
     *  Returns:
     *      The value of the current System.datetime_t object expressed as a Windows file
     *      time.
     */
    u64 datetime_t::toFileTime() const
    {
        s64 fileTime = sDateTimeSource->getFileTimeFromSystemTime(__ticks());
        ASSERTS((fileTime >= 0) && (fileTime <= MaxTicks), "ArgumentOutOfRange_FileTimeInvalid");
        return (u64)fileTime;
    }

    void datetime_t::swap(datetime_t& t)
    {
        u64 tmp  = mTicks;
        mTicks   = t.mTicks;
        t.mTicks = tmp;
    }

    datetime_t& datetime_t::add(s32 value, s32 scale)
    {
        s64 num = (s64)value * scale;
        ASSERTS((num > D_CONSTANT_64(-315537897600000)) && (num < MaxMillis), "Argument Out Of Range, AddValue");
        return addTicks(num * TicksPerMillisecond);
    }

    /**
     * Global operators
     */
    datetime_t operator-(const datetime_t& d, const timespan_t& t)
    {
        s64 ticks = d.ticks() - t.ticks();
        return datetime_t(ticks);
    }

    datetime_t operator+(const datetime_t& d, const timespan_t& t)
    {
        s64 ticks = d.ticks() + t.ticks();
        return datetime_t(ticks);
    }

    timespan_t operator-(const datetime_t& d1, const datetime_t& d2)
    {
        s64 ticks = d1.ticks() - d2.ticks();
        return timespan_t(ticks);
    }

    bool operator<(const datetime_t& t1, const datetime_t& t2) { return t1.ticks() < t2.ticks(); }
    bool operator>(const datetime_t& t1, const datetime_t& t2) { return t1.ticks() > t2.ticks(); }
    bool operator<=(const datetime_t& t1, const datetime_t& t2) { return t1.ticks() <= t2.ticks(); }
    bool operator>=(const datetime_t& t1, const datetime_t& t2) { return t1.ticks() >= t2.ticks(); }
    bool operator!=(const datetime_t& d1, const datetime_t& d2) { return d1.ticks() != d2.ticks(); }
    bool operator==(const datetime_t& d1, const datetime_t& d2) { return d1.ticks() == d2.ticks(); }

}; // namespace ncore
