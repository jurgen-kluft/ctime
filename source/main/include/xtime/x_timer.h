#ifndef __X_TIME_TIMER_H__
#define __X_TIME_TIMER_H__
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
	//==============================================================================
	// Types
	//==============================================================================
	typedef s64 xtick;

    /**
     * ------------------------------------------------------------------------------
	 *  Author:
	 *      Virtuos
	 *  Description:
	 *      The timer class is a handy way to measure how long has past since a particular event.
	 *      It is most often used for performance measurement but could also be used for other 
	 *      things as well. Usually the game time is based on one of these and then the delta
	 *      time is passed to the onAdvanceLogic to each of the objects.
	 *      The accuracy is in microseconds so very well suited to measure performance of parts
	 *      of code/execution.
	 * 
	 * <P>  The functions work as expected. The only tricky ones are the trip function
	 *      which are similar to a stop, reset, start sequence of calls. This trip 
	 *      function is especially handy to measure performance.
	 * 
	 *  Example:
	 * <CODE>
	 *       void main(void)
	 *       {
	 *           xticks Acc=0;
	 *           xtimer timer;
	 *           xtimer wholeTime;
	 * 
	 *            *  Start counting how long the whole thing is going to last
	 *           wholeTime.start();
	 *           
	 *           for(s32 i=0; i<100; i++)
	 *           {
	 *               Acc += timer.trip();    *  Note that the first trip will be zero
	 *               someFunction();
	 *           }
	 *           
	 *           x_printf("Whole Time %f average time %f", wholeTime.stopSec(), x_TicksToSec(Acc) / (i-1));
	 *       }
	 * </CODE>
	 * ------------------------------------------------------------------------------
	 */
	class xtimer
	{
	public:
					xtimer              ();

		void		start               ();
		void		reset               ();

		xtick		stop                ();
		xtick		read                () const;
		xtick		trip                ();

		xbool		isRunning           () const;
		s32			getNumTrips		    () const;

		f64			stopSec             ();
		f64			stopMs              ();
		f64			stopUs              ();

		f64			readSec             () const;
		f64			readMs              () const;
		f64			readUs              () const;

		f64			tripSec             ();
		f64			tripMs              ();
		f64			tripUs              ();

		f64			getAverageMs        () const;

	private:
		xtick		mStartTime;
		xtick		mTotalTime;
		xbool		mIsRunning;
		s32			mNumTrips;
	};
	

	//==============================================================================
	// INLINE
	//==============================================================================
	#include "private/x_timer_inline.h"


	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


//==============================================================================
// END __X_TIME_TIMER_H__
//==============================================================================
#endif