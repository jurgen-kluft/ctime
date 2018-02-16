
#include "xunittest/xunittest.h"

#include "xtime/x_timer.h"
#include "xtime/x_time.h"
#include "xtime/private/x_time_source.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(timer)
{
	UNITTEST_FIXTURE(main)
	{
		class xtime_source_test : public xtime_source
		{
			xtick			mTicks;
		public:
			xtime_source_test()
				: mTicks(0)
			{

			}

			void			reset()
			{
				mTicks = 0;
			}

			void			set(xtick t)
			{
				mTicks = t;
			}

			void			update(xtick ticks)
			{
				mTicks += ticks;
			}

			virtual xtick	getTimeInTicks()
			{
				return mTicks;
			}

			virtual s64		getTicksPerSecond()
			{
				return 1000 * 1000;
			}
		};

		static xtime_source_test sTimeSource;

		UNITTEST_FIXTURE_SETUP()
		{
			x_SetTimeSource(&sTimeSource);
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
			x_SetTimeSource(NULL);
		}

		UNITTEST_TEST(constructor)
		{
			sTimeSource.reset();

			xtimer timer;
			CHECK_FALSE(timer.isRunning());
			CHECK_EQUAL(0, timer.read());
		}

		UNITTEST_TEST(RealTest)
		{
			x_TimeInit();

			xtimer t1;

			t1.start();
			while (t1.readMs() < 180.0) {
				
			}
			f64 ms1 = t1.stopMs();
			CHECK_TRUE(ms1 > 180.0 && ms1 < 220.0);
			f64 us1 = t1.stopUs();
			CHECK_TRUE(us1 > 180000.0 && us1 < 220000.0);

			f64 ms = t1.readMs();
			CHECK_TRUE(ms > 180.0 && ms < 220.0);
			f64 us = t1.readUs();
			CHECK_TRUE(us > 180000.0 && us < 220000.0);

			x_TimeExit();
			x_SetTimeSource(&sTimeSource);
		}

		UNITTEST_TEST(start)
		{
			sTimeSource.reset();

			xtimer timer;
			CHECK_FALSE(timer.isRunning());
			CHECK_EQUAL(0, timer.read());

			timer.start();
			CHECK_TRUE(timer.isRunning());
		}

		UNITTEST_TEST(read)
		{
			sTimeSource.reset();

			xtimer timer;
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

			xtimer timer;
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

			xtimer timer;
			CHECK_FALSE(timer.isRunning());
			CHECK_EQUAL(0, timer.read());
			CHECK_EQUAL(0, timer.trip());

			timer.start();
			for (xcore::s32 i=0; i<10; ++i)
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
			CHECK_EQUAL(1 + 3*10, timer.getNumTrips());
		}

		UNITTEST_TEST(global_x_GetTime)
		{
			sTimeSource.reset();
			xtick tps = x_GetTicksPerSecond();
			sTimeSource.set(tps);

			xtick time1 = x_GetTime();
			xtick time2 = x_GetTime();

			CHECK_TRUE(time1 == tps);
			CHECK_TRUE(time1 == time2);
		}
		UNITTEST_TEST(global_x_GetTimeSec)
		{
			sTimeSource.reset();
			xtick tps = x_GetTicksPerSecond();
			sTimeSource.set(tps);

			f64 timeSec1 = x_GetTimeSec();
			CHECK_TRUE(timeSec1 == 1);

			f64 timeSec2 = x_GetTimeSec();
			CHECK_TRUE(timeSec1 == timeSec2);
		}
		UNITTEST_TEST(global_x_TicksToUs)
		{
			f64 us1 = x_TicksToUs(200);
			f64 us2 = (((f64)200 * 1000000)/x_GetTicksPerSecond());

			CHECK_TRUE(us1);
			CHECK_TRUE(us1 == us2);
		}
		UNITTEST_TEST(global_x_TicksToMs)
		{
			f64 ms1 = x_TicksToMs(200);
			f64 ms2 = (((f64)200 * 1000)/x_GetTicksPerSecond());

			CHECK_TRUE(ms1);
			CHECK_TRUE(ms1 == ms2);
		}
		UNITTEST_TEST(global_x_TicksToSec)
		{
			f64 ms1 = x_TicksToSec(200);
			f64 ms2 = (f64(200))/x_GetTicksPerSecond();

			CHECK_TRUE(ms1);
			CHECK_TRUE(ms1 == ms2);
		}
	}
}
UNITTEST_SUITE_END
