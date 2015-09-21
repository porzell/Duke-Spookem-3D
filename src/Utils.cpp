#include "Utils.h"

//Random float generation function derived from 
//http://stackoverflow.com/questions/5289613/c-generate-random-float-between-two-floats.
float randomFloat(float min, float max)
{
	 if(min > max)
	{
		float tmp = max;
		max = min;
		min = tmp;
	}

	 float random = ((float) rand()) / (float) RAND_MAX;

	 float range = max - min;  
	 return (random*range) + min;
}

//Map a float from one scale to another.
//http://stackoverflow.com/questions/17134839/how-does-the-map-function-in-processing-work.
double convertScale(double value, double istart, double istop, double ostart, double ostop)
{
	 return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

//Map a float from one scale to another.
//http://stackoverflow.com/questions/17134839/how-does-the-map-function-in-processing-work.
float convertScale(float value, float istart, float istop, float ostart, float ostop)
{
	 return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

float constrain(float value, float istart, float istop)
{
	return std::max(std::min(value, istop), istart);
}