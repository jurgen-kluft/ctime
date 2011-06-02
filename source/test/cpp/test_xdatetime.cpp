#include "xbase\x_types.h"
#include "xtime\x_time.h"
#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(datetime)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Now)
		{
			xdatetime dt1 = xcore::xdatetime::sNow();
			xdatetime dt2 = xcore::xdatetime::sNow();

			CHECK_TRUE(dt1.year() == dt2.year());
			CHECK_TRUE(dt1.month() == dt2.month());
			CHECK_TRUE(dt1.day() == dt2.day());
			CHECK_TRUE(dt1.dayOfWeek() == dt2.dayOfWeek());
		}

	}
}
UNITTEST_SUITE_END
