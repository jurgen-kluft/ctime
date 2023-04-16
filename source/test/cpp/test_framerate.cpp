#include "cunittest/cunittest.h"

#include "ctime/c_frame_rate.h"
#include "ctime/private/c_time_source.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(framerate)
{
	UNITTEST_FIXTURE(main)
	{
		class xtime_source_test : public time_source_t
		{
			tick_t			mTicks;
		public:
			xtime_source_test()
				: mTicks(0)
			{

			}

			void			reset()
			{
				mTicks = 0;
			}

			void			update(tick_t ticks)
			{
				mTicks += ticks;
			}

			virtual tick_t	getTimeInTicks()
			{
				return mTicks;
			}

			virtual s64		getTicksPerSecond()
			{
				return 1000 * 1000;
			}

			virtual s64		getTicksPerMilliSecond()
			{
				return 1000;
			}

			virtual s64		getTicksPerMicroSecond()
			{
				return 1;
			}
		};

		static xtime_source_test sTimeSource;

		UNITTEST_FIXTURE_SETUP()
		{
			x_SetTimeSource(&sTimeSource);
		}

		UNITTEST_FIXTURE_TEARDOWN()
		{
			x_SetTimeSource(nullptr);
		}

		UNITTEST_TEST(constructor)
		{
			framerate_t fps;
		}

		UNITTEST_TEST(restart)
		{
			framerate_t fps;

			f32 f;
			CHECK_FALSE(fps.getFrameRate(f));
			CHECK_EQUAL(0.0f, f);
		}

		UNITTEST_TEST(markframe)
		{
			framerate_t fps;

			f32 f;
			CHECK_FALSE(fps.getFrameRate(f));
			CHECK_EQUAL(0.0f, f);

			fps.markFrame();
			CHECK_FALSE(fps.getFrameRate(f));
			CHECK_EQUAL(0.0f, f);
			fps.restart();

			sTimeSource.update(sTimeSource.getTicksPerSecond());

			fps.markFrame();

			CHECK_TRUE(fps.getFrameRate(f));
			CHECK_EQUAL(1.0f, f);
		}

	}
}
UNITTEST_SUITE_END
