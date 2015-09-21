#ifndef _COLOR_H
#define _COLOR_H

#include "GLincludes.h"
#include "Trackable.h"

struct Color : public Trackable
{
	GLfloat r, g, b, a;

	Color() { r = g = b = 0.0f; a = 1.0f; };
	
	Color(GLfloat red, GLfloat green, GLfloat blue)
	{
		r = red;
		g = green;
		b = blue;
		a = 1.0f;
	}

	Color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	void setActiveColor()
	{
		//glColor4fv(&r);	//This should be allocated such that it will load all 4 rgba values.
		glColor4f(r,g,b,a);
	}
};

#endif
