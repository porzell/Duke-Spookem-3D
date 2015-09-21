#ifndef ISAVEWRITABLE
#define ISAVEWRITABLE
#include <cstdio>

class ISaveWritable
{
public:
	virtual void writeSave(FILE* outfile, unsigned pos) = 0;
	virtual void readSave(FILE* infile, unsigned pos) = 0;
};
#endif