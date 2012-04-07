#include "xbase\x_types.h"
#include "xunittest\xunittest.h"
#include "xunittest\private\ut_Thread.h"

#include "xtime\x_datetime.h"
#include "xtime\private\x_datetime_source.h"

#include "xtime\x_timespan.h"

#include "xtime\x_time.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(datetime)
{
	UNITTEST_FIXTURE(main)
	{
		static const s64 TicksPerDay			= X_CONSTANT_64(0xc92a69c000);
		static const s64 TicksPerHour			= X_CONSTANT_64(0x861c46800);
		static const s64 TicksPerMillisecond	= 10000;
		static const s64 TicksPerMinute			= 600000000;
		static const s64 TicksPerSecond			= 10000000;

		class xdatetime_source_test : public xdatetime_source
		{
			u64					mDateTimeTicks;

		public:
			void				update(u64 ticks)
			{
				mDateTimeTicks += ticks;
			}

			void				set(u64 ticks)
			{
				mDateTimeTicks = ticks;
			}

			void				reset()
			{
				mDateTimeTicks = TicksPerDay;
			}

			virtual u64			getSystemTimeUtc()
			{
				return mDateTimeTicks - (TicksPerHour * 8);
			}

			virtual s64			getSystemTimeZone()
			{
				return (TicksPerHour * 8);
			}

			virtual u64			getSystemTimeLocal()
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



		UNITTEST_FIXTURE_SETUP() 
		{
			x_SetDateTimeSource(&sDateTimeSource);
		}
		UNITTEST_FIXTURE_TEARDOWN() 
		{
			x_SetDateTimeSource(NULL);
		}

		UNITTEST_TEST(RealNow)
		{
			x_TimeInit();

			xdatetime start = xdatetime::sNow();
			UnitTest::gSleep(200);
			xdatetime end = xdatetime::sNow();

			xtimespan span = end - start;
			u32 ms = span.totalMilliseconds();
			CHECK_TRUE(ms >= 150 && ms < 250);

			x_TimeExit();
			x_SetDateTimeSource(&sDateTimeSource);
		}

		UNITTEST_TEST(Now)
		{
			sDateTimeSource.reset();

			xdatetime dt1(2011,5,1,14,30,40,300);
			xdatetime dt2(2011,5,1,14,30,40,300);

			CHECK_TRUE(dt1.year() == dt2.year());
			CHECK_TRUE(dt1.year() == 2011);
			CHECK_TRUE(dt1.month() == dt2.month());
			CHECK_TRUE(dt1.month() == 5);
			CHECK_TRUE(dt1.day() == dt2.day());
			CHECK_TRUE(dt1.day() == 1);
			CHECK_TRUE(dt1.hour() == dt2.hour());
			CHECK_TRUE(dt1.hour() == 14);
			CHECK_TRUE(dt1.minute() == dt2.minute());
			CHECK_TRUE(dt1.minute() == 30);
			CHECK_TRUE(dt1.second() == dt2.second());
			CHECK_TRUE(dt1.second() == 40);
			CHECK_TRUE(dt1.millisecond() == dt2.millisecond());
			CHECK_TRUE(dt1.millisecond() == 300);
			CHECK_TRUE(dt1.ticks() == dt2.ticks());
			CHECK_TRUE(dt1.ticks() == 634398570403000000);
		}
		UNITTEST_TEST(date)
		{
			xdatetime dt1(2011,5,1,3,3,3);

			xdatetime dt2(2011,5,1,0,0,0);

			CHECK_TRUE(dt2 == dt1.date());
		}
		UNITTEST_TEST(timeOfDay)
		{
			xdatetime dt1(2011,5,1,3,3,3);

			xtimespan ts1(3,3,3);

			CHECK_TRUE(ts1 == dt1.timeOfDay());
		}
		UNITTEST_TEST(dayOfWeek)
		{
			xdatetime dt1(2011,5,2);

			CHECK_TRUE(dt1.dayOfWeek() == 1);

			xdatetime dt2(2011,4,27);

			CHECK_TRUE(dt2.dayOfWeek() == 3);
		}
		UNITTEST_TEST(dayOfWeekShort)
		{
			xdatetime dt1(2011,5,2);

			CHECK_TRUE(dt1.dayOfWeekShort() == 8);

			xdatetime dt2(2011,4,27);

			CHECK_TRUE(dt2.dayOfWeekShort() == 10);
		}
		UNITTEST_TEST(dayOfYear)
		{
			xdatetime dt1(2011,1,2);

			CHECK_TRUE(dt1.dayOfYear() == 2);

			xdatetime dt2(2011,3,5);
			
			CHECK_TRUE(dt2.dayOfYear() == 64);

			xdatetime dt3(2012,3,5);

			CHECK_TRUE(dt3.dayOfYear() == 65);
		}
		UNITTEST_TEST(monthShort)
		{
			xdatetime dt1(2011,1,1);

			xdatetime dt2(2011,7,3);

			CHECK_TRUE(dt1.monthShort() == 13);

			CHECK_TRUE(dt2.monthShort() == 19);
		}

		UNITTEST_TEST(add)
		{
			xdatetime dt1 = xcore::xdatetime::sNow();

			u64 tick = dt1.ticks();

			xtimespan ts(200);

			dt1.add(ts);

			CHECK_TRUE(dt1.ticks() == tick + ts.ticks());

			xdatetime dt2(2011,5,1);

			xtimespan ts2(10,0,0,0);

			dt2.add(ts2);

			xdatetime dt3(2011,5,11,0,0,0);

			CHECK_TRUE(dt2 == dt3);
		}
		UNITTEST_TEST(addYears)
		{
			xdatetime dt1 = xcore::xdatetime::sNow();

			s32 year = dt1.year();

			dt1.addYears(2);

			CHECK_TRUE(dt1.year() == (year + 2));
		}

		UNITTEST_TEST(addMonths)
		{
			xdatetime dt1 = xcore::xdatetime::sNow();

			s32 month = dt1.month();

			s32 year = dt1.year();

			if(month < 8)
			{
				dt1.addMonths(4);

				CHECK_TRUE(dt1.month() == (month + 4));
			}
			else
			{
				dt1.addMonths(5);

				CHECK_TRUE(dt1.month() == (month -7));	
			}	
		}
		UNITTEST_TEST(addDays)
		{
			s32 Month1[] = {1,3,5,7,8,10};
			s32 Month2[] = {4,6,9,11};

			for(s32 i = 0; i < 6; i++)
			{
				xdatetime dt1(2011,Month1[i],25);

				xdatetime dt2 = dt1.addDays(10);

				xdatetime dt3(2011,Month1[i] + 1,4);

				CHECK_TRUE(dt2 == dt3);
			}
            for(s32 j = 0; j < 4; j++)
			{
				xdatetime dt4(2011,Month2[j],25);

				xdatetime dt5 = dt4.addDays(10);

				xdatetime dt6(2011,Month2[j] + 1,5);

				CHECK_TRUE(dt5 == dt6);
			}
			xdatetime dt7(2011,12,25);

			xdatetime dt8 = dt7.addDays(10);

			xdatetime dt9(2012,1,4);

			CHECK_TRUE(dt8 == dt9);

			xdatetime dt10(2011,2,25);

			xdatetime dt11 = dt10.addDays(10);

			xdatetime dt12(2011,3,7);

			CHECK_TRUE(dt11 == dt12);

			xdatetime dt13(2012,2,25);

			xdatetime dt14 = dt13.addDays(10);

			xdatetime dt15(2012,3,6);

			CHECK_TRUE(dt14 == dt15);
		}
		UNITTEST_TEST(addHours)
		{
			xdatetime dt1(2011,5,1,12,10,20);

			xdatetime dt2 = dt1.addHours(8);

			xdatetime dt3(2011,5,1,20,10,20);

			CHECK_TRUE(dt2 == dt3);
			
			xdatetime dt4 = dt2.addHours(10);

			xdatetime dt5(2011,5,2,6,10,20);

			CHECK_TRUE(dt4 == dt5);
		}
		UNITTEST_TEST(addMilliseconds)
		{
			xdatetime dt1(2011,5,1,12,10,20,500);

			dt1.addMilliseconds(300);

		    xdatetime dt2(2011,5,1,12,10,20,800);

			CHECK_TRUE(dt2 == dt1);

			dt1.addMilliseconds(200);

			xdatetime dt5(2011,5,1,12,10,21,0);

			CHECK_TRUE(dt1 == dt5);
		}
		UNITTEST_TEST(addMinutes)
		{
			xdatetime dt1(2011,5,1,5,10,20);

			xdatetime dt2 = dt1.addMinutes(10);

		    xdatetime dt3(2011,5,1,5,20,20);
			
			CHECK_TRUE(dt2 == dt3);

			xdatetime dt4 = dt1.addMinutes(50);

			xdatetime dt5(2011,5,1,6,10,20);

			CHECK_TRUE(dt4 == dt5);
		}
		UNITTEST_TEST(addSeconds)
		{
			xdatetime dt1(2011,5,1,5,20,10);

		    dt1.addSeconds(30);

			xdatetime dt2(2011,5,1,5,20,40);

			CHECK_TRUE(dt1 == dt2);

			dt1.addSeconds(30);

			xdatetime dt3(2011,5,1,5,21,10);

			CHECK_TRUE(dt1 == dt3);
		}
		UNITTEST_TEST(addTicks)
		{
			xdatetime dt1 = xcore::xdatetime::sNow();

			u64 tick = dt1.ticks();

			dt1.addTicks(1);

			CHECK_TRUE(dt1.ticks() == tick + 1);
		}
		UNITTEST_TEST(subtract_xdatetime)
		{
			xdatetime dt1(300);
			u64 tick = dt1.ticks();

			xtimespan dt2(200);
			u64 tick1 = dt2.ticks();

			dt1.subtract(dt2);
			CHECK_TRUE(dt1.ticks() == tick - tick1);
		}
		UNITTEST_TEST(subtract_xtimespan)
		{
			xdatetime dt1(300);
			xdatetime dt2(200);

			u64 tick  = dt1.ticks();
			u64 tick1 = dt2.ticks();

		    xtimespan ts = dt1.subtract(dt2);

			CHECK_TRUE(ts.ticks() == tick - tick1);
		}
		UNITTEST_TEST(compareTo)
		{
			xdatetime dt1(200);
			xdatetime dt2(200);

			s32 isEqual1 = dt1.compareTo(dt2);

			CHECK_TRUE(isEqual1 == 0);

			xdatetime dt3(2010,5,1);
			xdatetime dt4(2011,5,1);

			s32 isEqual2 = dt3.compareTo(dt4);
			CHECK_TRUE(isEqual2 == -1);

			s32 isEqual3 = dt4.compareTo(dt3);
			CHECK_TRUE(isEqual3 == 1);
		}
		UNITTEST_TEST(equals)
		{
			xdatetime dt1(2011,7,1);
			xdatetime dt2(2011,7,1);

			xdatetime dt3(2011,1,1);

			s32 isEqual1 = dt1.equals(dt2);
			s32 isEqual2 = dt2.equals(dt3);

			CHECK_TRUE(isEqual1);
			CHECK_FALSE(isEqual2);
		}
		UNITTEST_TEST(sNow)
		{
			xdatetime dt1(2011,8,18);
			xdatetime dt2(2011,8,18);

			CHECK_TRUE(dt1 == dt2);
		}
		UNITTEST_TEST(sToday)
		{
			xdatetime dt1 = xdatetime::sToday();
			xdatetime dt2 = xdatetime::sToday();

			CHECK_TRUE(dt1 == dt2);
		}
		UNITTEST_TEST(sFromBinary)
		{
			xdatetime dt1 = xdatetime::sFromBinary(2000);
			xdatetime dt2 = xdatetime::sFromBinary(2000);

			CHECK_TRUE(dt1 == dt2);
			CHECK_TRUE(dt1.ticks() == 2000);
		}
		UNITTEST_TEST(sFromFileTime)
		{
			xdatetime dt1 = xdatetime::sFromFileTime(2000);
			xdatetime dt2 = xdatetime::sFromFileTime(2000);

			CHECK_TRUE(dt1 == dt2);
		}
		UNITTEST_TEST(sDaysInMonth)
		{
			s32 Month1[] = {1,3,5,7,8,10,12};

			for(s32 i = 0; i < 7; i++)
			{
				s32 sDay1 = xdatetime::sDaysInMonth(2011,Month1[i]);

				CHECK_TRUE(sDay1 == 31);
			}

			s32 Month2[] = {4,6,9,11};
			for(s32 j = 0; j < 4; j++)
			{
				s32 sDay2 = xdatetime::sDaysInMonth(2011,Month2[j]);
				
				CHECK_TRUE(sDay2 == 30);
			}
			s32 sDay3 = xdatetime::sDaysInMonth(2012,2);

			CHECK_TRUE(sDay3 == 29);

			s32 sDay4 = xdatetime::sDaysInMonth(2011,2);

			CHECK_TRUE(sDay4 == 28);
		}
		UNITTEST_TEST(sDaysInYear)
		{
			s32 sDays1 = xdatetime::sDaysInYear(2011);

			s32 sDays2 = xdatetime::sDaysInYear(2012);

			CHECK_TRUE(sDays1 == 365);

			CHECK_TRUE(sDays2 == 366);
		}
		UNITTEST_TEST(sIsLeapYear)
		{
			xbool isLeapYear1 = xdatetime::sIsLeapYear(2010);

			xbool isLeapYear2 = xdatetime::sIsLeapYear(2012);

			CHECK_FALSE(isLeapYear1);

			CHECK_TRUE(isLeapYear2);
		}
		UNITTEST_TEST(sCompare)
		{
			xdatetime dt1(1000);

			xdatetime dt2(1000);

			xdatetime dt3(2000);

			s32 compareResult1 = xdatetime::sCompare(dt1,dt2);

			CHECK_TRUE(compareResult1 == 0);

			s32 compareResult2 = xdatetime::sCompare(dt1,dt3);

			CHECK_TRUE(compareResult2 == -1);

			s32 compareResult3 = xdatetime::sCompare(dt3,dt1);

			CHECK_TRUE(compareResult3 == 1);
		}
		//==============================================================================
		// GLOBAL OPERATORS UNITTEST
		//==============================================================================
		UNITTEST_TEST(global_operator_xdatetime_subtract_xtimespan)
		{
			xdatetime dt(2011,5,1,5,30,40);

			xtimespan ts(4,30,40);

			xdatetime dt1 = dt - ts;

			xdatetime dt2(2011,5,1,1,0,0);

			CHECK_TRUE(dt1 == dt2);
		}
		UNITTEST_TEST(global_operator_xdatetime_add_xtimespan)
		{
			xdatetime dt(2011,5,1,5,30,40);

			xtimespan ts1(4,30,40);

			xdatetime dt1 = dt + ts1;

			xdatetime dt2(2011,5,1,10,1,20);

			CHECK_TRUE(dt1 == dt2);

			xtimespan ts2(20,0,0);

			xdatetime dt3 = dt + ts2;

			xdatetime dt4(2011,5,2,1,30,40);

			CHECK_TRUE(dt3 == dt4);
		}
        UNITTEST_TEST(global_operator_xdatetime_subtract_xdatetime)
		{
			xdatetime dt1 = xdatetime::sNow();

			xdatetime dt2 = xdatetime::sNow();

			xtimespan ts1 = dt1 - dt2;

			CHECK_TRUE(ts1.ticks() == 0);

			xdatetime dt4(2011,5,12,6,30,50);

			xdatetime dt5(2011,4,29,6,30,50);

			xtimespan ts2(13,0,0,0);	

			xtimespan ts3 = dt4 - dt5;

			CHECK_TRUE(ts3 == ts2);
		}
		UNITTEST_TEST(global_operator_xdatetime_small_xdatetime)
		{
			xdatetime dt1(1000);

			xdatetime dt2(2000);

			CHECK_TRUE(dt1 < dt2);

			CHECK_FALSE(dt2 < dt1);
		}
		UNITTEST_TEST(global_operator_xdatetime_large_xdatetime)
		{
			xdatetime dt1(1000);

			xdatetime dt2(2000);

			xdatetime dt3(2000);

			CHECK_TRUE(dt2 > dt1);

			CHECK_FALSE(dt1 > dt2);

			CHECK_FALSE(dt2 > dt3);
		}
		UNITTEST_TEST(global_operator_xdatetime_noLarge_xdatetime)
		{
			xdatetime dt1(1000);

			xdatetime dt2(2000);

			xdatetime dt3(2000);

			CHECK_TRUE(dt1 <= dt2);

			CHECK_TRUE(dt2 <= dt2);

			CHECK_FALSE(dt2 <= dt1);
		}
		UNITTEST_TEST(global_operator_xdatetime_noSmall_xdatetime)
		{
			xdatetime dt1(1000);

			xdatetime dt2(2000);

			xdatetime dt3(2000);

			CHECK_TRUE(dt2 >= dt1);

			CHECK_TRUE(dt3 >= dt2);

			CHECK_FALSE(dt1 >= dt2);
		}
		UNITTEST_TEST(global_operator_xdatetime_noEqual_xdatime)
		{
			xdatetime dt1 = xdatetime::sNow();
			xdatetime dt2 = xdatetime::sNow();
			xdatetime dt3(2000,1,1,3,3,3);

			CHECK_FALSE(dt1 != dt2);
			CHECK_TRUE(dt1 != dt3);
		}
		UNITTEST_TEST(global_operator_xdatetime_equal_xdatetime)
		{
			xdatetime dt1 = xdatetime::sNow();
			xdatetime dt2 = xdatetime::sNow();

			CHECK_TRUE(dt1 == dt2);
		}

		//==============================================================================
		// INLINE FUNCTION UNITTEST OF CLASS XTIMER
		//==============================================================================
		UNITTEST_TEST(inline_xtimer_gAsShortNotation_EDayOfWeek)
		{
			EDayOfWeek dayofweek1 = gAsShortNotation(EDayOfWeek(3));

			EDayOfWeek dayofweek2 = gAsShortNotation(EDayOfWeek(9));

			CHECK_TRUE(dayofweek1 == EDayOfWeek(10));

			CHECK_TRUE(dayofweek2 == EDayOfWeek(9));
		}
		UNITTEST_TEST(inline_xtimer_gAsShortNotation_EMonth)
		{
			EMonth month1 = gAsShortNotation(EMonth(6));

			EMonth month2 = gAsShortNotation(EMonth(15));

			CHECK_TRUE(month1 == EMonth(18));

			CHECK_TRUE(month2 == EMonth(15));
		}
	}
}
UNITTEST_SUITE_END
