#include "Menu.h"

#include "Event.h"

#include "Game.h"

#include "GLincludes.h"

#include "MenuItem.h"

#include "Texture.h"

extern Game *game;

Menu::Menu(Size &size, Texture *texture, Color forecolor, Color backcolor)
{
	mDimensions = Vec2d(size.getHeight(), size.getWidth());

	mForeColor = forecolor;
	mBackColor = backcolor;

	mPosition = Vec2d(0,0);

	opening = 0.1f;
	mIsOpen = false;

	mpTexture = texture;
}

Menu::Menu(Vec2d &size, Texture *texture, Color forecolor, Color backcolor)
{
	mDimensions = size;

	mForeColor = forecolor;
	mBackColor = backcolor;

	mPosition = Vec2d(0,0);

	opening = 0.1f;
	mIsOpen = false;

	mpTexture = texture;
}

Menu::Menu(float width, float height, Texture *texture, Color forecolor, Color backcolor)
{
	mDimensions = Vec2d(width, height);

	mForeColor = forecolor;
	mBackColor = backcolor;

	mPosition = Vec2d(0,0);

	opening = 0.1f;
	mIsOpen = false;

	mpTexture = texture;
}

Menu::~Menu()
{
	for(auto it = mItems.begin(); it != mItems.end(); ++it)
	{
		delete it->second;
	}
}

void Menu::addItem(std::string name, MenuItem *item)
{
	mItems[name] = item;
}

void Menu::draw()
{
	/*//Clear backbuffer.
	mpBuffer->fill(mBackColor);

	//Draw outline.
	mpBuffer->drawRect(Vec2d(0,0), mpBuffer->getDimensions(), mForeColor, 5.0);

	//Draw all of the menu items.
	for(auto it = mItems.begin(); it != mItems.end(); ++it)
	{
		it->second->draw(*mpBuffer);
	}

	mpBuffer->draw(buffer,mPosition);*/
	
	if(!mIsOpen)
	{
		opening += opening * 0.25;

		if(opening >= mDimensions.Y())
		{
			opening = mDimensions.Y();
			mIsOpen = true;
		}
	}

	glPushMatrix();

	glTranslatef(mPosition.X(), mPosition.Y(), 0);

	//***********
	//Draw menu.
	//***********
	
	if(mpTexture)
	{
		glEnable( GL_TEXTURE_2D );

		mpTexture->setAsActiveTexture();
	}

	glBegin(GL_QUADS);

		if(mpTexture)
			glColor4f(1.0f,1.0f,1.0f,0.5f);
		else
			mBackColor.setActiveColor();

		glTexCoord2f(0,0);
		glVertex2f( 0, 0 );	// Bottom Left Of The Texture and Quad

		glTexCoord2f(1,0);
		//glColor4ub(crBC.r, crBC.g, crBC.b, crBC.a);
		glVertex2f( mDimensions.X(), 0);	// Bottom Right Of The Texture and Quad

		glTexCoord2f(1,1);
		//glColor4ub(crTC.r, crTC.g, crTC.b, crTC.a);
		glVertex2f( mDimensions.X(), opening);	// Top Right Of The Texture and Quad

		glTexCoord2f(0,1);
		//glColor4ub(clTC.r, clTC.g, clTC.b, clTC.a);
		glVertex2f( 0.0, opening);	// Top Left Of The Texture and Quad

		glEnd();

	if(mpTexture)
	{
		glDisable( GL_TEXTURE_2D );
	}

	//***********
	//Draw outline.
	//***********
	
	glBegin(GL_LINES);
	
	//Set line width to 2.0.
	glLineWidth(5.0f);

	mForeColor.setActiveColor();

	glVertex2f(0,0);
	glVertex2f(mDimensions.X(), 0);

	glVertex2f(mDimensions.X(), 0);
	glVertex2f(mDimensions.X(), opening);

	glVertex2f(mDimensions.X(), opening);
	glVertex2f(0, opening);

	glVertex2f(0, opening);
	glVertex2f(0,0);

	glEnd();

	//Draw all of the menu items.
	if(mIsOpen)
		for(auto it = mItems.begin(); it != mItems.end(); ++it)
		{
			it->second->draw();
		}


	glPopMatrix();
}

void Menu::setEnabled(bool enabled)
{
	mEnabled = enabled;

	for(auto it = mItems.begin(); it != mItems.end(); ++it)
	{
		it->second->setEnabled(mEnabled);
	}
}

void Menu::handleEvent( const Event& theEvent )
{
}

void Menu::updateControlPositions()
{
	for(auto it = mItems.begin(); it != mItems.end(); ++it)
	{
		it->second->setMenuPosition(mPosition);
	}
}