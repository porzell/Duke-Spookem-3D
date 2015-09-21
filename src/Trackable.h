#pragma once
#ifndef TRACKABLE_H
#define TRACKABLE_H

#include <iostream>

#define New new(__LINE__,__FILE__)

class Trackable
{
public:
	void* operator new( std::size_t size, int line, const char *file );
	void* operator new[]( std::size_t size, int line, const char *file );

	inline void operator delete  ( void *ptr, int line, const char *file ) { ::operator delete(ptr); }
	inline void operator delete[] ( void *ptr, int line, const char *file ) { ::operator delete[](ptr); }

	void* operator new( std::size_t size );
	void* operator new[]( std::size_t size );

	void operator delete( void *ptr );	
	void operator delete[]( void *ptr );
};

#endif