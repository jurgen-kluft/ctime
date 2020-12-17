#ifndef __X_TIME_TIMESPAN_H__
#define __X_TIME_TIMESPAN_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
    class datetime_t;

    typedef s64 tick_t;

    class timespan_t
    {
    public:
        timespan_t(u64 ticks);
        timespan_t(s32 hours, s32 minutes, s32 seconds);
        timespan_t(s32 days, s32 hours, s32 minutes, s32 seconds);
        timespan_t(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds);

        ///@name Binary
        u64 ticks() const;

        ///@name Date and Time
        s32 days() const;
        s32 hours() const;
        s32 minutes() const;
        s32 seconds() const;
        s32 milliseconds() const;

        ///@name Total (Reference: Year 0000, January 1st, 00:00:00)
        u64 totalDays() const;
        u64 totalHours() const;
        u64 totalMinutes() const;
        u64 totalSeconds() const;
        u64 totalMilliseconds() const;

        timespan_t &add(const timespan_t &ts);
        timespan_t &substract(const timespan_t &ts);
        timespan_t &negate();

        timespan_t duration() const;

        ///@name Operators
        timespan_t &operator-=(const timespan_t &inRHS) { return substract(inRHS); }
        timespan_t &operator+=(const timespan_t &inRHS) { return add(inRHS); }

        ///@name Comparison
        bool equals(const timespan_t &inRHS) const { return sCompare(*this, inRHS) == 0; }
        s32 compareTo(const timespan_t &inRHS) const { return sCompare(*this, inRHS); }

        ///@name Static Methods
        static timespan_t sNow();
        static timespan_t sFromDays(u64 value);
        static timespan_t sFromHours(u64 value);
        static timespan_t sFromMinutes(u64 value);
        static timespan_t sFromSeconds(u64 value);
        static timespan_t sFromMilliseconds(u64 value);
        static timespan_t sFromTicks(u64 value);

        static u64 sTimeToTicks(s32 hours, s32 minutes, s32 seconds);
        static u64 sTimeToTicks(s32 hours, s32 minutes, s32 seconds, s32 milliseconds);
        static u64 sTimeToTicks(s32 days, s32 hours, s32 minutes, s32 seconds, s32 milliseconds);

        static s32 sCompare(const timespan_t &t1, const timespan_t &t2);

        static const u64 sTicksPerDay;
        static const u64 sTicksPerHour;
        static const u64 sTicksPerMillisecond;
        static const u64 sTicksPerMinute;
        static const u64 sTicksPerSecond;

        static const s32 sMillisPerDay;
        static const s32 sMillisPerHour;
        static const s32 sMillisPerMinute;
        static const s32 sMillisPerSecond;

        static const timespan_t sMaxValue;
        static const timespan_t sMinValue;
        static const timespan_t sZero;

    private:
        inline s64 __ticks() const { return (s64)mTicks; }

        s64 mTicks;
    };

    extern timespan_t operator-(const timespan_t &t1, const timespan_t &t2);
    extern timespan_t operator+(const timespan_t &t1, const timespan_t &t2);

    extern bool operator<(const timespan_t &t1, const timespan_t &t2);
    extern bool operator>(const timespan_t &t1, const timespan_t &t2);
    extern bool operator<=(const timespan_t &t1, const timespan_t &t2);
    extern bool operator>=(const timespan_t &t1, const timespan_t &t2);
    extern bool operator==(const timespan_t &t1, const timespan_t &t2);
    extern bool operator!=(const timespan_t &t1, const timespan_t &t2);

    extern timespan_t operator-(const datetime_t &d1, const datetime_t &d2);

}; // namespace xcore

#endif
