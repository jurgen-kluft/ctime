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


	}
}
UNITTEST_SUITE_END
