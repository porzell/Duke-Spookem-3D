#include <iostream>
#include <string>
#include "MemoryTracker.h"

using namespace std;

int MemoryTracker::msAllocationNum = 0;
MemoryTracker gMemoryTracker;

MemoryTracker::MemoryTracker()
{
}

MemoryTracker::~MemoryTracker()
{
	cout << "MemoryTracker being deleted: final allocations follow:\n";
	reportAllocations( cout );
}

void MemoryTracker::addAllocation( void* ptr, size_t size, std::string file, int line )
{
	//make sure it's not already in the map
	map<void*, AllocationRecord>::iterator iter = mAllocations.find( ptr );
	if( iter != mAllocations.end() )
	{
		//already exists - problem!
	}
	else
	{
		AllocationRecord theRec( msAllocationNum, size, file, line );
		pair<void*,AllocationRecord> thePair(ptr,theRec);
		mAllocations.insert( thePair );
		msAllocationNum++;
	}
}

void MemoryTracker::removeAllocation( void* ptr )
{
	//find it in the map!
	map<void*, AllocationRecord>::iterator iter = mAllocations.find( ptr );
	if( iter == mAllocations.end() )
	{
		//problem!!!!
	}
	else
	{
		mAllocations.erase( iter );
	}
}

void MemoryTracker::reportAllocations( std::ostream& stream )
{
	stream << "\n--------------------\nCurrent memory allocations:\n--------------------\n";

	map<void*, AllocationRecord>::iterator iter;
	for( iter = mAllocations.begin(); iter != mAllocations.end(); ++iter )
	{
		stream << "Address: 0x" << iter->first << endl
			   << "File: " << iter->second.file << endl
			   << "Line: " << iter->second.line << endl
			   << "Size:" << iter->second.size << " Num:" << iter->second.num
			   << "\n--------------------\n";
	}
}
