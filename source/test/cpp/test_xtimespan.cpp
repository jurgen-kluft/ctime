
#include "ctime/c_timespan.h"
#include "cunittest/xunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(timespan)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Now)
		{
			timespan_t ts1(15,20,30,40,500);
			timespan_t ts2(15,20,30,40,500);
			CHECK_TRUE(ts1.ticks() == ts2.ticks());
			CHECK_TRUE(ts1.days() == ts2.days());
			CHECK_TRUE(ts1.days() == 15);
			CHECK_TRUE(ts1.hours() == ts2.hours());
			CHECK_TRUE(ts1.hours() == 20);
			CHECK_TRUE(ts1.minutes() == ts2.minutes());
			CHECK_TRUE(ts1.minutes() == 30);
			CHECK_TRUE(ts1.seconds() == ts2.seconds());
			CHECK_TRUE(ts1.seconds() == 40);
			CHECK_TRUE(ts1.milliseconds() == ts2.milliseconds());
			CHECK_TRUE(ts1.milliseconds() == 500);
			CHECK_TRUE(ts1.totalDays() == ts2.totalDays());
			CHECK_TRUE(ts1.totalDays() == 15);
			CHECK_TRUE(ts1.totalHours() == ts2.totalHours());
			CHECK_TRUE(ts1.totalHours() == 380);
			CHECK_TRUE(ts1.totalMinutes() == ts2.totalMinutes());
			CHECK_TRUE(ts1.totalMinutes() == 22830);
			CHECK_TRUE(ts1.totalSeconds() == ts2.totalSeconds());
			CHECK_TRUE(ts1.totalSeconds() ==1369840);
			CHECK_TRUE(ts1.totalMilliseconds() == ts2.totalMilliseconds());
			CHECK_TRUE(ts1.totalMilliseconds() == 1369840500);
		}
		UNITTEST_TEST(add)
		{
			timespan_t ts1(10,0,20,0);

			timespan_t ts2(40,0,50,0);
			
			timespan_t ts3(50,1,10,0);
			
			ts1.add(ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(substract)
		{
			timespan_t ts1(50,1,10,0);

			timespan_t ts2(10,0,20,0);

			timespan_t ts3(40,0,50,0);

			ts1.substract(ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(duration)
		{
            timespan_t ts1(2000);

			timespan_t ts2 = ts1.duration();

			timespan_t ts3(-2000);

			timespan_t ts4 = ts3.duration();

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts4 == ts1);
		}
		UNITTEST_TEST(negate)
		{
			timespan_t ts1(1000);

			timespan_t ts2(-1000);

			ts1.negate();

			CHECK_TRUE(ts1 == ts2);

			timespan_t ts3(2000);

			timespan_t ts4(-2000);

			ts4.negate();

			CHECK_TRUE(ts3 == ts4);
		}
        UNITTEST_TEST(equal)
		{
			timespan_t ts1(3000);

			timespan_t ts2(3000);

			timespan_t ts3(4000);

			CHECK_TRUE(ts1.equals(ts2));

			CHECK_FALSE(ts1.equals(ts3));
		}
		UNITTEST_TEST(compareTo)
		{
			timespan_t ts1(2000);

			timespan_t ts2(3000);

			timespan_t ts3(3000);

			CHECK_TRUE(ts1.compareTo(ts2) == -1);

			CHECK_TRUE(ts2.compareTo(ts1) == 1);

			CHECK_TRUE(ts2.compareTo(ts3) == 0);
		}
		UNITTEST_TEST(sFromDays)
		{
			timespan_t ts1 = timespan_t::sFromDays(20);

			timespan_t ts2 = timespan_t::sFromDays(20);

			timespan_t ts3(20,0,0,0);

			timespan_t ts4 = timespan_t::sFromDays(100);

			timespan_t ts5(100,0,0,0);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);

			CHECK_TRUE(ts4 == ts5);
		}
        UNITTEST_TEST(sFromHours)
		{
			timespan_t ts1 = timespan_t::sFromHours(20);

			timespan_t ts2 = timespan_t::sFromHours(20);
			
			timespan_t ts3(20,0,0);

			timespan_t ts4 = timespan_t::sFromHours(2400);

			timespan_t ts5(100,0,0,0);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);
		
		    CHECK_TRUE(ts4 == ts5);
		}
		UNITTEST_TEST(sFromMinutes)
		{
            timespan_t ts1 = timespan_t::sFromMinutes(100);

			timespan_t ts2 = timespan_t::sFromMinutes(100);

			timespan_t ts3(1,40,0);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(sFromSeconds)
		{
			timespan_t ts1 = timespan_t::sFromSeconds(25);

			timespan_t ts2 = timespan_t::sFromSeconds(25);

			timespan_t ts3(0,0,25);

			CHECK_TRUE(ts1 == ts2);
		
			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(sFromMilliseconds)
		{
			timespan_t ts1 = timespan_t::sFromMilliseconds(450);

			timespan_t ts2 = timespan_t::sFromMilliseconds(450);

			timespan_t ts3(0,0,0,0,450);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(sFromTicks)
		{
			timespan_t ts1 = timespan_t::sFromTicks(2000);

			timespan_t ts2 = timespan_t::sFromTicks(2000);

			timespan_t ts3(2000);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(sTimeToTicks)
		{
			u64 ticks1 = timespan_t::sTimeToTicks(2,20,20);

			u64 ticks2 = timespan_t::sTimeToTicks(2,20,20);

			u64 ticks3 = timespan_t::sTimeToTicks(2,20,20,20);

			u64 ticks4 = timespan_t::sTimeToTicks(2,20,20,20);

			u64 ticks5 = timespan_t::sTimeToTicks(2,20,20,20,20);

			u64 ticks6 = timespan_t::sTimeToTicks(2,20,20,20,20);

			timespan_t ts1(2,20,20);

			timespan_t ts2(0,2,20,20,20);

			timespan_t ts3(2,20,20,20,20);

			CHECK_TRUE(ticks1 == ticks2);

			CHECK_TRUE(ticks3 == ticks4);

			CHECK_TRUE(ticks5 == ticks6);
			
			CHECK_TRUE(ticks1 == ts1.ticks());

			CHECK_TRUE(ticks3 == ts2.ticks());

			CHECK_TRUE(ticks5 == ts3.ticks());
		}
		UNITTEST_TEST(sCompare)
		{
			timespan_t ts1(2000);

			timespan_t ts2(2000);

			timespan_t ts3(3000);

			s32 isCompare1 = timespan_t::sCompare(ts1,ts2);

			s32 isCompare2 = timespan_t::sCompare(ts2,ts3);

			s32 isCompare3 = timespan_t::sCompare(ts3,ts2);
			
			CHECK_TRUE(isCompare1 == 0);
			
			CHECK_TRUE(isCompare2 == -1);

			CHECK_TRUE(isCompare3 == 1);
		}
		UNITTEST_TEST(operator_subtration_equal)
		{
			timespan_t ts1(3000);

			timespan_t ts2(2000);

			ts1 -= ts2;

			timespan_t ts3(1000);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(operator_add_equal)
		{
			timespan_t ts1(2000);

			timespan_t ts2(1000);

			ts1 += ts2;

			timespan_t ts3(3000);

			CHECK_TRUE(ts1 == ts3);
		}
		//==============================================================================
		// GLOBAL OPERATORS UNITTEST
		//==============================================================================
		UNITTEST_TEST(global_operator_xtimespan_subtract_xtimespan)
		{
			timespan_t ts1(3000);

			timespan_t ts2(1000);

			timespan_t ts3 = ts1 - ts2;

			timespan_t ts4(2000);

			CHECK_TRUE(ts3 == ts4);
		}
		UNITTEST_TEST(global_operator_xtimespan_add_xtimespan)
		{
			timespan_t ts1(2000);

			timespan_t ts2(1000);

			timespan_t ts3 = ts1 + ts2;

			timespan_t ts4(3000);

			CHECK_TRUE(ts3 == ts4);
		}
		UNITTEST_TEST(global_operator_xtimespan_small_xtimespan)
		{
			timespan_t ts1(1000);

			timespan_t ts2(2000);

			timespan_t ts3(2000);

			bool isSmall1 = (ts1 < ts2);

			bool isSmall2 = (ts2 < ts3);
			
			bool isSmall3 = (ts2 < ts1);
			
			CHECK_TRUE(isSmall1);

			CHECK_FALSE(isSmall2);

			CHECK_FALSE(isSmall3);
		}
		UNITTEST_TEST(global_operator_xtimespan_large_xtimespan)
		{
			timespan_t ts1(2000);

			timespan_t ts2(1000);

			timespan_t ts3(1000);

			bool isLarge1 = (ts1 > ts2);

			bool isLarge2 = (ts2 > ts1);

			bool isLarge3 = (ts2 > ts3);

			CHECK_TRUE(isLarge1);

			CHECK_FALSE(isLarge2);

			CHECK_FALSE(isLarge3);
		}
		UNITTEST_TEST(global_operator_xtimespan_noLarge_xtimespan)
		{
			timespan_t ts1(1000);

			timespan_t ts2(2000);

			timespan_t ts3(2000);

			bool isSmall1 = (ts1 <= ts2);
			
			bool isSmall2 = (ts2 <= ts3);

			bool isSmall3 = (ts2 <= ts1);

			CHECK_TRUE(isSmall1);

			CHECK_TRUE(isSmall2);

			CHECK_FALSE(isSmall3);
		}
		UNITTEST_TEST(global_operator_xtimespan_noSmall_xtimespan)
		{
            timespan_t ts1(1000);

			timespan_t ts2(2000);

			timespan_t ts3(2000);

			bool isLarge1 = (ts2 >= ts1);

			bool isLarge2 = (ts3 >= ts2);

			bool isLarge3 = (ts1 >= ts2);

			CHECK_TRUE(isLarge1);

			CHECK_TRUE(isLarge2);

			CHECK_FALSE(isLarge3);
	    }
		UNITTEST_TEST(global_operator_xtimespan_equal_xtimespan)
		{
			timespan_t ts1(2000);

			timespan_t ts2(2000);

			timespan_t ts3(3000);

			CHECK_TRUE(ts1 == ts2);

			CHECK_FALSE(ts2 == ts3);
		}
		UNITTEST_TEST(global_operator_xtimespan_noEqual_xtimespan)
		{
			timespan_t ts1(1000);

			timespan_t ts2(2000);

			timespan_t ts3(2000);

			CHECK_TRUE(ts2 != ts1);

			CHECK_FALSE(ts2 != ts3);
		}
	}
}
UNITTEST_SUITE_END
