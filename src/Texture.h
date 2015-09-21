#pragma once
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "GLincludes.h"

#include "Trackable.h"

#include "Size.h"

#include "Vec2d.h"

class Texture : public Trackable
{
	private:

		GLuint mTextureID;

		//Texture dimensions - DO NOT CHECK THESE TO SEE IF A TEXTURE IS LOADED!  CHECK if(!getTextureID())!
		Size mSize;

		bool mShouldWrap;

		inline void setFilters()
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   // Linear Min Filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // Linear Mag Filter
		}

		inline void facilitateWrap()
		{
			if(mTextureID)
			{
				/*if(mShouldWrap)
				{
					glBindTexture(GL_TEXTURE_2D, mTextureID);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}*/
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
		}

    public:
		//Default Constructor.
		Texture();

		//Constructor for loading texture from a 32-bit pixel array.
		Texture( GLuint* pixels, GLuint width, GLuint height );

		//Constructor for loading texture from a PNG/BMP filepath.
		Texture( const char *path );

		//Constructor for loading texture from a PNG/BMP filepath.
		Texture(std::string path);

		//Note: Calls freeTexture().
		~Texture();

		//Loads a texture from a 32-bit pixel array, given width and height.
		//(If the Texture object already has a valid textureID, then the current textureID is freed and the new texture is loaded.)
		bool loadTextureFromPixels32( GLuint* pixels, GLuint width, GLuint height );

		//Loads a texture from a .png filepath.
		bool loadTextureFromPNG( const char *path );

		//Loads a texture from a .bmp filepath.
		bool loadTextureFromBMP( const char *path );

		void loadCheckerboard();

		//Deletes the current texture and nulls the texture GLuint.
		void freeTexture();

		//Set this texture to the active drawing texture (GL).
		inline void setAsActiveTexture() { glActiveTexture(GL_TEXTURE0); glBindTexture( GL_TEXTURE_2D, mTextureID); };

		//UNUSED
		void render( GLfloat x, GLfloat y );

		//Returns texture ID as GLuint.
		inline GLuint getTextureID() { return mTextureID; };

		//Get texture width.
		inline GLuint getWidth() { return mSize.getWidth(); };

		//Get texture height.
		inline GLuint getHeight() { return mSize.getHeight(); };

		//Get texture size.
		inline Size getSize() { return mSize; };

		//Get texture size as Vec2d.
		inline Vec2d getSizeVec2d() { return Vec2d((float)mSize.getWidth(), (float)mSize.getHeight()); };

		//Get width to height ratio of texture.
		inline GLfloat getRatio() { return GLfloat(mSize.getWidth())/GLfloat(mSize.getHeight()); };

		//Set whether texture should wrap.
		inline void setWrap(bool shouldWrap)
		{
			if(shouldWrap != mShouldWrap)
			{
				mShouldWrap = shouldWrap; 
				facilitateWrap();
			}
		}
};

#endif