#pragma once

#include <map>
#include <iostream>


struct AllocationRecord
{
	AllocationRecord( int theNum, size_t theSize, std::string theFile, int theLine ): num(theNum), size(theSize), file(theFile), line(theLine){};
	int num;
	size_t size;

	std::string file;
	int line;
};

class MemoryTracker
{
public:
	MemoryTracker();
	~MemoryTracker();

	void addAllocation( void* ptr, size_t size, std::string file, int line );
	void removeAllocation( void* ptr );
	
	void reportAllocations( std::ostream& stream );

private:
	//copying not allowed
	MemoryTracker( const MemoryTracker& );
	MemoryTracker& operator=( const MemoryTracker& );

	std::map<void*,AllocationRecord> mAllocations;

	static int msAllocationNum;
};

extern MemoryTracker gMemoryTracker;