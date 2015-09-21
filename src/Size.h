#pragma once
#ifndef _SIZE_H
#define _SIZE_H

#include "GLincludes.h"

class Size : public Trackable
{
	GLuint mDimX, mDimY;

	public:
		Size() { mDimX = 0; mDimY = 0; };
		Size(int x, int y) { mDimX = x; mDimY = y; };
		~Size() {};

		const inline GLuint getArea() { return mDimX * mDimY; };
		const inline GLuint getWidth() { return mDimX; };
		const inline GLuint getHeight() { return mDimY; };

		const inline void set(GLuint width, GLuint height) { mDimX = width; mDimY = height; };

		const inline void setWidth(GLuint width) { mDimX = width; };
		const inline void setHeight(GLuint height) { mDimY = height; };
};

#endif