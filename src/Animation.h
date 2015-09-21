#pragma once
#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <vector>

#include "Trackable.h"

#include "VectorMath.h"
#include "Sprite.h"
#include "Color.h"

class Animation : public Trackable
{
	private:
		Sprite *mpCurSprite;
		std::vector<Sprite*> mSprites;

		Color mTint;

		double mFrameTime;
		double mElapsedTime;

		bool mLoopable;
		bool mPingPong;
		bool mBackwards;
		
		bool mPaused;

		unsigned int mFrame;

		float mRotation;
		Vec2d mScale;
		//Color mTint;

	public:
		Animation();
		
		Animation(Sprite *sprite);
		
		Animation(const Animation& animation);

		~Animation();


		//DEPRECATED
		//Frees the memory for all of the Sprites' bitmap data.
		//void freeAllFrames(); 

		//Advance 1 frame.
		void advance();

		//Add a sprite frame to the end of the Animation.
		void addFrameToEnd(Sprite *frame);

		//Adds a frame into the Animation at a specified position.
		void addFrame(Sprite *frame, int pos);

		//Removes a frame from the Animation at a specified position.
		void removeFrame(int pos);

		//Removes the last added frame in the Animation.
		void removeFrameAtEnd();

		//Update animation.
		void update(const double mElapsedTime);

		//Get a pointer to the current Sprite of the animation.
		Sprite* getCurrentSprite();


		//Get milliseconds per frame.
		const inline double getFrameTime() { return mFrameTime; };

		//Get elapsed time.
		const inline double getElapsedTime() { return mElapsedTime; };

		//Get whether animation is loopable.
		const inline bool getLoopable() { return mLoopable; };

		//Get whether animation should ping pong.
		const inline bool getPingPong() { return mPingPong; };

		//Get animation Tint.
		const inline Color& getTint() { return mTint; };

		//Get animation scale.
		inline Vec2d getScale() { return mScale; };

		//Get animation rotation.
		const inline float getRotation() { return mRotation; };

		//Gets whether paused.
		inline bool getPaused() { return mPaused; };

		//Set frame time (in milliseconds).
		inline void setFrameTime(const double time) { mFrameTime = time; };

		//Set elapsed time.
		inline void setElapsedTime(const double time) { mElapsedTime = time; };

		//Set whether Animation is loopable.
		inline void setLoopable(bool loopable) { mLoopable = loopable; };

		//Set whether Animation should ping pong.
		inline void setPingPong(bool pingpong) { mPingPong = pingpong; };

		//Set animation tint.
		inline void setTint(Color &tint) { mTint = tint; };

		//Set animation scale.
		inline void setScale(Vec2d scale) { mScale = scale; };

		//Set animation rotation.
		inline void setRotation(float rotation) { mRotation = rotation; };

		//Add elapsed time.
		inline void addElapsedTime(const double time) { mElapsedTime += time; };

		//Sets pause.
 		inline void setPaused(const bool pause) { mPaused = pause; };

		//Toggles animation pause.
		inline void togglePause() { mPaused = !mPaused; };

		//Get current dimensions of animation frame.
		inline Vec2d getCurrentDimensions() { return mpCurSprite->getSize() * mScale; };//{ return mpCurSprite->getSizeAndScale() * mScale; };


		virtual void draw(Vec3d &location);

		//virtual void drawScaledFromTopLeftCorner(GraphicsBuffer &buffer, Vec2d &location);

		//This is unnecessary as default copy constructor can be used.
		//Animation& operator = (const Animation& animation);
};

#endif 