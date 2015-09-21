#ifndef __FONT_METRICS_H	//For some reason, _FONT_METRICS_H is already defined :S
#define __FONT_METRICS_H

#include "GlyphMetrics.h"

#include <string>

#include "Trackable.h"

#include "Header.h"

struct FontMetrics : public Trackable
{
	//Array of glyph metrics structs.
	GlyphMetrics mCharacterMetrics[MAX_ALPHABET];

	//Constructor.
	FontMetrics(std::string &xmlPath);

	bool loadFromXML(std::string &xmlPath);

	inline GlyphMetrics& operator[] (size_t index) { return mCharacterMetrics[index % MAX_ALPHABET]; }
};

#endif
