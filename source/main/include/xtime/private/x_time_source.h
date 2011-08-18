#ifndef __X_TIME_SOURCE_H__
#define __X_TIME_SOURCE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
//  INCLUDES
//==============================================================================
#include "xbase\x_types.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class xtime_source
	{
	public:
		virtual			~xtime_source()		{}

		virtual s64		getTimeInTicks() = 0;
		virtual s64		getTicksPerMilliSecond() = 0;
		virtual s64		getTicksPerSecond() = 0;
	};
	
	extern void		x_SetTimeSource		(xtime_source*);

	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


//==============================================================================
// END __X_TIME_SOURCE_H__
//==============================================================================
#endif
