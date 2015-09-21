#ifndef _FONT_H
#define _FONT_H

#include "Trackable.h"

#ifdef WINDOWS
#include <windows.h>                      // Header File For Windows
#include <math.h>                     // Header File For Windows Math Library     ( ADD )
#endif
#include <stdio.h>                        // Header File For Standard Input/Output    ( ADD )
#include <stdarg.h>                       // Header File For Variable Argument Routines   ( ADD )
#include "GLincludes.h"

#include "Vec2d.h"

#include "FontMetrics.h"

#include "Header.h"

class Texture;

class Font : public Trackable
{
	protected:
		Texture *mpFontTexture;
		//GLuint mFont;
		//GLYPHMETRICSFLOAT mData[MAX_ALPHABET];

		FontMetrics *mpFontMetrics;

		bool mShouldDeleteMetrics;

	public:
		Font(Texture *fontTexture, FontMetrics *metrics);

		Font(Texture *fontTexture, std::string *xmlPath);

		//Get the length of a specified string written in this font.
		float getTextLength(std::string &msg);

		//Get font height.
		inline float getHeight() { return (*mpFontMetrics)['A'].mSize.Y(); };

		//Get the dimensions of a specified string written in this font.
		Vec2d getStringDimensionsVec2d(std::string &msg);

		//Print text in this font.
		GLvoid print(std::string &msg, float x, float y);

		~Font();
};

#endif
