#include "xbase\x_types.h"
#include "xunittest\xunittest.h"

#include "xtime\x_time.h"
#include "xtime\x_timer.h"

#include "xtime\private\x_time_source.h"

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

			virtual s64		getTicksPerMilliSecond()
			{
				return 1000;
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
		UNITTEST_TEST(global_x_TicksToMs)
		{
			f64 ms1 = x_TicksToMs(200);
			f64 ms2 = ((f64)200)/x_GetTicksPerMs();

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
		UNITTEST_TEST(global_x_GetTicksPerMs)
		{
			s64 tickPerMs1 = x_GetTicksPerMs();
			s64 tickPerMs2 = x_GetTicksPerSecond();

			CHECK_TRUE(tickPerMs1);
			CHECK_TRUE(tickPerMs1 == tickPerMs2/1000);
		}
		UNITTEST_TEST(global_x_GetTicksPerSecond)
		{
			s64 tickPerSecond1 = x_GetTicksPerSecond();
			s64 tickPerSecond2 = x_GetTicksPerMs();

			CHECK_TRUE(tickPerSecond1);
			CHECK_TRUE(tickPerSecond1/1000 == tickPerSecond2);
		}
	}
}
UNITTEST_SUITE_END
