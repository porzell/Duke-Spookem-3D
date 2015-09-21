#include "Timer.h"

Timer::Timer()
:mElapsedTime(0.0)
,mPaused(true)
,mFactor(1.0)
,mLastFactor(1.0)
{
	QueryPerformanceFrequency( &mTimerFrequency );
	mStartTime.QuadPart = 0;
	mEndTime.QuadPart = 0;
}

Timer::~Timer()
{
}

void Timer::start()
{
	QueryPerformanceCounter( &mStartTime );
	
	//reset end time as well
	mEndTime.QuadPart = 0;

	mElapsedTime = 0.0;

	pause( false );//unpause
}

void Timer::stop()
{
	QueryPerformanceCounter( &mEndTime );
	mElapsedTime = calcDifferenceInMS( mStartTime, mEndTime );
} 

void Timer::pause( bool shouldPause )
{
	if( shouldPause && !mPaused )//want to pause and we are not currently paused
	{
		mPaused = true;
		QueryPerformanceCounter( &mEndTime );
		mElapsedTime += calcDifferenceInMS( mStartTime, mEndTime );
	}
	else if( !shouldPause && mPaused )//want to unpause and we are paused
	{
		mPaused = false;
		QueryPerformanceCounter( &mStartTime );
	}
}

double Timer::getElapsedTime() const
{
	//if we have an end time then the timer isn't running and we can just return the elapsed time
	if( mEndTime.QuadPart != 0 )
	{
		return mElapsedTime;
	}
	else //otherwise we need to get the current time, do the math and return that
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter( &currentTime );
		return calcDifferenceInMS( mStartTime, currentTime );
	}
}

void Timer::sleepUntilElapsed( double ms )
{
	LARGE_INTEGER currentTime, lastTime;
	QueryPerformanceCounter( &currentTime );
	double timeToSleep = ms - calcDifferenceInMS( mStartTime, currentTime );

	while( timeToSleep > 0.0 )
	{
		lastTime = currentTime;
		QueryPerformanceCounter( &currentTime );
		double timeElapsed = calcDifferenceInMS( lastTime, currentTime );
		timeToSleep -= timeElapsed;
		if( timeToSleep > 10.0 )//if we are going to be in this loop for a long time - 
		{						//Sleep to relinquish back to Windows
			Sleep(10);
		}
	}
}

double Timer::calcDifferenceInMS( LARGE_INTEGER from, LARGE_INTEGER to ) const
{
	double difference = (double)(to.QuadPart - from.QuadPart) / (double)mTimerFrequency.QuadPart;
	difference *= mFactor;
	return difference * 1000;
}
