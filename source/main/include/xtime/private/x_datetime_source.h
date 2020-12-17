#ifndef __X_TIME_DATETIME_SOURCE_H__
#define __X_TIME_DATETIME_SOURCE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
    class datetime_source_t
    {
    public:
        virtual ~datetime_source_t() {}

        // The platform specific part
        virtual u64 getSystemTimeUtc() = 0;
        virtual u64 getSystemTimeLocal() = 0;
        virtual s64 getSystemTimeZone() = 0;
        virtual u64 getSystemTimeAsFileTime() = 0;
        virtual u64 getSystemTimeFromFileTime(u64 inFileSystemTime) = 0;
        virtual u64 getFileTimeFromSystemTime(u64 inSystemTime) = 0;
    };

    extern void x_SetDateTimeSource(datetime_source_t *);

}; // namespace xcore

#endif
