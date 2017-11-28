
#include "xtime/x_timespan.h"
#include "xunittest/xunittest.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(timespan)
{
	UNITTEST_FIXTURE(main)
	{
		UNITTEST_FIXTURE_SETUP() {}
		UNITTEST_FIXTURE_TEARDOWN() {}

		UNITTEST_TEST(Now)
		{
			xtimespan ts1(15,20,30,40,500);
			xtimespan ts2(15,20,30,40,500);
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
			xtimespan ts1(10,0,20,0);

			xtimespan ts2(40,0,50,0);
			
			xtimespan ts3(50,1,10,0);
			
			ts1.add(ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(substract)
		{
			xtimespan ts1(50,1,10,0);

			xtimespan ts2(10,0,20,0);

			xtimespan ts3(40,0,50,0);

			ts1.substract(ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(duration)
		{
            xtimespan ts1(2000);

			xtimespan ts2 = ts1.duration();

			xtimespan ts3(-2000);

			xtimespan ts4 = ts3.duration();

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts4 == ts1);
		}
		UNITTEST_TEST(negate)
		{
			xtimespan ts1(1000);

			xtimespan ts2(-1000);

			ts1.negate();

			CHECK_TRUE(ts1 == ts2);

			xtimespan ts3(2000);

			xtimespan ts4(-2000);

			ts4.negate();

			CHECK_TRUE(ts3 == ts4);
		}
        UNITTEST_TEST(equal)
		{
			xtimespan ts1(3000);

			xtimespan ts2(3000);

			xtimespan ts3(4000);

			CHECK_TRUE(ts1.equals(ts2));

			CHECK_FALSE(ts1.equals(ts3));
		}
		UNITTEST_TEST(compareTo)
		{
			xtimespan ts1(2000);

			xtimespan ts2(3000);

			xtimespan ts3(3000);

			CHECK_TRUE(ts1.compareTo(ts2) == -1);

			CHECK_TRUE(ts2.compareTo(ts1) == 1);

			CHECK_TRUE(ts2.compareTo(ts3) == 0);
		}
		UNITTEST_TEST(sFromDays)
		{
			xtimespan ts1 = xtimespan::sFromDays(20);

			xtimespan ts2 = xtimespan::sFromDays(20);

			xtimespan ts3(20,0,0,0);

			xtimespan ts4 = xtimespan::sFromDays(100);

			xtimespan ts5(100,0,0,0);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);

			CHECK_TRUE(ts4 == ts5);
		}
        UNITTEST_TEST(sFromHours)
		{
			xtimespan ts1 = xtimespan::sFromHours(20);

			xtimespan ts2 = xtimespan::sFromHours(20);
			
			xtimespan ts3(20,0,0);

			xtimespan ts4 = xtimespan::sFromHours(2400);

			xtimespan ts5(100,0,0,0);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);
		
		    CHECK_TRUE(ts4 == ts5);
		}
		UNITTEST_TEST(sFromMinutes)
		{
            xtimespan ts1 = xtimespan::sFromMinutes(100);

			xtimespan ts2 = xtimespan::sFromMinutes(100);

			xtimespan ts3(1,40,0);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(sFromSeconds)
		{
			xtimespan ts1 = xtimespan::sFromSeconds(25);

			xtimespan ts2 = xtimespan::sFromSeconds(25);

			xtimespan ts3(0,0,25);

			CHECK_TRUE(ts1 == ts2);
		
			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(sFromMilliseconds)
		{
			xtimespan ts1 = xtimespan::sFromMilliseconds(450);

			xtimespan ts2 = xtimespan::sFromMilliseconds(450);

			xtimespan ts3(0,0,0,0,450);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(sFromTicks)
		{
			xtimespan ts1 = xtimespan::sFromTicks(2000);

			xtimespan ts2 = xtimespan::sFromTicks(2000);

			xtimespan ts3(2000);

			CHECK_TRUE(ts1 == ts2);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(sTimeToTicks)
		{
			u64 ticks1 = xtimespan::sTimeToTicks(2,20,20);

			u64 ticks2 = xtimespan::sTimeToTicks(2,20,20);

			u64 ticks3 = xtimespan::sTimeToTicks(2,20,20,20);

			u64 ticks4 = xtimespan::sTimeToTicks(2,20,20,20);

			u64 ticks5 = xtimespan::sTimeToTicks(2,20,20,20,20);

			u64 ticks6 = xtimespan::sTimeToTicks(2,20,20,20,20);

			xtimespan ts1(2,20,20);

			xtimespan ts2(0,2,20,20,20);

			xtimespan ts3(2,20,20,20,20);

			CHECK_TRUE(ticks1 == ticks2);

			CHECK_TRUE(ticks3 == ticks4);

			CHECK_TRUE(ticks5 == ticks6);
			
			CHECK_TRUE(ticks1 == ts1.ticks());

			CHECK_TRUE(ticks3 == ts2.ticks());

			CHECK_TRUE(ticks5 == ts3.ticks());
		}
		UNITTEST_TEST(sCompare)
		{
			xtimespan ts1(2000);

			xtimespan ts2(2000);

			xtimespan ts3(3000);

			s32 isCompare1 = xtimespan::sCompare(ts1,ts2);

			s32 isCompare2 = xtimespan::sCompare(ts2,ts3);

			s32 isCompare3 = xtimespan::sCompare(ts3,ts2);
			
			CHECK_TRUE(isCompare1 == 0);
			
			CHECK_TRUE(isCompare2 == -1);

			CHECK_TRUE(isCompare3 == 1);
		}
		UNITTEST_TEST(operator_subtration_equal)
		{
			xtimespan ts1(3000);

			xtimespan ts2(2000);

			ts1 -= ts2;

			xtimespan ts3(1000);

			CHECK_TRUE(ts1 == ts3);
		}
		UNITTEST_TEST(operator_add_equal)
		{
			xtimespan ts1(2000);

			xtimespan ts2(1000);

			ts1 += ts2;

			xtimespan ts3(3000);

			CHECK_TRUE(ts1 == ts3);
		}
		//==============================================================================
		// GLOBAL OPERATORS UNITTEST
		//==============================================================================
		UNITTEST_TEST(global_operator_xtimespan_subtract_xtimespan)
		{
			xtimespan ts1(3000);

			xtimespan ts2(1000);

			xtimespan ts3 = ts1 - ts2;

			xtimespan ts4(2000);

			CHECK_TRUE(ts3 == ts4);
		}
		UNITTEST_TEST(global_operator_xtimespan_add_xtimespan)
		{
			xtimespan ts1(2000);

			xtimespan ts2(1000);

			xtimespan ts3 = ts1 + ts2;

			xtimespan ts4(3000);

			CHECK_TRUE(ts3 == ts4);
		}
		UNITTEST_TEST(global_operator_xtimespan_small_xtimespan)
		{
			xtimespan ts1(1000);

			xtimespan ts2(2000);

			xtimespan ts3(2000);

			xbool isSmall1 = (ts1 < ts2);

			xbool isSmall2 = (ts2 < ts3);
			
			xbool isSmall3 = (ts2 < ts1);
			
			CHECK_TRUE(isSmall1);

			CHECK_FALSE(isSmall2);

			CHECK_FALSE(isSmall3);
		}
		UNITTEST_TEST(global_operator_xtimespan_large_xtimespan)
		{
			xtimespan ts1(2000);

			xtimespan ts2(1000);

			xtimespan ts3(1000);

			xbool isLarge1 = (ts1 > ts2);

			xbool isLarge2 = (ts2 > ts1);

			xbool isLarge3 = (ts2 > ts3);

			CHECK_TRUE(isLarge1);

			CHECK_FALSE(isLarge2);

			CHECK_FALSE(isLarge3);
		}
		UNITTEST_TEST(global_operator_xtimespan_noLarge_xtimespan)
		{
			xtimespan ts1(1000);

			xtimespan ts2(2000);

			xtimespan ts3(2000);

			xbool isSmall1 = (ts1 <= ts2);
			
			xbool isSmall2 = (ts2 <= ts3);

			xbool isSmall3 = (ts2 <= ts1);

			CHECK_TRUE(isSmall1);

			CHECK_TRUE(isSmall2);

			CHECK_FALSE(isSmall3);
		}
		UNITTEST_TEST(global_operator_xtimespan_noSmall_xtimespan)
		{
            xtimespan ts1(1000);

			xtimespan ts2(2000);

			xtimespan ts3(2000);

			xbool isLarge1 = (ts2 >= ts1);

			xbool isLarge2 = (ts3 >= ts2);

			xbool isLarge3 = (ts1 >= ts2);

			CHECK_TRUE(isLarge1);

			CHECK_TRUE(isLarge2);

			CHECK_FALSE(isLarge3);
	    }
		UNITTEST_TEST(global_operator_xtimespan_equal_xtimespan)
		{
			xtimespan ts1(2000);

			xtimespan ts2(2000);

			xtimespan ts3(3000);

			CHECK_TRUE(ts1 == ts2);

			CHECK_FALSE(ts2 == ts3);
		}
		UNITTEST_TEST(global_operator_xtimespan_noEqual_xtimespan)
		{
			xtimespan ts1(1000);

			xtimespan ts2(2000);

			xtimespan ts3(2000);

			CHECK_TRUE(ts2 != ts1);

			CHECK_FALSE(ts2 != ts3);
		}
	}
}
UNITTEST_SUITE_END
