#ifndef __X_TIME_DATETIME_H__
#define __X_TIME_DATETIME_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
//  INCLUDES
//==============================================================================
#include "xbase\x_types.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//==============================================================================
	// Classes
	//==============================================================================
	class xtimespan;

	//==============================================================================
	// Types
	//==============================================================================
	typedef s64 xtick;

	enum EDayOfWeek
	{
		Sunday		= 0,
		Monday		= 1,
		Tuesday		= 2,
		Wednesday	= 3,
		Thursday	= 4,
		Friday		= 5,
		Saturday	= 6,
		DaysPerWeek = 7,
		Sun			= Sunday+DaysPerWeek,
		Mon			= Monday+DaysPerWeek,
		Tue			= Tuesday+DaysPerWeek,
		Wed			= Wednesday+DaysPerWeek,
		Thu			= Thursday+DaysPerWeek,
		Fri			= Friday+DaysPerWeek,
		Sat			= Saturday+DaysPerWeek,
	};
	inline EDayOfWeek	gAsShortNotation(EDayOfWeek dow)
	{
		if (dow<DaysPerWeek) return (EDayOfWeek)((s32)dow + DaysPerWeek);
		else return dow;
	}

	enum EMonth
	{
		January		= 1,
		February	= 2,
		March		= 3,
		April		= 4,
		May			= 5,
		June		= 6,
		July		= 7,
		August		= 8,
		September	= 9,
		Oktober		= 10,
		November	= 11,
		December	= 12,
		MonthsPerYear = 12,
		Jan_		= January + MonthsPerYear,
		Feb_		= February + MonthsPerYear,
		Mar_		= March + MonthsPerYear,
		Apr_		= April + MonthsPerYear,
		May_		= May + MonthsPerYear,
		Jun_		= June  + MonthsPerYear,
		Jul_		= July + MonthsPerYear,
		Aug_		= August + MonthsPerYear,
		Sep_		= September + MonthsPerYear,
		Okt_		= Oktober + MonthsPerYear,
		Nov_		= November + MonthsPerYear,
		Dec_		= December + MonthsPerYear,
	};
	inline EMonth	gAsShortNotation(EMonth month)
	{
		if (month<=MonthsPerYear) return (EMonth)((s32)month + MonthsPerYear);
		else return month;
	}

	class xdatetime_source
	{
	public:
		// The platform specific part
		virtual u64			getSystemTime() = 0;
		virtual u64			getSystemTimeAsFileTime() = 0;
		virtual u64			getSystemTimeFromFileTime(u64 inFileSystemTime) = 0;
		virtual u64			getFileTimeFromSystemTime(u64 inSystemTime) = 0;
	};

	extern void				x_SetDateTimeSource(xdatetime_source*);

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos
	// Description:
	//     A class for date and time operations
	//------------------------------------------------------------------------------
	class xdatetime
	{
	public:
							xdatetime();
							xdatetime(u64 ticks);
							xdatetime(s32 year, s32 month, s32 day);
							xdatetime(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second);
							xdatetime(s32 year, s32 month, s32 day, s32 hour, s32 minute, s32 second, s32 millisecond);

		xdatetime			date() const;
		xtimespan			timeOfDay() const;
		EDayOfWeek			dayOfWeek() const;
		EDayOfWeek			dayOfWeekShort() const;
		s32					dayOfYear() const;

		s32					year() const;
		EMonth				month() const;
		EMonth				monthShort() const;
		s32					day() const;
		s32					hour() const;
		s32					minute() const;
		s32					second() const;
		s32					millisecond() const;

		u64					ticks() const;

		xdatetime&			add(const xtimespan& value);

		xdatetime&			addYears(s32 value);
		xdatetime&			addMonths(s32 months);
		xdatetime&			addDays(s32 value);
		xdatetime&			addHours(s32 value);
		xdatetime&			addMilliseconds(s32 value);
		xdatetime&			addMinutes(s32 value);
		xdatetime&			addSeconds(s32 value);

		xdatetime&			addTicks(u64 value);

		xdatetime&			subtract(xtimespan value);
		xtimespan			subtract(xdatetime value) const;

		s32					compareTo(const xdatetime& value) const							{ return sCompare(*this, value); }
		xbool				equals(const xdatetime& value) const							{ return sCompare(*this, value)==0; }

		u64					toBinary() const;
		u64					toFileTime() const;

		static xdatetime	sNow();
		static xdatetime	sToday();

		static xdatetime	sFromBinary(u64 binary)											{ return xdatetime(binary); }
		static xdatetime	sFromFileTime(u64 fileTime);

		static s32			sDaysInMonth(s32 year, s32 month);
		static s32			sDaysInYear(s32 year);
		static xbool		sIsLeapYear(s32 year);

		static s32			sCompare(const xdatetime& t1, const xdatetime& t2);

		static const xdatetime	sMaxValue;
		static const xdatetime	sMinValue;

	private:
		inline s64			__ticks() const													{ return mTicks & X_CONSTANT_64(0x3fffffffffffffff); }

		// 
		xdatetime&			add(s32 value, s32 scale);

		union
		{
			u64				mTicks;
			struct
			{
				u32				mMSB;
				u32				mLSB;
			};
		};
	};

	// Global operators
	extern xdatetime		operator  -(const xdatetime&  d, const xtimespan&  t);
	extern xdatetime		operator  +(const xdatetime&  d, const xtimespan&  t);

	extern xbool			operator  <(const xdatetime& t1, const xdatetime& t2);
	extern xbool			operator  >(const xdatetime& t1, const xdatetime& t2);
	extern xbool			operator <=(const xdatetime& t1, const xdatetime& t2);
	extern xbool			operator >=(const xdatetime& t1, const xdatetime& t2);
	extern xbool			operator !=(const xdatetime& d1, const xdatetime& d2);
	extern xbool			operator ==(const xdatetime& d1, const xdatetime& d2);


	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


//==============================================================================
// END __X_TIME_DATETIME_H__
//==============================================================================
#endif
