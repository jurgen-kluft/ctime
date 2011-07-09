#ifndef __X_TIME_H__
#define __X_TIME_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
//  INCLUDES
//==============================================================================
#include "xbase\x_target.h"
#include "xbase\x_string_std.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{

	//==============================================================================
	// Classes
	//==============================================================================
	typedef s64 xtick;

	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos
	// Description:
	//     The time class is a handy way to measure how long has past since a particular event.
	//     It is most often used for performance measurement but could also be used for other 
	//     things as well. Usually the game time is based on one of these and then the delta
	//     time is passed to the onAdvanceLogic to each of the objects. 
	//
	//<P>  The functions work as expected. The only tricky ones are the trip function
	//     which are similar to a stop, reset, start sequence of calls. This trip 
	//     function is especially handy to measure performance.
	//
	// Example:
	//<CODE>
	//      void main(void)
	//      {
	//          xticks Acc=0;
	//          xtimer timer;
	//          xtimer wholeTime;
	//
	//          // Start counting how long the whole thing is going to last
	//          wholeTime.start();
	//          
	//          for(s32 i=0; i<100; i++)
	//          {
	//              Acc += timer.trip();   // Note that the first trip will be zero
	//              someFunction();
	//          }
	//          
	//          x_printf("Whole Time %f average time %f", wholeTime.stopSec(), x_TicksToSec(Acc) / (i-1));
	//      }
	//</CODE>
	//------------------------------------------------------------------------------
	class xtimer
	{
	public:
					xtimer              ();

		void        start               ();
		void        reset               ();

		xtick       stop                ();
		xtick       read                () const;
		xtick       trip                ();

		xbool       isRunning           () const;
		s32         getSampleCount      () const;

		f64         stopMs              ();
		f64         stopSec             ();

		f64         readMs              () const;
		f64         readSec             () const;

		f64         tripMs              ();
		f64         tripSec             ();

		f64         getAverageMs        () const;

	protected:

		xtick        mStartTime;
		xtick        mTotalTime;
		xbool        mIsRunning;
		s32          mNumSamples;
	};

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



	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos
	// Description:
	//      Provides a time interval.
	//
	//<P>   
	class xtimespan
	{
	public:
							xtimespan(u64 ticks);
							xtimespan(s32 hours, s32 minutes, s32 seconds);
							xtimespan(s32 days, s32 hours, s32 minutes, s32 seconds);
							xtimespan(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds);

		///@name Binary
		u64					ticks() const;

		///@name Date and Time
		s32					days() const;
		s32					hours() const;
		s32					minutes() const;
		s32					seconds() const;
		s32					milliseconds() const;

		///@name Total (Reference: Year 0000, January 1st, 00:00:00)
		u64					totalDays() const;
		u64					totalHours() const;
		u64					totalMinutes() const;
		u64					totalSeconds() const;
		u64					totalMilliseconds() const;

		xtimespan&			add(const xtimespan& ts);
		xtimespan&			substract(const xtimespan& ts);
		xtimespan&			negate();

		xtimespan			duration() const;

		///@name Operators
		xtimespan&			operator  -=(const xtimespan& inRHS)						{ return substract(inRHS); }
		xtimespan&			operator  +=(const xtimespan& inRHS)						{ return add(inRHS); }
//		xtimespan			operator  -(const xtimespan& inRHS) const					{ xtimespan t(*this); t.substract(inRHS); return t; }
//		xtimespan			operator  +(const xtimespan& inRHS) const					{ xtimespan t(*this); t.add(inRHS); return t; }

		///@name Comparison
		xbool				equals(const xtimespan& inRHS) const						{ return sCompare(*this, inRHS) == 0; }
		s32					compareTo(const xtimespan& inRHS) const						{ return sCompare(*this, inRHS); }

		///@name Static Methods
//		static xtimespan	sNow(); 
		static xtimespan	sFromDays(u64 value);
		static xtimespan	sFromHours(u64 value);
		static xtimespan	sFromMinutes(u64 value);
		static xtimespan	sFromSeconds(u64 value);
		static xtimespan	sFromMilliseconds(u64 value);
		static xtimespan	sFromTicks(u64 value);

		static u64			sTimeToTicks(s32 hours, s32 minutes, s32 seconds);
		static u64			sTimeToTicks(s32 hours, s32 minutes, s32 seconds, s32 milliseconds);
		static u64			sTimeToTicks(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds);

		static s32			sCompare(const xtimespan& t1, const xtimespan& t2);

		static const u64	sTicksPerDay;
		static const u64	sTicksPerHour;
		static const u64	sTicksPerMillisecond;
		static const u64	sTicksPerMinute;
		static const u64	sTicksPerSecond;

		static const xtimespan	sMaxValue;
		static const xtimespan	sMinValue;
		static const xtimespan	sZero;

	protected:
		inline s64			__ticks() const											{ return (s64)mTicks; }

		s64					mTicks;
	};


	// Global operators
	extern xtimespan		operator  -(const xtimespan& t1, const xtimespan& t2);
	extern xtimespan		operator  +(const xtimespan& t1, const xtimespan& t2);

	extern xbool			operator  <(const xtimespan& t1, const xtimespan& t2);
	extern xbool			operator  >(const xtimespan& t1, const xtimespan& t2);
	extern xbool			operator <=(const xtimespan& t1, const xtimespan& t2);
	extern xbool			operator >=(const xtimespan& t1, const xtimespan& t2);
	extern xbool			operator ==(const xtimespan& t1, const xtimespan& t2);
	extern xbool			operator !=(const xtimespan& t1, const xtimespan& t2);


	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos
	// Description:
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
		static xbool			sIsLeapYear(s32 year);

		static s32			sCompare(const xdatetime& t1, const xdatetime& t2);

		static const xdatetime	sMaxValue;
		static const xdatetime	sMinValue;

	private:
		inline s64			__ticks() const													{ return mTicks & X_CONSTANT_64(0x3fffffffffffffff); }

		// 
		xdatetime&			add(s32 value, s32 scale);

		// The platform specific part
		static u64			sGetSystemTime();
		static u64			sGetSystemTimeAsFileTime();
		static u64			sGetSystemTimeFromFileTime(u64 inFileSystemTime);
		static u64			sGetFileTimeFromSystemTime(u64 inSystemTime);

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

	extern xtimespan		operator  -(const xdatetime& d1, const xdatetime& d2);

	extern xbool			operator  <(const xdatetime& t1, const xdatetime& t2);
	extern xbool			operator  >(const xdatetime& t1, const xdatetime& t2);
	extern xbool			operator <=(const xdatetime& t1, const xdatetime& t2);
	extern xbool			operator >=(const xdatetime& t1, const xdatetime& t2);
	extern xbool			operator !=(const xdatetime& d1, const xdatetime& d2);
	extern xbool			operator ==(const xdatetime& d1, const xdatetime& d2);



	//------------------------------------------------------------------------------
	// Author:
	//     Virtuos
	// Description:
	//      This class is a standard wrapper class around the Coordinated Universal 
	//      time (UTC). In other words it contains how many seconds have elapsed 
	//      since midnight (00:00:00), January 1, 1970. So there for it is not very 
	//      accurate only seconds accuracy. The size of the structure is 64 bits to 
	//      avoid failures after January 18, 2038. Make sure to remember that this 
	//      class doesn't deal with any time prior to Jan 1, 1970.
	//
	//------------------------------------------------------------------------------
	class xtime_utc
	{
	public:
		void				updateTime          (void);
		void				clear               (void);

		s32					getSeconds          (void) const;
		s32					getMinutes          (void) const;
		s32					getHours24          (void) const;
		s32					getHours12          (void) const;
		xdatetime			getDate             (void) const;
		u64					getRawData          (void) const    { return miTime; }
		void				setRawData          (u64 inData)    { miTime = inData; }

		xbool				operator  <         (const xtime_utc inTime) const;
		xbool				operator  <=        (const xtime_utc inTime) const;
		xbool				operator  >         (const xtime_utc inTime) const;
		xbool				operator  >=        (const xtime_utc inTime) const;
		xbool				operator  ==        (const xtime_utc inTime) const;

		xtime_utc			operator +          (const xtime_utc inTime) const;
		xtime_utc			operator -          (const xtime_utc inTime) const;

		const xtime_utc&    operator +=         (const xtime_utc inTime);
		const xtime_utc&    operator -=         (const xtime_utc inTime);

	protected:

		u64                    miTime;
	};

	//==============================================================================
	// GLOBAL FUNCTIONS
	//==============================================================================

	xtick   x_GetTime           (void);
	f64     x_GetTimeSec        (void);
	f64     x_TicksToMs         (xtick inTicks);
	f64     x_TicksToSec        (xtick inTicks);
	s64     x_GetTicksPerMs     (void);
	s64     x_GetTicksPerSecond (void);

	//==============================================================================
	// INLINE
	//==============================================================================
	#include "private/x_time_inline.h"


	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


//==============================================================================
// END __X_TIME_H__
//==============================================================================
#endif