#ifndef __CTIME_SOURCE_H__
#define __CTIME_SOURCE_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

namespace ncore
{
    class time_source_t
    {
    public:
        virtual ~time_source_t() {}

        virtual s64 getTimeInTicks() = 0;
        virtual s64 getTicksPerSecond() = 0;
    };

    extern void g_SetTimeSource(time_source_t *);

}; // namespace ncore

#endif
