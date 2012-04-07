#include "xbase\x_types.h"
#include "xunittest\xunittest.h"

#include "xtime\x_frame_rate.h"
#include "xtime\private\x_time_source.h"

using namespace xcore;

UNITTEST_SUITE_BEGIN(framerate)
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
			x_SetTimeSource(NULL);
		}

		UNITTEST_TEST(constructor)
		{
			xframerate fps;
		}

		UNITTEST_TEST(restart)
		{
			xframerate fps;

			f32 f;
			CHECK_FALSE(fps.getFrameRate(f));
			CHECK_EQUAL(0.0f, f);
		}

		UNITTEST_TEST(markframe)
		{
			xframerate fps;

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
