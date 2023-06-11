#include "cunittest/cunittest.h"
#include "ctime/c_datetime.h"
#include "ctime/private/c_datetime_source.h"
#include "ctime/c_timespan.h"
#include "ctime/c_time.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(datetime)
{
	UNITTEST_FIXTURE(main)
	{
		static const s64 TicksPerDay			= D_CONSTANT_64(0xc92a69c000);
		static const s64 TicksPerHour			= D_CONSTANT_64(0x861c46800);
		static const s64 TicksPerMillisecond	= 10000;
		static const s64 TicksPerMinute			= 600000000;
		static const s64 TicksPerSecond			= 10000000;

		class xdatetime_source_test : public datetime_source_t
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
			g_SetDateTimeSource(&sDateTimeSource);
		}
		UNITTEST_FIXTURE_TEARDOWN() 
		{
			g_SetDateTimeSource(nullptr);
		}

		UNITTEST_TEST(RealNow)
		{
			ntime::init();

			datetime_t start = datetime_t::sNow();
			datetime_t end;
			while (true)
			{
				end = datetime_t::sNow();
				timespan_t span = end - start;
				u32 ms = (u32)span.totalMilliseconds();
				if (ms >= 150)
					break;
			}

			timespan_t span = end - start;
			u32 ms = (u32)span.totalMilliseconds();
			CHECK_TRUE(ms >= 150);

			ntime::exit();
			g_SetDateTimeSource(&sDateTimeSource);
		}

		UNITTEST_TEST(Now)
		{
			sDateTimeSource.reset();

			datetime_t dt1(2011,5,1,14,30,40,300);
			datetime_t dt2(2011,5,1,14,30,40,300);

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
			datetime_t dt1(2011,5,1,3,3,3);

			datetime_t dt2(2011,5,1,0,0,0);

			CHECK_TRUE(dt2 == dt1.date());
		}
		UNITTEST_TEST(timeOfDay)
		{
			datetime_t dt1(2011,5,1,3,3,3);

			timespan_t ts1(3,3,3);

			CHECK_TRUE(ts1 == dt1.timeOfDay());
		}
		UNITTEST_TEST(dayOfWeek)
		{
			datetime_t dt1(2011,5,2);

			CHECK_TRUE(dt1.dayOfWeek() == 1);

			datetime_t dt2(2011,4,27);

			CHECK_TRUE(dt2.dayOfWeek() == 3);
		}
		UNITTEST_TEST(dayOfWeekShort)
		{
			datetime_t dt1(2011,5,2);

			CHECK_TRUE(dt1.dayOfWeekShort() == 8);

			datetime_t dt2(2011,4,27);

			CHECK_TRUE(dt2.dayOfWeekShort() == 10);
		}
		UNITTEST_TEST(dayOfYear)
		{
			datetime_t dt1(2011,1,2);

			CHECK_TRUE(dt1.dayOfYear() == 2);

			datetime_t dt2(2011,3,5);
			
			CHECK_TRUE(dt2.dayOfYear() == 64);

			datetime_t dt3(2012,3,5);

			CHECK_TRUE(dt3.dayOfYear() == 65);
		}
		UNITTEST_TEST(monthShort)
		{
			datetime_t dt1(2011,1,1);

			datetime_t dt2(2011,7,3);

			CHECK_TRUE(dt1.monthShort() == 13);

			CHECK_TRUE(dt2.monthShort() == 19);
		}

		UNITTEST_TEST(add)
		{
			datetime_t dt1 = ncore::datetime_t::sNow();

			u64 tick = dt1.ticks();

			timespan_t ts(200);

			dt1.add(ts);

			CHECK_TRUE(dt1.ticks() == tick + ts.ticks());

			datetime_t dt2(2011,5,1);

			timespan_t ts2(10,0,0,0);

			dt2.add(ts2);

			datetime_t dt3(2011,5,11,0,0,0);

			CHECK_TRUE(dt2 == dt3);
		}
		UNITTEST_TEST(addYears)
		{
			datetime_t dt1 = ncore::datetime_t::sNow();

			s32 year = dt1.year();

			dt1.addYears(2);

			CHECK_TRUE(dt1.year() == (year + 2));
		}

		UNITTEST_TEST(addMonths)
		{
			datetime_t dt1 = ncore::datetime_t::sNow();

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
				datetime_t dt1(2011,Month1[i],25);

				datetime_t dt2 = dt1.addDays(10);

				datetime_t dt3(2011,Month1[i] + 1,4);

				CHECK_TRUE(dt2 == dt3);
			}
            for(s32 j = 0; j < 4; j++)
			{
				datetime_t dt4(2011,Month2[j],25);

				datetime_t dt5 = dt4.addDays(10);

				datetime_t dt6(2011,Month2[j] + 1,5);

				CHECK_TRUE(dt5 == dt6);
			}
			datetime_t dt7(2011,12,25);

			datetime_t dt8 = dt7.addDays(10);

			datetime_t dt9(2012,1,4);

			CHECK_TRUE(dt8 == dt9);

			datetime_t dt10(2011,2,25);

			datetime_t dt11 = dt10.addDays(10);

			datetime_t dt12(2011,3,7);

			CHECK_TRUE(dt11 == dt12);

			datetime_t dt13(2012,2,25);

			datetime_t dt14 = dt13.addDays(10);

			datetime_t dt15(2012,3,6);

			CHECK_TRUE(dt14 == dt15);
		}
		UNITTEST_TEST(addHours)
		{
			datetime_t dt1(2011,5,1,12,10,20);

			datetime_t dt2 = dt1.addHours(8);

			datetime_t dt3(2011,5,1,20,10,20);

			CHECK_TRUE(dt2 == dt3);
			
			datetime_t dt4 = dt2.addHours(10);

			datetime_t dt5(2011,5,2,6,10,20);

			CHECK_TRUE(dt4 == dt5);
		}
		UNITTEST_TEST(addMilliseconds)
		{
			datetime_t dt1(2011,5,1,12,10,20,500);

			dt1.addMilliseconds(300);

		    datetime_t dt2(2011,5,1,12,10,20,800);

			CHECK_TRUE(dt2 == dt1);

			dt1.addMilliseconds(200);

			datetime_t dt5(2011,5,1,12,10,21,0);

			CHECK_TRUE(dt1 == dt5);
		}
		UNITTEST_TEST(addMinutes)
		{
			datetime_t dt1(2011,5,1,5,10,20);

			datetime_t dt2 = dt1.addMinutes(10);

		    datetime_t dt3(2011,5,1,5,20,20);
			
			CHECK_TRUE(dt2 == dt3);

			datetime_t dt4 = dt1.addMinutes(50);

			datetime_t dt5(2011,5,1,6,10,20);

			CHECK_TRUE(dt4 == dt5);
		}
		UNITTEST_TEST(addSeconds)
		{
			datetime_t dt1(2011,5,1,5,20,10);

		    dt1.addSeconds(30);

			datetime_t dt2(2011,5,1,5,20,40);

			CHECK_TRUE(dt1 == dt2);

			dt1.addSeconds(30);

			datetime_t dt3(2011,5,1,5,21,10);

			CHECK_TRUE(dt1 == dt3);
		}
		UNITTEST_TEST(addTicks)
		{
			datetime_t dt1 = ncore::datetime_t::sNow();

			u64 tick = dt1.ticks();

			dt1.addTicks(1);

			CHECK_TRUE(dt1.ticks() == tick + 1);
		}
		UNITTEST_TEST(subtract_xdatetime)
		{
			datetime_t dt1(300);
			u64 tick = dt1.ticks();

			timespan_t dt2(200);
			u64 tick1 = dt2.ticks();

			dt1.subtract(dt2);
			CHECK_TRUE(dt1.ticks() == tick - tick1);
		}
		UNITTEST_TEST(subtract_xtimespan)
		{
			datetime_t dt1(300);
			datetime_t dt2(200);

			u64 tick  = dt1.ticks();
			u64 tick1 = dt2.ticks();

		    timespan_t ts = dt1.subtract(dt2);

			CHECK_TRUE(ts.ticks() == tick - tick1);
		}
		UNITTEST_TEST(compareTo)
		{
			datetime_t dt1(200);
			datetime_t dt2(200);

			s32 isEqual1 = dt1.compareTo(dt2);

			CHECK_TRUE(isEqual1 == 0);

			datetime_t dt3(2010,5,1);
			datetime_t dt4(2011,5,1);

			s32 isEqual2 = dt3.compareTo(dt4);
			CHECK_TRUE(isEqual2 == -1);

			s32 isEqual3 = dt4.compareTo(dt3);
			CHECK_TRUE(isEqual3 == 1);
		}
		UNITTEST_TEST(equals)
		{
			datetime_t dt1(2011,7,1);
			datetime_t dt2(2011,7,1);

			datetime_t dt3(2011,1,1);

			s32 isEqual1 = dt1.equals(dt2);
			s32 isEqual2 = dt2.equals(dt3);

			CHECK_TRUE(isEqual1);
			CHECK_FALSE(isEqual2);
		}
		UNITTEST_TEST(sNow)
		{
			datetime_t dt1(2011,8,18);
			datetime_t dt2(2011,8,18);

			CHECK_TRUE(dt1 == dt2);
		}
		UNITTEST_TEST(sToday)
		{
			datetime_t dt1 = datetime_t::sToday();
			datetime_t dt2 = datetime_t::sToday();

			CHECK_TRUE(dt1 == dt2);
		}
		UNITTEST_TEST(sFromBinary)
		{
			datetime_t dt1 = datetime_t::sFromBinary(2000);
			datetime_t dt2 = datetime_t::sFromBinary(2000);

			CHECK_TRUE(dt1 == dt2);
			CHECK_TRUE(dt1.ticks() == 2000);
		}
		UNITTEST_TEST(sFromFileTime)
		{
			datetime_t dt1 = datetime_t::sFromFileTime(2000);
			datetime_t dt2 = datetime_t::sFromFileTime(2000);

			CHECK_TRUE(dt1 == dt2);
		}
		UNITTEST_TEST(sDaysInMonth)
		{
			s32 Month1[] = {1,3,5,7,8,10,12};

			for(s32 i = 0; i < 7; i++)
			{
				s32 sDay1 = datetime_t::sDaysInMonth(2011,Month1[i]);

				CHECK_TRUE(sDay1 == 31);
			}

			s32 Month2[] = {4,6,9,11};
			for(s32 j = 0; j < 4; j++)
			{
				s32 sDay2 = datetime_t::sDaysInMonth(2011,Month2[j]);
				
				CHECK_TRUE(sDay2 == 30);
			}
			s32 sDay3 = datetime_t::sDaysInMonth(2012,2);

			CHECK_TRUE(sDay3 == 29);

			s32 sDay4 = datetime_t::sDaysInMonth(2011,2);

			CHECK_TRUE(sDay4 == 28);
		}
		UNITTEST_TEST(sDaysInYear)
		{
			s32 sDays1 = datetime_t::sDaysInYear(2011);

			s32 sDays2 = datetime_t::sDaysInYear(2012);

			CHECK_TRUE(sDays1 == 365);

			CHECK_TRUE(sDays2 == 366);
		}
		UNITTEST_TEST(sIsLeapYear)
		{
			bool isLeapYear1 = datetime_t::sIsLeapYear(2010);

			bool isLeapYear2 = datetime_t::sIsLeapYear(2012);

			CHECK_FALSE(isLeapYear1);

			CHECK_TRUE(isLeapYear2);
		}
		UNITTEST_TEST(sCompare)
		{
			datetime_t dt1(1000);

			datetime_t dt2(1000);

			datetime_t dt3(2000);

			s32 compareResult1 = datetime_t::sCompare(dt1,dt2);

			CHECK_TRUE(compareResult1 == 0);

			s32 compareResult2 = datetime_t::sCompare(dt1,dt3);

			CHECK_TRUE(compareResult2 == -1);

			s32 compareResult3 = datetime_t::sCompare(dt3,dt1);

			CHECK_TRUE(compareResult3 == 1);
		}
		//==============================================================================
		// GLOBAL OPERATORS UNITTEST
		//==============================================================================
		UNITTEST_TEST(global_operator_xdatetime_subtract_xtimespan)
		{
			datetime_t dt(2011,5,1,5,30,40);

			timespan_t ts(4,30,40);

			datetime_t dt1 = dt - ts;

			datetime_t dt2(2011,5,1,1,0,0);

			CHECK_TRUE(dt1 == dt2);
		}
		UNITTEST_TEST(global_operator_xdatetime_add_xtimespan)
		{
			datetime_t dt(2011,5,1,5,30,40);

			timespan_t ts1(4,30,40);

			datetime_t dt1 = dt + ts1;

			datetime_t dt2(2011,5,1,10,1,20);

			CHECK_TRUE(dt1 == dt2);

			timespan_t ts2(20,0,0);

			datetime_t dt3 = dt + ts2;

			datetime_t dt4(2011,5,2,1,30,40);

			CHECK_TRUE(dt3 == dt4);
		}
        UNITTEST_TEST(global_operator_xdatetime_subtract_xdatetime)
		{
			datetime_t dt1 = datetime_t::sNow();

			datetime_t dt2 = datetime_t::sNow();

			timespan_t ts1 = dt1 - dt2;

			CHECK_TRUE(ts1.ticks() == 0);

			datetime_t dt4(2011,5,12,6,30,50);

			datetime_t dt5(2011,4,29,6,30,50);

			timespan_t ts2(13,0,0,0);	

			timespan_t ts3 = dt4 - dt5;

			CHECK_TRUE(ts3 == ts2);
		}
		UNITTEST_TEST(global_operator_xdatetime_small_xdatetime)
		{
			datetime_t dt1(1000);

			datetime_t dt2(2000);

			CHECK_TRUE(dt1 < dt2);

			CHECK_FALSE(dt2 < dt1);
		}
		UNITTEST_TEST(global_operator_xdatetime_large_xdatetime)
		{
			datetime_t dt1(1000);

			datetime_t dt2(2000);

			datetime_t dt3(2000);

			CHECK_TRUE(dt2 > dt1);

			CHECK_FALSE(dt1 > dt2);

			CHECK_FALSE(dt2 > dt3);
		}
		UNITTEST_TEST(global_operator_xdatetime_noLarge_xdatetime)
		{
			datetime_t dt1(1000);

			datetime_t dt2(2000);

			datetime_t dt3(2000);

			CHECK_TRUE(dt1 <= dt2);

			CHECK_TRUE(dt2 <= dt2);

			CHECK_FALSE(dt2 <= dt1);
		}
		UNITTEST_TEST(global_operator_xdatetime_noSmall_xdatetime)
		{
			datetime_t dt1(1000);

			datetime_t dt2(2000);

			datetime_t dt3(2000);

			CHECK_TRUE(dt2 >= dt1);

			CHECK_TRUE(dt3 >= dt2);

			CHECK_FALSE(dt1 >= dt2);
		}
		UNITTEST_TEST(global_operator_xdatetime_noEqual_xdatime)
		{
			datetime_t dt1 = datetime_t::sNow();
			datetime_t dt2 = datetime_t::sNow();
			datetime_t dt3(2000,1,1,3,3,3);

			CHECK_FALSE(dt1 != dt2);
			CHECK_TRUE(dt1 != dt3);
		}
		UNITTEST_TEST(global_operator_xdatetime_equal_xdatetime)
		{
			datetime_t dt1 = datetime_t::sNow();
			datetime_t dt2 = datetime_t::sNow();

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
