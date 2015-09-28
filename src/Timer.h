#ifndef GENERALLIB_TIMER
#define GENERALLIB_TIMER

#ifdef _WIN32
#include <windows.h>
#include "Trackable.h"

/* Timer - high accuracy timer - uses Large Integer to prevent rollover

Dean Lawson
Champlain College
2011
*/

class Timer:public Trackable
{
public:
	Timer();
	~Timer();

	void start();
	void stop();
	double getElapsedTime() const;//returns how much time has elapsed since start
	void sleepUntilElapsed( double ms );
	void pause( bool shouldPause );
	inline double getFactor() const { return mFactor; };
	inline void multFactor( double mult ) { mLastFactor = mFactor; mFactor *= mult; };
	inline void setFactor( double theFactor ) { mLastFactor = mFactor; mFactor = theFactor; };
	inline void restoreLastFactor() { mFactor = mLastFactor; };

private:
	LARGE_INTEGER mStartTime;
	LARGE_INTEGER mEndTime;
	LARGE_INTEGER mTimerFrequency;
	double mElapsedTime;
	double mFactor;
	double mLastFactor;
	bool mPaused;

	//helper function - uses current frequency for the Timer
	double calcDifferenceInMS( LARGE_INTEGER from, LARGE_INTEGER to ) const;

};
#elif __linux
#include <chrono>
#include "Trackable.h"

class Timer:public Trackable
{
public:
	Timer();
	~Timer();

	void start();
	void stop();
	double getElapsedTime() const;//returns how much time has elapsed since start
	void sleepUntilElapsed( double ms );
	void pause( bool shouldPause );
	inline double getFactor() const { return mFactor; };
	inline void multFactor( double mult ) { mLastFactor = mFactor; mFactor *= mult; };
	inline void setFactor( double theFactor ) { mLastFactor = mFactor; mFactor = theFactor; };
	inline void restoreLastFactor() { mFactor = mLastFactor; };

private:
	std::chrono::high_resolution_clock::time_point mStartTime;
	std::chrono::high_resolution_clock::time_point mEndTime;
	//std::chrono::high_resolution_clock::time_point mTimerFrequency;
	double mElapsedTime;
	double mFactor;
	double mLastFactor;
	bool mPaused;

	//helper function - uses current frequency for the Timer
	double calcDifferenceInMS( std::chrono::high_resolution_clock::time_point from, std::chrono::high_resolution_clock::time_point to ) const;

};
#endif
#endif
