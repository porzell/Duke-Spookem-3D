#include "Animation.h"

Animation::Animation()
{
	mpCurSprite = NULL;
	mFrame = 0;
	mFrameTime = 0;
	mElapsedTime = 0;
	mRotation = 0;
	mLoopable = true;
	mPingPong = false;
	mBackwards = false;

	mPaused = false;

	mScale = Vec2d(1.0f,1.0f);
	mTint = Color(1.0f,1.0f,1.0f);
}


Animation::~Animation()
{

}

Animation::Animation(Sprite *sprite)
{
	mpCurSprite = sprite;
	addFrameToEnd(sprite);
	
	mFrame = 0;
	mFrameTime = 0;
	mElapsedTime = 0;
	mRotation = 0;
	mLoopable = true;
	mPingPong = false;
	mBackwards = false;

	mPaused = false;

	mScale = Vec2d(1.0f,1.0f);
	mTint = Color(1.0f,1.0f,1.0f);
}

Animation::Animation(const Animation& animation)
{
	mSprites = animation.mSprites;
	if(mSprites.size() > 0)
		mpCurSprite = *(mSprites.begin());
	else
		mpCurSprite = NULL;
	mFrameTime = animation.mFrameTime;
	mLoopable = animation.mLoopable;
	mPingPong = animation.mPingPong;
	mBackwards = false;
	
	mFrame = animation.mFrame;//animation.mFrame;
	mElapsedTime = animation.mElapsedTime;

	mRotation = 0;

	mPaused = false;

	mScale = animation.mScale;
	mTint = animation.mTint;
}

void Animation::advance()
{
	if(mSprites.size() < 1)
		return;

	/*if(mpCurSprite == &(*(mSprites.end())))
	{
		if(mLoopable)
		{
			//If loopable, set the frame pointer back to our first frame.	Same as &(mSprites[0]).
			mpCurSprite = &(*(mSprites.begin()));
			mFrame = 0;
		}
	}*/
	if(!mLoopable)
	{
		//if(mpCurSprite == &(*(mSprites.end())))
		if(mSprites.size() > 0 && mpCurSprite == mSprites[mSprites.size()-1])
			return;
		else
		{
			mFrame++;
			mpCurSprite = mSprites[mFrame];
		}
	}
	else
	{
		if(!mBackwards)
		{
			mFrame++;
			mFrame %= mSprites.size();

			mpCurSprite = mSprites[mFrame];

			if(mPingPong)
				if(mpCurSprite == mSprites[mSprites.size()-1])
					mBackwards = true;
		}
		else
		{
			mFrame--;
			mFrame %= mSprites.size();

			mpCurSprite = mSprites[mFrame];

			if(mpCurSprite == mSprites[0])
				mBackwards = false;
		}
	}
}

Sprite* Animation::getCurrentSprite()
{
	return mpCurSprite;
}

//DEPRECATED:
/*void Animation::freeAllFrames()
{
	for(std::vector<Sprite*>::iterator i = mSprites.begin(); i != mSprites.end(); ++i)
	{
		if((*i)->mpBuffer != NULL)
			delete (*i)->mpBuffer;
	}
}*/

void Animation::addFrameToEnd(Sprite *frame)
{
	mSprites.push_back(frame);

	//If this is our first frame inserted, set our frame pointer to the frame we just entered.
	if(mSprites.size() == 1)
		mpCurSprite = *(mSprites.begin());
}

void Animation::addFrame(Sprite *frame, int pos)
{
	mSprites.insert(mSprites.begin() + pos, frame);
	
	//If this is our first frame inserted, set our frame pointer to the frame we just entered.
	if(mSprites.size() == 1)
		mpCurSprite = *(mSprites.begin());
}

void Animation::removeFrame(int pos)
{
	mSprites.erase(mSprites.begin() + pos);
}

void Animation::removeFrameAtEnd()
{
	mSprites.pop_back();
}

void Animation::update(const double elapsedTime)
{
	if(!mPaused)
	{
		addElapsedTime(elapsedTime);

		if(mElapsedTime > mFrameTime)
		{
			advance();
			mElapsedTime -= mFrameTime;
		}
	}
}

void Animation::draw(Vec3d &location)
{
	mpCurSprite->drawScaledTintedRotated(location, mScale, mTint, mRotation);
	//al_draw_tinted_scaled_rotated_bitmap_region(mpCurSprite->mpBuffer->mpBitmap,mpCurSprite->mPosition.X(),mpCurSprite->mPosition.Y(),(float)(mpCurSprite->mSize.getWidth()),(float)(mpCurSprite->mSize.getHeight()),mTint.getAllegroColor(),(float)mpCurSprite->mSize.getWidth()/2,(float)mpCurSprite->mSize.getHeight()/2,location.X()+mpCurSprite->mSize.getWidth()/2,location.Y()+mpCurSprite->mSize.getHeight()/2,mpCurSprite->mScale.X()*mScale.X(),mpCurSprite->mScale.Y()*mScale.Y(),mpCurSprite->mRotation+mRotation,0);
	////mpCurSprite->draw(buffer,location);
}

/*void Animation::drawScaledFromTopLeftCorner(GraphicsBuffer &buffer, Vec2d &location)
{
	al_set_target_bitmap(buffer.mpBitmap);
	al_draw_tinted_scaled_rotated_bitmap_region(mpCurSprite->mpBuffer->mpBitmap,mpCurSprite->mPosition.X(),mpCurSprite->mPosition.Y(),(float)(mpCurSprite->mSize.getWidth()),(float)(mpCurSprite->mSize.getHeight()),mTint.getAllegroColor(),0,0,location.X()+mpCurSprite->mSize.getWidth()/2,location.Y()+mpCurSprite->mSize.getHeight()/2,mpCurSprite->mScale.X()*mScale.X(),mpCurSprite->mScale.Y()*mScale.Y(),mpCurSprite->mRotation+mRotation,0);
}*/