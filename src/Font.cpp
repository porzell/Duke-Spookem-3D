#include "Font.h"

#include "Display.h"

#include "Game.h"

#include "Texture.h"

#include <cmath>

#include "Utils.h"

extern Game *game;

Font::Font(Texture *fontTexture, FontMetrics *metrics)
{
	mpFontTexture = fontTexture;
	mpFontMetrics = metrics;

	mShouldDeleteMetrics = false;
}

Font::Font(Texture *fontTexture, std::string *xmlPath)
{
	//mCharDimensions.set(charWidth, charHeight);
	mpFontTexture = fontTexture;
	mpFontMetrics = New FontMetrics(*xmlPath);

	mShouldDeleteMetrics = true;
}

Font::~Font()
{
	//glDeleteLists(mFont, MAX_ALPHABET);
	delete mpFontMetrics;
}

float Font::getTextLength(std::string &msg)
{
	float length = 0;

	for(size_t i = 0; i < msg.size(); i++)
	{
		char c = msg[i];

		length += (*mpFontMetrics)[c % MAX_ALPHABET].mSize.X();
	}
	
	return length;
}

Vec2d Font::getStringDimensionsVec2d(std::string &msg)
{
	Vec2d out;

	if(msg.length() > 0)
		out.addY(getHeight());

	for(size_t i = 0; i < msg.length(); ++i)
	{
		char c = msg[i];

		if(c == '\n' || c == '\r')
		{
			out.addY(getHeight());
			continue;
		}

		out.addX((*mpFontMetrics)[c % MAX_ALPHABET].mSize.X());
	}

	return out;
}

GLvoid Font::print(std::string &msg, float x, float y)					// Custom GL "Print" Routine
{
	glPushMatrix();

	glTranslatef(x, y, 0);

	Vec2d charPos;

	//mpFontTexture->setAsActiveTexture();

	glDisable(GL_LINES);
	glEnable(GL_TEXTURE_2D);

	char c = 'A';

	for(size_t i = 0; i < msg.length(); ++i)
	{
		//charPos.setX(fmod(mCharDimensions.X() * msg[i], mpFontTexture->getWidth()));
		//charPos.setY((((mCharDimensions.X() * msg[i]) - fmod(mCharDimensions.X() * msg[i], mpFontTexture->getWidth())) / mpFontTexture->getWidth()) * mCharDimensions.Y());
		
		/*charPos.setX(msg[i] % 16 * mCharDimensions.X());
		charPos.setY(256 - (msg[i] / 16 + 1) * mCharDimensions.Y());*/

		c = msg[i];

		//Support line endings.
		if(c == '\n' || c == '\r')
		{
			glTranslatef(0, getHeight(), 0);
			continue;
		}

		GlyphMetrics &glyph = (*mpFontMetrics)[c];

		charPos = glyph.mPos;

		//charPos.setY(-charPos.Y() + mpFontTexture->getHeight());
		charPos = charPos / mpFontTexture->getSizeVec2d();

		mpFontTexture->setAsActiveTexture();

		glBegin(GL_QUADS);

		glColor3f(1,1,1);

		
		glTexCoord2f(charPos.X(), 1.0f-(charPos.Y() + glyph.mSize.Y()/mpFontTexture->getHeight()));
		glVertex2f(0,0);

		glTexCoord2f(charPos.X() + glyph.mSize.X()/mpFontTexture->getWidth(), 1.0f-(charPos.Y() + glyph.mSize.Y()/mpFontTexture->getHeight()));
		
		glVertex2f(glyph.mSize.X(), 0);

		glTexCoord2f(charPos.X() + glyph.mSize.X()/mpFontTexture->getWidth(), 1.0f-charPos.Y());
		glVertex2f(glyph.mSize.X(), glyph.mSize.Y());

		glTexCoord2f(charPos.X(), 1.0f-charPos.Y());
		glVertex2f(0, glyph.mSize.Y());

		glEnd();

		glTranslatef(glyph.mSize.X(), 0, 0);
	}

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LINES);

	glPopMatrix();
	//mpFont->draw(x, y, fmt);
}