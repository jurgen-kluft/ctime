
#include "cunittest/cunittest.h"

#include "ctime/c_timer.h"
#include "ctime/c_time.h"
#include "ctime/private/c_time_source.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(timer)
{
    UNITTEST_FIXTURE(main)
    {
        class xtime_source_test : public time_source_t
        {
            tick_t mTicks;

        public:
            xtime_source_test()
                : mTicks(0)
            {
            }

            void reset() { mTicks = 0; }

            void set(tick_t t) { mTicks = t; }

            void update(tick_t ticks) { mTicks += ticks; }

            virtual tick_t getTimeInTicks() { return mTicks; }

            virtual s64 getTicksPerSecond() { return 1000 * 1000; }
        };

        static xtime_source_test sTimeSource;

        UNITTEST_FIXTURE_SETUP() { g_SetTimeSource(&sTimeSource); }

        UNITTEST_FIXTURE_TEARDOWN() { g_SetTimeSource(nullptr); }

        UNITTEST_TEST(constructor)
        {
            sTimeSource.reset();

            timer_t timer;
            CHECK_FALSE(timer.isRunning());
            CHECK_EQUAL(0, timer.read());
        }

        UNITTEST_TEST(RealTest)
        {
            ntime::init();

            timer_t t1;

            t1.start();
            float var = 0.0f;
            while (t1.readMs() <= 180.0)
            {
                var += 1.0f;
            }
            f64 ms1 = t1.stopMs();
            CHECK_TRUE(ms1 >= 180.0);
            CHECK_TRUE(var > 1.0f);
            f64 us1 = t1.stopUs();
            CHECK_TRUE(us1 >= 180000.0);

            f64 ms = t1.readMs();
            CHECK_TRUE(ms >= 180.0);
            f64 us = t1.readUs();
            CHECK_TRUE(us >= 180000.0);

            ntime::exit();
            g_SetTimeSource(&sTimeSource);
        }

        UNITTEST_TEST(start)
        {
            sTimeSource.reset();

            timer_t timer;
            CHECK_FALSE(timer.isRunning());
            CHECK_EQUAL(0, timer.read());

            timer.start();
            CHECK_TRUE(timer.isRunning());
        }

        UNITTEST_TEST(read)
        {
            sTimeSource.reset();

            timer_t timer;
            CHECK_FALSE(timer.isRunning());
            CHECK_EQUAL(0, timer.read());

            timer.start();
            CHECK_TRUE(timer.isRunning());
            CHECK_EQUAL(0, timer.read());
            sTimeSource.update(sTimeSource.getTicksPerSecond());
            CHECK_EQUAL(sTimeSource.getTicksPerSecond(), timer.read());
        }

        UNITTEST_TEST(trip)
        {
            sTimeSource.reset();

            timer_t timer;
            CHECK_FALSE(timer.isRunning());
            CHECK_EQUAL(0, timer.read());
            CHECK_EQUAL(0, timer.trip());

            timer.start();
            CHECK_TRUE(timer.isRunning());
            CHECK_EQUAL(0, timer.read());
            CHECK_EQUAL(0, timer.trip());
            sTimeSource.update(sTimeSource.getTicksPerSecond());
            CHECK_EQUAL(sTimeSource.getTicksPerSecond(), timer.read());
            CHECK_EQUAL(sTimeSource.getTicksPerSecond(), timer.trip());

            // Now the timer should have been re-set due to the call to trip()
            CHECK_TRUE(timer.isRunning());
            CHECK_EQUAL(0, timer.read());
            CHECK_EQUAL(0, timer.trip());
        }

        UNITTEST_TEST(getNumTrips)
        {
            sTimeSource.reset();

            timer_t timer;
            CHECK_FALSE(timer.isRunning());
            CHECK_EQUAL(0, timer.read());
            CHECK_EQUAL(0, timer.trip());

            timer.start();
            for (ncore::s32 i = 0; i < 10; ++i)
            {
                CHECK_TRUE(timer.isRunning());
                CHECK_EQUAL(0, timer.read());
                CHECK_EQUAL(0, timer.trip());
                sTimeSource.update(sTimeSource.getTicksPerSecond());
                CHECK_EQUAL(sTimeSource.getTicksPerSecond(), timer.read());
                CHECK_EQUAL(sTimeSource.getTicksPerSecond(), timer.trip());

                // Now the timer should have been re-set due to the call to trip()
                // Every time you call trip, the trip counter is increased with 1
                CHECK_TRUE(timer.isRunning());
                CHECK_EQUAL(0, timer.read());
                CHECK_EQUAL(0, timer.trip());
            }
            CHECK_EQUAL(1 + 3 * 10, timer.getNumTrips());
        }

        UNITTEST_TEST(global_x_GetTime)
        {
            sTimeSource.reset();
            tick_t tps = getTicksPerSecond();
            sTimeSource.set(tps);

            tick_t time1 = getTime();
            tick_t time2 = getTime();

            CHECK_TRUE(time1 == tps);
            CHECK_TRUE(time1 == time2);
        }
        UNITTEST_TEST(global_x_GetTimeSec)
        {
            sTimeSource.reset();
            tick_t tps = getTicksPerSecond();
            sTimeSource.set(tps);

            f64 timeSec1 = getTimeSec();
            CHECK_TRUE(timeSec1 == 1);

            f64 timeSec2 = getTimeSec();
            CHECK_TRUE(timeSec1 == timeSec2);
        }
        UNITTEST_TEST(global_x_TicksToUs)
        {
            f64 us1 = ticksToUs(200);
            f64 us2 = (((f64)200 * 1000000) / getTicksPerSecond());

            CHECK_TRUE(us1);
            CHECK_TRUE(us1 == us2);
        }
        UNITTEST_TEST(global_x_TicksToMs)
        {
            f64 ms1 = ticksToMs(200);
            f64 ms2 = (((f64)200 * 1000) / getTicksPerSecond());

            CHECK_TRUE(ms1);
            CHECK_TRUE(ms1 == ms2);
        }
        UNITTEST_TEST(global_x_TicksToSec)
        {
            f64 ms1 = ticksToSec(200);
            f64 ms2 = (f64(200)) / getTicksPerSecond();

            CHECK_TRUE(ms1);
            CHECK_TRUE(ms1 == ms2);
        }
    }
}
UNITTEST_SUITE_END
