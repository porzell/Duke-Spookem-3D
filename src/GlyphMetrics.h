#pragma once
#ifndef _GLYPH_METRICS_H
#define _GLYPH_METRICS_H

#include "Vec2d.h"

#include "Trackable.h"

struct GlyphMetrics : public Trackable
{
	//int x, y;
	//int width, height;

	Vec2d mSize;
	Vec2d mPos;
};

#endif