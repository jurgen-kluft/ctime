#ifndef __CTIME_DATETIME_H__
#define __CTIME_DATETIME_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
	class timespan_t;
	typedef s64 tick_t;

	enum EDayOfWeek
	{
		Sunday = 0,
		Monday = 1,
		Tuesday = 2,
		Wednesday = 3,
		Thursday = 4,
		Friday = 5,
		Saturday = 6,
		DaysPerWeek = 7,
		Sun = Sunday + DaysPerWeek,
		Mon = Monday + DaysPerWeek,
		Tue = Tuesday + DaysPerWeek,
		Wed = Wednesday + DaysPerWeek,
		Thu = Thursday + DaysPerWeek,
		Fri = Friday + DaysPerWeek,
		Sat = Saturday + DaysPerWeek,
	};
	inline EDayOfWeek gAsShortNotation(EDayOfWeek dow)
	{
		if (dow < DaysPerWeek)
			return (EDayOfWeek)((s32)dow + DaysPerWeek);
		else
			return dow;
	}

	enum EMonth
	{
		January = 1,
		February = 2,
		March = 3,
		April = 4,
		May = 5,
		June = 6,
		July = 7,
		August = 8,
		September = 9,
		Oktober = 10,
		November = 11,
		December = 12,
		MonthsPerYear = 12,
		Jan_ = January + MonthsPerYear,
		Feb_ = February + MonthsPerYear,
		Mar_ = March + MonthsPerYear,
		Apr_ = April + MonthsPerYear,
		May_ = May + MonthsPerYear,
		Jun_ = June + MonthsPerYear,
		Jul_ = July + MonthsPerYear,
		Aug_ = August + MonthsPerYear,
		Sep_ = September + MonthsPerYear,
		Okt_ = Oktober + MonthsPerYear,
		Nov_ = November + MonthsPerYear,
		Dec_ = December + MonthsPerYear,
	};
	inline EMonth gAsShortNotation(EMonth month)
	{
		if (month <= MonthsPerYear)
			return (EMonth)((s32)month + MonthsPerYear);
		else
			return month;
	}

	class datetime_t
	{
	public:
		datetime_t();
		datetime_t(u64 ticks);
		datetime_t(s32 year, s32 month, s32 day);
		datetime_t(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second);
		datetime_t(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second, s32 millisecond);

		datetime_t date() const;
		timespan_t timeOfDay() const;
		EDayOfWeek dayOfWeek() const;
		EDayOfWeek dayOfWeekShort() const;
		s32 dayOfYear() const;

		s32 year() const;
		EMonth month() const;
		EMonth monthShort() const;
		s32 day() const;
		s32 hour() const;
		s32 minute() const;
		s32 second() const;
		s32 millisecond() const;

		u64 ticks() const;

		datetime_t &add(const timespan_t &value);

		datetime_t &addYears(s32 value);
		datetime_t &addMonths(s32 months);
		datetime_t &addDays(s32 value);
		datetime_t &addHours(s32 value);
		datetime_t &addMilliseconds(s32 value);
		datetime_t &addMinutes(s32 value);
		datetime_t &addSeconds(s32 value);

		datetime_t &addTicks(u64 value);

		datetime_t &subtract(timespan_t value);
		timespan_t subtract(datetime_t value) const;

		s32 compareTo(const datetime_t &value) const { return sCompare(*this, value); }
		bool equals(const datetime_t &value) const { return sCompare(*this, value) == 0; }

		u64 toBinary() const;
		u64 toFileTime() const;

		void swap(datetime_t &t);

		static datetime_t sNow();	 // Local time
		static datetime_t sNowUtc(); // UTC time
		static datetime_t sToday();

		static datetime_t sFromBinary(u64 binary) { return datetime_t(binary); }
		static datetime_t sFromFileTime(u64 fileTime);

		static s32 sDaysInMonth(s32 year, s32 month);
		static s32 sDaysInYear(s32 year);
		static bool sIsLeapYear(s32 year);

		static s32 sCompare(const datetime_t &t1, const datetime_t &t2);

		static const datetime_t sMaxValue;
		static const datetime_t sMinValue;

	private:
		inline s64 __ticks() const { return mTicks & X_CONSTANT_64(0x3fffffffffffffff); }

		//
		datetime_t &add(s32 value, s32 scale);

		union
		{
			u64 mTicks;
			struct
			{
				u32 mMSB;
				u32 mLSB;
			};
		};
	};

	// Global operators
	extern datetime_t operator-(const datetime_t &d, const timespan_t &t);
	extern datetime_t operator+(const datetime_t &d, const timespan_t &t);

	extern bool operator<(const datetime_t &t1, const datetime_t &t2);
	extern bool operator>(const datetime_t &t1, const datetime_t &t2);
	extern bool operator<=(const datetime_t &t1, const datetime_t &t2);
	extern bool operator>=(const datetime_t &t1, const datetime_t &t2);
	extern bool operator!=(const datetime_t &d1, const datetime_t &d2);
	extern bool operator==(const datetime_t &d1, const datetime_t &d2);

}; // namespace ncore

#endif
