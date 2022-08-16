#include "cbase/c_base.h"
#include "cbase/c_allocator.h"
#include "cbase/c_console.h"
#include "cbase/c_context.h"

#include "cunittest/xunittest.h"
#include "cunittest/private/ut_ReportAssert.h"

using namespace ncore;

UNITTEST_SUITE_LIST(xTimeUnitTest);
UNITTEST_SUITE_DECLARE(xTimeUnitTest, datetime);
UNITTEST_SUITE_DECLARE(xTimeUnitTest, timer);
UNITTEST_SUITE_DECLARE(xTimeUnitTest, framerate);
UNITTEST_SUITE_DECLARE(xTimeUnitTest, timespan);


namespace ncore
{
	// Our own assert handler
	class UnitTestAssertHandler : public ncore::asserthandler_t
	{
	public:
		UnitTestAssertHandler()
		{
			NumberOfAsserts = 0;
		}

		virtual bool handle_assert(u32 &flags, const char *fileName, s32 lineNumber, const char *exprString, const char *messageString)
		{
			UnitTest::reportAssert(exprString, fileName, lineNumber);
			NumberOfAsserts++;
			return false;
		}

		ncore::s32 NumberOfAsserts;
	};

	class UnitTestAllocator : public UnitTest::Allocator
	{
		ncore::alloc_t *mAllocator;

	public:
		UnitTestAllocator(ncore::alloc_t *allocator) { mAllocator = allocator; }
		virtual void *Allocate(size_t size) { return mAllocator->allocate((u32)size, sizeof(void *)); }
		virtual size_t Deallocate(void *ptr) { return mAllocator->deallocate(ptr); }
	};

	class TestAllocator : public alloc_t
	{
		alloc_t *mAllocator;

	public:
		TestAllocator(alloc_t *allocator) : mAllocator(allocator) {}

		virtual void *v_allocate(u32 size, u32 alignment)
		{
			UnitTest::IncNumAllocations();
			return mAllocator->allocate(size, alignment);
		}

		virtual u32 v_deallocate(void *mem)
		{
			UnitTest::DecNumAllocations();
			return mAllocator->deallocate(mem);
		}

		virtual void v_release()
		{
			mAllocator = nullptr;
		}
	};
} // namespace ncore

ncore::alloc_t *gTestAllocator = nullptr;
ncore::UnitTestAssertHandler gAssertHandler;

bool gRunUnitTest(UnitTest::TestReporter &reporter)
{
	xbase::init();

#ifdef TARGET_DEBUG
	ncore::context_t::set_assert_handler(&gAssertHandler);
#endif
	ncore::console->write("Configuration: ");
	ncore::console->setColor(ncore::console_t::YELLOW);
	ncore::console->writeLine(TARGET_FULL_DESCR_STR);
	ncore::console->setColor(ncore::console_t::NORMAL);

	ncore::alloc_t* systemAllocator = ncore::context_t::system_alloc();
	ncore::UnitTestAllocator unittestAllocator( systemAllocator );
	UnitTest::SetAllocator(&unittestAllocator);

	ncore::TestAllocator testAllocator(systemAllocator);
	gTestAllocator = &testAllocator;
	ncore::context_t::set_system_alloc(&testAllocator);

	int r = UNITTEST_SUITE_RUN(reporter, xTimeUnitTest);
	if (UnitTest::GetNumAllocations() != 0)
	{
		reporter.reportFailure(__FILE__, __LINE__, "xunittest", "memory leaks detected!");
		r = -1;
	}

	UnitTest::SetAllocator(nullptr);
	ncore::context_t::set_system_alloc(systemAllocator);

	xbase::exit();
	return r==0;
}
