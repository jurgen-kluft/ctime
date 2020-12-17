#ifndef __X_TIME_SOURCE_H__
#define __X_TIME_SOURCE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace xcore
{
    class time_source_t
    {
    public:
        virtual ~time_source_t() {}

        virtual s64 getTimeInTicks() = 0;
        virtual s64 getTicksPerSecond() = 0;
    };

    extern void x_SetTimeSource(time_source_t *);

}; // namespace xcore

#endif
