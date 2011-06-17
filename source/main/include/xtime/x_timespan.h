#ifndef __X_TIME_TIMESPAN_H__
#define __X_TIME_TIMESPAN_H__
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
	class xdatetime;

	//==============================================================================
	// Types
	//==============================================================================
	typedef s64 xtick;

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
		xtimespan			operator  - (const xtimespan& inRHS) const					{ xtimespan t(*this); t.substract(inRHS); return t; }
		xtimespan			operator  + (const xtimespan& inRHS) const					{ xtimespan t(*this); t.add(inRHS); return t; }

		///@name Comparison
		xbool				equals(const xtimespan& inRHS) const						{ return sCompare(*this, inRHS) == 0; }
		s32					compareTo(const xtimespan& inRHS) const						{ return sCompare(*this, inRHS); }

		///@name Static Methods
		static xtimespan	sNow();
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

	private:
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

	extern xtimespan		operator  -(const xdatetime& d1, const xdatetime& d2);

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


//==============================================================================
// END __X_TIME_TIMESPAN_H__
//==============================================================================
#endif
