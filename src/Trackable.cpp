#include "Trackable.h"
#include "MemoryTracker.h"

void* Trackable::operator new( std::size_t size, int line, const char *file )
{
	void* ptr = malloc(size);

	std::string tmp = file;
	
	tmp = tmp.substr(tmp.find_last_of('\\')+1,tmp.size());

	gMemoryTracker.addAllocation( ptr, size, tmp, line );
	return ptr;
}

void* Trackable::operator new[]( std::size_t size, int line, const char *file )
{
	void* ptr = malloc(size);

	std::string tmp = file;

	tmp = tmp.substr(tmp.find_last_of('\\')+1,tmp.size());

	gMemoryTracker.addAllocation( ptr, size, tmp, line );
	return ptr;
}

void* Trackable::operator new( std::size_t size )
{
	void* ptr = malloc(size);
	gMemoryTracker.addAllocation( ptr, size, "N/A", -1 ); 
	return ptr;
}

void Trackable::operator delete( void *ptr )
{
	gMemoryTracker.removeAllocation(ptr); 
	free(ptr);
}

void* Trackable::operator new[]( std::size_t size )
{
	void* ptr = malloc(size);
	gMemoryTracker.addAllocation( ptr, size, "N/A", -1 );
	return ptr;
}

void Trackable::operator delete[]( void *ptr )
{
	gMemoryTracker.removeAllocation(ptr);
	free(ptr);
}