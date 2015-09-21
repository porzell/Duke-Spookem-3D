#pragma once
#ifndef _SPRITE_H
#define _SPRITE_H

#include "GLincludes.h"

#include "Trackable.h"
#include "VectorMath.h"

class Texture;
struct Color;

class Sprite : public Trackable
{
	private:
		Texture *mpTexture;
		Vec2d mTexPosition;
		Vec2d mSize;
		Vec2d mScale;

		//float mRotation;

	public:
		Sprite();
		Sprite(Texture *texture);
		Sprite(Texture *texture, Vec2d scale);
		Sprite(Texture *texture, Vec2d location, Vec2d size);
		Sprite(Texture *texture, Vec2d location, Vec2d size, Vec2d scale);

		~Sprite() {};

		inline Vec2d getSize() { return mSize; };
		inline Vec2d getScale() { return mScale; };
		inline Vec2d getSizeAndScale() { return mSize * mScale; };
		inline Texture& getTexture() { return *mpTexture; };
		inline Vec2d getTexturePosition() { return mTexPosition; };

		inline void setSize(const Vec2d& size) { mSize = size; };
		inline void setTexture(Texture& texture) { mpTexture = &texture; };
		inline void setTexturePosition(const Vec2d& pos) { mTexPosition = pos; };

		virtual void draw(Vec3d &location);
		virtual void drawScaled(Vec3d &location, Vec2d &scale);
		virtual void drawScaledTinted(Vec3d &location, Vec2d &scale, Color &tint);
		virtual void drawScaledTintedRotated(Vec3d &location, Vec2d &scale, Color &tint, float rotation);

		virtual void drawPlain(Vec2d &location, Vec2d &bounds, Vec2d &scale);
};

#endif
