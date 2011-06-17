#include "xbase\x_types.h"
#include "xunittest\xunittest.h"

#include "xtime\x_datetime.h"

#include "xtime\private\x_datetime_source.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(datetime)
{
	UNITTEST_FIXTURE(main)
	{
		class xdatetime_source_test : public xdatetime_source
		{
			u64					mDateTimeTicks;

		public:
			void				update(u64 ticks)
			{
				mDateTimeTicks += ticks;
			}

			void				reset()
			{
				mDateTimeTicks = 0;
			}

			virtual u64			getSystemTime()
			{
				return mDateTimeTicks;
			}

			virtual u64			getSystemTimeAsFileTime()
			{
				return mDateTimeTicks;
			}

			virtual u64			getSystemTimeFromFileTime(u64 inFileSystemTime)
			{
				return inFileSystemTime;
			}

			virtual u64			getFileTimeFromSystemTime(u64 inSystemTime)
			{
				return inSystemTime;
			}
		};
		static xdatetime_source_test sDateTimeSource;

		static const s64 TicksPerDay			= X_CONSTANT_64(0xc92a69c000);
		static const s64 TicksPerHour			= X_CONSTANT_64(0x861c46800);
		static const s64 TicksPerMillisecond	= 10000;
		static const s64 TicksPerMinute			= 600000000;
		static const s64 TicksPerSecond			= 10000000;

		UNITTEST_FIXTURE_SETUP() 
		{
			x_SetDateTimeSource(&sDateTimeSource);
		}
		UNITTEST_FIXTURE_TEARDOWN() 
		{
			x_SetDateTimeSource(NULL);
		}

		UNITTEST_TEST(Now)
		{
			sDateTimeSource.reset();

			xdatetime dt1 = xcore::xdatetime::sNow();
			xdatetime dt2 = xcore::xdatetime::sNow();

			CHECK_EQUAL(1, dt1.year());
			CHECK_EQUAL(1, dt1.month());
			CHECK_EQUAL(1, dt1.day());
			CHECK_EQUAL(0, dt1.hour());
			CHECK_EQUAL(Monday, dt1.dayOfWeek());

			CHECK_TRUE(dt1.year() == dt2.year());
			CHECK_TRUE(dt1.month() == dt2.month());
			CHECK_TRUE(dt1.day() == dt2.day());
			CHECK_TRUE(dt1.dayOfWeek() == dt2.dayOfWeek());

			sDateTimeSource.update(TicksPerHour);

			dt1 = xcore::xdatetime::sNow();

			CHECK_EQUAL(1, dt1.year());
			CHECK_EQUAL(1, dt1.month());
			CHECK_EQUAL(1, dt1.day());
			CHECK_EQUAL(1, dt1.hour());
			CHECK_EQUAL(Monday, dt1.dayOfWeek());
		}

	}
}
UNITTEST_SUITE_END
