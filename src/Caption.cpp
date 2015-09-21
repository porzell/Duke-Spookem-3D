#include "Caption.h"

#include "Font.h"

Caption::Caption(Font &font, std::string text, Color forecolor)
{
	mColor = forecolor;

	mpFont = &font;

	mText = text;

	mDimensions = mpFont->getStringDimensionsVec2d(text);

	//redraw();

	mEnabled = false;

	checkListeners();
}

Caption::~Caption()
{
}

void Caption::draw()
{
	//mpBuffer->draw(buffer,mPosition);
	
	//glPushMatrix();

	//glTranslatef(mPosition.X(), mPosition.Y(), 0);

	//Vec2d center = (getDimensions() - mTextDimensions)/2;

	mpFont->print(mText, mPosition.X(), mPosition.Y());
	
	//mpFont->print(mText.c_str());

	//glPopMatrix();
}

/*void Caption::redraw()
{
	if(mpBuffer)
		delete mpBuffer;
	
	mpBuffer = New GraphicsBuffer(*mpFont, mText);
	
	mpBuffer->writeText(mText,Vec2d(0,0),*mpFont,mColor);
}*/