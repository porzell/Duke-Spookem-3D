#include "Player.h"
#include "Map.h"

#include "ResourceManager.h"

#include "TextureManager.h"
#include "EntityManager.h"
#include "FontManager.h"

#include "SoundEngine.h"

#include "Grenade.h"
#include "StickyGrenade.h"

#include "Texture.h"

#include "Display.h"

#include "EventKeyDown.h"
#include "EventKeyUp.h"
#include "EventMouseDown.h"
#include "EventMouseMove.h"
#include "EventMouseUp.h"

#include "EventLose.h"

#include "EventSystem.h"

#include "MenuStack.h"

#include "OptionsMenu.h"
#include "MainMenu.h"

#include "IceBolt.h"

#include "Kick.h"

#include "Utils.h"

#include "FlamePuff.h"

#include "Piss.h"

extern EventSystem *gpEventSystem;

float Player::msHeight = 0.25f;

const float HAND_SPEED = 40.0f;

const float MAX_HP = 100.0f;
const float MAX_MP = 100.0f;

Player::Player() : Entity()
{

	mForwardPressed = false;
	mBackwardPressed = false;
	mLeftPressed = false;
	mRightPressed = false;
	mWalkPressed = false;
	
	mIsSnapchatting = false;
	mSnapchatted = false;
	mIsAttacking = false;
	mIsKicking = false;
	mIsPeeing = false;
	mIsFlamethrowing = false;

	mType = ENTITY_PLAYER;

	mpHandAnim = New Animation(*game->getAnimationManager()->get("spellhand"));

	mpLeftHandAnim = New Animation(*game->getAnimationManager()->get("selfie"));

	mpLeftHandAnim->setScale(Vec2d(1.25,1.25));

	mpLegAnim = New Animation(*game->getAnimationManager()->get("kick"));

	mpHandAnim->setScale(Vec2d(4,4));
	mpLegAnim->setScale(Vec2d(10,10));

	mHandPosition = game->getDisplay()->getDimensions() - mpHandAnim->getCurrentDimensions();
	mLeftHandPosition = Vec2d(0,-mpLeftHandAnim->getCurrentDimensions().Y());
	mLegPosition = Vec2d(350, game->getDisplay()->getDimensions().Y() - mpLegAnim->getCurrentDimensions().Y());

	mHealth = MAX_HP;
	mp = MAX_MP;
	size = 2;

	//mpSpeaking = nullptr;
	//mpPeeing = nullptr;

	gpEventSystem->addListener(EVENT_MOUSEMOVE, this);
	gpEventSystem->addListener(EVENT_MOUSEUP, this);
	gpEventSystem->addListener(EVENT_MOUSEDOWN, this);
	gpEventSystem->addListener(EVENT_KEYUP, this);
	gpEventSystem->addListener(EVENT_KEYDOWN, this);

	mViewBob = 0;
	mVoiceVolume = 1.0f;

	mLastPiss.clear();
}

Player::~Player()
{
	//if(mpSpeaking)
		//mpSpeaking->drop();

	//if(mpPeeing)
	//{
		//mpPeeing->drop();
	//}

	delete mpHandAnim;
	delete mpLeftHandAnim;
	delete mpLegAnim;
}

void Player::speak(std::string soundFile, bool shouldInterrupt, bool shouldAddToQueue)
{
    fprintf(stderr, "Speaking %s\n", soundFile.c_str());
    if(mpSpeaking == NULL)
    {
        mpSpeaking = game->getSoundEngine()->play2DSoundKeep(soundFile, mVoiceVolume);
    }
    else if(mpSpeaking->isFinished() || shouldInterrupt)
    {
        mpSpeaking->cleanup();
        delete mpSpeaking;

        mpSpeaking = game->getSoundEngine()->play2DSoundKeep(soundFile, mVoiceVolume);
    }
    else if(shouldAddToQueue)
    {
        mQueuedQuips.push(soundFile);
    }
}

void Player::handleKey(bool keyDown, int keycode)
{
	switch(keycode)
	{
		case GLFW_KEY_W:
			mForwardPressed = keyDown;
			break;
		case GLFW_KEY_S:
			mBackwardPressed = keyDown;
			break;
		case GLFW_KEY_A:
			mLeftPressed = keyDown;
			break;
		case GLFW_KEY_D:
			mRightPressed = keyDown;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			mWalkPressed = keyDown;
		case GLFW_KEY_E:
			if(!keyDown)
				game->interact();
			break;
		case GLFW_KEY_C:
			if(keyDown)
			{
				if(mp >= 5 * game->getHardness())
				{
					Vec3d shotVelocity;

					shotVelocity.x = sin((mAngle.x * PI) / 180);
					shotVelocity.y = -sin((mAngle.y * PI) / 180);
	
					//Use this line if dealing with a non physics-enabled projectile.
					//shotVelocity.Y = 0;
	
					shotVelocity.z = -cos((mAngle.x * PI) / 180);

					shotVelocity /= 10;

					game->getEntityManager()->add(New IceBolt(mPosition-Vec3d(0,msHeight,0), shotVelocity, game->getAnimationManager()->get("iceball"), Vec2d(5,10), ENTITY_ATTACK_PLAYER));
					game->getSoundEngine()->play2DSound(game->getResourceManager()->get("frost"));
					mIsAttacking = true;

					mp -= 5 * game->getHardness();
				}
			}
			break;
		case GLFW_KEY_L:
			game->load();
			break;
		case GLFW_KEY_Q:
			if(keyDown)
			{
				mIsKicking = true;

				//This is still here to provide a kick quote slightly before kicking.
				if(rand() % 5 == 0)
					speak(getQuipKick());
			}
			break;
		case GLFW_KEY_F:
			if(keyDown)
			{
				mSnapchatted = false;
				mIsSnapchatting = true;

                mSnapchatTimer.start();
			}
			else
			{
				mIsSnapchatting = false;
				mSnapchatted = false;
			}
			break;
		case GLFW_KEY_V:
		{
			mIsPeeing = keyDown;

			if(mIsPeeing)
			{
				game->getSoundEngine()->play2DSound(game->getResourceManager()->get("zipdown"));

				//if(rand() % 2 == 0)
					speak(getQuipUnzip(),true);

				//mpPeeing = game->getSoundEngine()->play2DSoundKeepLooped(game->getResourceManager()->get("spookem_peeing"), 1.0f, 1.0f);

				//std::cout << mpPeeing << std::endl;

				/*if(!mpPeeing)
				{
					mpPeeing = game->getSoundEngine()->play2DSoundKeepLooped(game->getResourceManager()->get("spookem_peeing"), 1.0f, 1.0f);
					mpPeeing->setIsLooped(true);
				}*/
			}
			else
			{
				//std::cout << mpPeeing << std::endl;

				//if(mpPeeing)
				//{
					//mpPeeing->stop();
					//mpPeeing->drop();
					//mpPeeing = nullptr;
				//}
				mLastPiss.clear();
			}

			/*else
			{
				game->getSoundEngine()->play2DSound(game->getResourceManager()->get("zipup"));
			}*/
			break;
		}
		case GLFW_KEY_X:
		{
			mIsFlamethrowing = keyDown;
			break;
		}
		case GLFW_KEY_P:
			if(keyDown)
			{
				game->getDisplay()->shouldTrapCursor(!game->getDisplay()->isCursorTrapped());
				game->getMenuStack()->push(New OptionsMenu());
			}
			break;
		case GLFW_KEY_O:
			if(keyDown)
			{
				game->getDisplay()->shouldTrapCursor(!game->getDisplay()->isCursorTrapped());
				game->getMenuStack()->push(New MainMenu());
			}
			break;
		default:
			break;
	}
}

std::vector<std::pair<int, int>> Player::getSections()
{
	std::vector<std::pair<int, int>> pos;
	for(float i = mPosition.x - 0.5f - size/2; i < mPosition.x + 0.5f + size/2; ++i)
	{
		for(float j = mPosition.z - 0.5f - size/2; j < mPosition.z + 0.5f + size/2; ++j)
		{
			pos.push_back(std::pair<int, int>(int(i + 0.5f), int(j + 0.5f)));
		}
	}
	return pos;
}

void Player::handleMouse(const Vec2d &delta)
{
	mAngle.x += delta.X()/5;
	mAngle.y += delta.Y()/5;
	
	if(mAngle.y > MAX_UPWARD_VIEWANGLE)
	{
		mAngle.y = MAX_UPWARD_VIEWANGLE;
	}
	else if(mAngle.y < MIN_DOWNWARD_VIEWANGLE)
	{
		mAngle.y = MIN_DOWNWARD_VIEWANGLE;
	}
}

void Player::think()
{
	if(mHealth < MAX_HP)
		mHealth += 0.01f;
	else if(mHealth < 0)
	{
		mHealth = 0;
	}
	if(mp < MAX_MP)
		mp += .05f;
	/*mVelocity.setLength(0);

	if(mForwardPressed)
		mVelocity.Z += PLAYER_SPEED;
	if(mBackwardPressed)
		mVelocity.Z -= PLAYER_SPEED;
	if(mLeftPressed)
		mVelocity.X += PLAYER_SPEED;
	if(mRightPressed)
		mVelocity.X -= PLAYER_SPEED;

	Vec3d angle = mAngle;

	angle.normalize();

	angle.Z = 1.0f;

	mVelocity *= angle;*/

	//Got some help from Yes's answer at: http://stackoverflow.com/questions/16201573/how-to-properly-move-the-camera-in-the-direction-its-facing

	GLfloat speed;

	if(mWalkPressed)
		speed = PLAYER_WALK_SPEED;
	else
		speed = PLAYER_RUN_SPEED;

	GLfloat pitchRadian = /*mAngle.Y * (PI / 180)*/ 0;
	GLfloat yawRadian = mAngle.x * (PI / 180);

	Vec3d mnPosition = mPosition;

	//Zero the velocity to start with
	mVelocity = glm::vec3(0);

	//Add velocity to position.
	if(mForwardPressed)
	{
		mVelocity.x = speed *  sinf( yawRadian ) * cosf( pitchRadian );
		mVelocity.y = speed * sinf( pitchRadian );
		mVelocity.z = speed *  -cosf( yawRadian ) * cosf( pitchRadian );
		mnPosition += mVelocity;
	}

	if(mBackwardPressed)
	{
		mVelocity.x = speed *  sinf( yawRadian ) * cosf( pitchRadian );
		mVelocity.y = speed * sinf( pitchRadian );
		mVelocity.z = speed *  -cosf( yawRadian ) * cosf( pitchRadian );
		mnPosition -= mVelocity;
	}

	if(mLeftPressed){
		mVelocity.x = speed *  cosf( yawRadian ) * cosf( pitchRadian );
		mVelocity.z = speed *  sinf( yawRadian ) * cosf( pitchRadian );
		mnPosition -= mVelocity * 0.5f;
	}

	if(mRightPressed){
		mVelocity.x = speed *  cosf( yawRadian ) * cosf( pitchRadian );
		mVelocity.z = speed *  sinf( yawRadian ) * cosf( pitchRadian );
		mnPosition += mVelocity * 0.5f;
	}
	
	if(game->getCurrentMap()->getTile(mnPosition.x + (mnPosition.x > mPosition.x ? .4 : -.4) , mPosition.z).type == TYPE_WALL)
		mnPosition.x = mnPosition.x > mPosition.x ? int(mPosition.x) + 0.6f : int(mPosition.x) + 0.4f;

	if(game->getCurrentMap()->getTile(mPosition.x, mnPosition.z + (mnPosition.z > mPosition.z ? .4 : -.4)).type == TYPE_WALL)
		mnPosition.z = mnPosition.z > mPosition.z ? int(mPosition.z) + 0.6f : int(mPosition.z) + 0.4f;

	mPosition = mnPosition;

	if(mIsSnapchatting)
	{
		if(mLeftHandPosition.Y() < mpLeftHandAnim->getCurrentDimensions().Y())
			mLeftHandPosition.addY(HAND_SPEED);
		else
		{
            if(!mSnapchatted && mSnapchatTimer.getElapsedTime() >= 1000)
            {
                game->getSoundEngine()->play2DSound(game->getResourceManager()->get("camera_shutter"), 1.0f);

                mSnapchatted = true;

                if(rand() % 5 < 4)
                    speak(getQuipSelfie());

                mSnapchatTimer.stop();
            }

            //mIsSnapchatting = false;
		}

		//Make attack hand go down.
		if(mHandPosition.Y() >= -mpHandAnim->getCurrentDimensions().Y());
			mHandPosition.addY(-HAND_SPEED);
	}
	else if(mLeftHandPosition.Y() > -mpLeftHandAnim->getCurrentDimensions().Y())
	{
		mLeftHandPosition.addY(-HAND_SPEED);

		if(mHandPosition.Y() < 0)
			mHandPosition.addY(HAND_SPEED);
	}



	if(mIsAttacking)
		mHandPosition.addY(HAND_SPEED);
	else if(mHandPosition.Y() > -mpHandAnim->getCurrentDimensions().Y()/4)
		mHandPosition.addY(-HAND_SPEED);
	else if(mHandPosition.Y() < -mpHandAnim->getCurrentDimensions().Y())
		mHandPosition.addY(HAND_SPEED);

	if(mHandPosition.Y() >= mpHandAnim->getCurrentDimensions().Y()/4)
	{
		mIsAttacking = false;
	}

	//mLegPosition.setX(350 * (game->getDisplay()->getDimensions().X() / game->getDisplay()->getDimensions().Y()));

	if(mIsKicking)
		mLegPosition.addY(HAND_SPEED);
	else if(mLegPosition.Y() > -mpLegAnim->getCurrentDimensions().Y())
		mLegPosition.addY(-HAND_SPEED);

	if(mLegPosition.Y() >= 0)
	{
		game->getEntityManager()->add(New Kick(mPosition));
		mIsKicking = false;
	}

	if (mVelocity.x > 0 || mVelocity.y > 0 || mVelocity.z > 0)
	{
		mViewBob += PI * speed;
	}

	if(mIsPeeing)
	{
		Vec3d shotVelocity;

			shotVelocity.x = sin((mAngle.x * PI) / 180);
			shotVelocity.y = -sin(((mAngle.y - 20) * PI) / 180);
	
			//Use this line if dealing with a non physics-enabled projectile.
			//shotVelocity.Y = 0;
	
			shotVelocity.z = -cos((mAngle.x * PI) / 180);

			shotVelocity /= 16;

			//game->getEntityManager()->add(New FlamePuff(mPosition-Vec3d(0,msHeight,0), shotVelocity, game->getAnimationManager()->get("fireball"), Vec2d(0,5), ENTITY_ATTACK_PLAYER));
			std::string pissName = game->getEntityManager()->add(New Piss(mLastPiss, mPosition-Vec3d(0,0.50,0), shotVelocity, ENTITY_PLAYER_PISS));

			mLastPiss = pissName;

		//mpPeeing->setIsLooped(true);
	}

	if(mIsFlamethrowing)
	{
		Vec3d shotVelocity;

			shotVelocity.x = sin((mAngle.x * PI) / 180);
			shotVelocity.y = -sin(((mAngle.y - 20) * PI) / 180);
	
			//Use this line if dealing with a non physics-enabled projectile.
			//shotVelocity.Y = 0;
	
			shotVelocity.z = -cos((mAngle.x * PI) / 180);

			shotVelocity /= 20;

			game->getEntityManager()->add(New FlamePuff(mPosition-Vec3d(0,msHeight,0), shotVelocity, game->getAnimationManager()->get("fireball"), Vec2d(0,5), ENTITY_ATTACK_PLAYER));
			//std::string pissName = game->getEntityManager()->add(New Piss(mLastPiss, mPosition-Vec3d(0,0.50,0), shotVelocity, ENTITY_PLAYER_PISS));

			//mLastPiss = pissName;

		//mpPeeing->setIsLooped(true);
	}

	//std::cerr << mPosition.X << ',' << mPosition.Z << '\n';

	//If we're done talking and there are queued quips, then take one out of the queue and say it.
	//if(mpSpeaking && mpSpeaking->isFinished())
	//{
		////mpSpeaking->stop();
		////mpSpeaking->drop();

		//if(!mQueuedQuips.empty())
		//{
			//speak(mQueuedQuips.front());
			//mQueuedQuips.pop();
		//}
		//else
		//{
			//mpSpeaking->drop();
			//mpSpeaking = NULL;
		//}
	//}
}

void Player::attack(int button)
{
	if(mIsSnapchatting)
		return;

	ResourceManager *resources = game->getResourceManager();

	Vec3d shotVelocity;

	shotVelocity.x = sin((mAngle.x * PI) / 180);
	shotVelocity.y = -sin((mAngle.y * PI) / 180);
	
	//Use this line if dealing with a non physics-enabled projectile.
	//shotVelocity.Y = 0;
	
	shotVelocity.z = -cos((mAngle.x * PI) / 180);

	shotVelocity /= 10;

	if(button == GLFW_MOUSE_BUTTON_1 && mp > 5 * game->getHardness())
	{
		game->getEntityManager()->add(New StickyGrenade(mPosition-Vec3d(0,msHeight,0), shotVelocity, game->getAnimationManager()->get("fireball"), Vec2d(5,10), ENTITY_ATTACK_PLAYER, 4.0));
		mp -= 5 * game->getHardness();
		mIsAttacking = true;
		game->getSoundEngine()->play2DSound(resources->get("flame_attack"));
	}
	else if(button == GLFW_MOUSE_BUTTON_2 && mp > 7.5 * game->getHardness())
	{
		game->getEntityManager()->add(New Grenade(mPosition-Vec3d(0,msHeight,0), shotVelocity, game->getAnimationManager()->get("bomb"), Vec2d(5,10), ENTITY_ATTACK_PLAYER, 30.0));
		mp -= 7.5 * game->getHardness();
		mIsAttacking = true;
		game->getSoundEngine()->play2DSound(resources->get("throw_bomb"));
	}
}

void Player::draw()
{
	glRotatef(mAngle.y,1,0,0);
	glRotatef(mAngle.x,0,1,0);
	glTranslatef(-mPosition.x,-mPosition.y,-mPosition.z);

	glTranslatef(0,msHeight,0);

	float pos[3];

	pos[0] = mPosition.x;
	pos[1] = mPosition.y;
	pos[2] = mPosition.z;

	float diffuse[4] = { .6, .6, .6, 1.0 };
	float specular[4] = { 1, 1, 1, 1 };

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
}

void Player::drawHUD()
{
		Vec2d handdim = mpHandAnim->getCurrentDimensions();

		mpHandAnim->getCurrentSprite()->getTexture().setAsActiveTexture();


		Vec2d mHandPositionA = mHandPosition + Vec2d(cos(mViewBob/2) * 50,sin(mViewBob) * 50);
		//game->getTextureManager()->get("spellhand")->setAsActiveTexture();

		glBegin(GL_QUADS);

		glColor3f(1,1,1);
		glTexCoord2f(0.0f, 0.0f); glVertex2f( mHandPositionA.X(), mHandPositionA.Y());	// Bottom Left Of The Texture and Quad

		glColor3f(1,1,1);
		//glColor4ub(crBC.r, crBC.g, crBC.b, crBC.a);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( mHandPositionA.X() + handdim.X(), mHandPositionA.Y());	// Bottom Right Of The Texture and Quad

		glColor3f(1,1,1);
		//glColor4ub(crTC.r, crTC.g, crTC.b, crTC.a);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( mHandPositionA.X() + handdim.X(), mHandPositionA.Y() + handdim.Y());	// Top Right Of The Texture and Quad

		glColor3f(1,1,1);
		//glColor4ub(clTC.r, clTC.g, clTC.b, clTC.a);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( mHandPositionA.X() + 0.0, mHandPositionA.Y() + handdim.Y());	// Top Left Of The Texture and Quad

		glEnd();


		Vec2d legdim = mpLegAnim->getCurrentDimensions();

		legdim.setX(legdim.X() / 2);

		//Don't check if mKicking because then leg disappears sharply after kicking stops.
		//if(mIsKicking)
		{
			
			//mpLegAnim->getCurrentSprite()->drawPlain(mLegPosition, legdim, Vec2d(8,8));
			mpLegAnim->getCurrentSprite()->getTexture().setAsActiveTexture();


			//This draws backwards.
			glBegin(GL_QUADS);

			glColor3f(1,1,1);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( mLegPosition.X(), mLegPosition.Y());	// Bottom Left Of The Texture and Quad

			glColor3f(1,1,1);
			//glColor4ub(crBC.r, crBC.g, crBC.b, crBC.a);
			glTexCoord2f(0.5f, 0.0f); glVertex2f( mLegPosition.X() + legdim.X(), mLegPosition.Y());	// Bottom Right Of The Texture and Quad

			glColor3f(1,1,1);
			//glColor4ub(crTC.r, crTC.g, crTC.b, crTC.a);
			glTexCoord2f(0.5f, 1.0f); glVertex2f( mLegPosition.X() + legdim.X(), mLegPosition.Y() + legdim.Y());	// Top Right Of The Texture and Quad

			glColor3f(1,1,1);
			//glColor4ub(clTC.r, clTC.g, clTC.b, clTC.a);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( mLegPosition.X() + 0.0, mLegPosition.Y() + legdim.Y());	// Top Left Of The Texture and Quad

			glEnd();

		}

		mpLeftHandAnim->getCurrentSprite()->getTexture().setAsActiveTexture();

		Vec2d lefthanddim = mpLeftHandAnim->getCurrentDimensions();

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f( mLeftHandPosition.X(), mLeftHandPosition.Y());	// Bottom Left Of The Texture and Quad

		//glColor4ub(crBC.r, crBC.g, crBC.b, crBC.a);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( mLeftHandPosition.X() + lefthanddim.X(), mLeftHandPosition.Y());	// Bottom Right Of The Texture and Quad

		//glColor4ub(crTC.r, crTC.g, crTC.b, crTC.a);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( mLeftHandPosition.X() + lefthanddim.X(), mLeftHandPosition.Y() + lefthanddim.Y());	// Top Right Of The Texture and Quad

		//glColor4ub(clTC.r, clTC.g, clTC.b, clTC.a);
		glTexCoord2f(0.0f, 1.0f); glVertex2f( mLeftHandPosition.X() + 0.0, mLeftHandPosition.Y() + lefthanddim.Y());	// Top Left Of The Texture and Quad

		glEnd();

		glDisable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex2f(10, 10);
		glVertex2f(210, 10);
		glVertex2f(210, 60);
		glVertex2f(10, 60);
		glColor3f(1 - (mHealth/MAX_HP), mHealth > MAX_HP * 0.9f ? 1 : mHealth / MAX_HP, mHealth > MAX_HP * 0.9f ? (mHealth - (MAX_HP * 0.9f)) / 10.0f : 0);
		glVertex2f(10, 10);
		glVertex2f(10 + ((mHealth / MAX_HP) * 200), 10);
		glVertex2f(10 + ((mHealth / MAX_HP) * 200), 60);
		glVertex2f(10, 60);
		glEnd();

		int w = game->getDisplay()->getWidth();
		glBegin(GL_QUADS);
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex2f(w - 10, 60);
		glVertex2f(w - 210, 60);
		glVertex2f(w - 210, 10);
		glVertex2f(w - 10, 10);
		glColor3f(1 - (mp/100.0f), 0, mp/100.0f);
		glVertex2f(w - 10, 60);
		glVertex2f(w - 10 - ((mp / MAX_MP) * 200), 60);
		glVertex2f(w - 10 - ((mp / MAX_MP) * 200), 10);
		glVertex2f(w - 10, 10);
		glEnd();
		glColor3f(1,1,1);
}

void Player::collide(Entity* other)
{
	if(other->getType() == ENTITY_ATTACK_ENEMY)
	{
		Vec2d damage = ((Projectile*)other)->getDamageRange();
		takeHit(randomFloat(damage.X(), damage.Y()));
		//game->getSoundEngine()->play2DSound(getQuipPain());
		speak(getQuipPain());
	}
	else if(other->getType() == ENTITY_ENEMY && !other->getFrozen())
		takeHit(0.05);
}

void Player::takeHit(float hpAmount)
{
	static bool saidDevQuote = false;

	float lastHP = mHealth;

	//Scale damage
	mHealth -= hpAmount * game->getHardness();

	if(mHealth <= 25 && lastHP > 25)
		speak(getQuipPain25(), false, true);

	else if(mHealth <= 50 && lastHP > 50)
		speak(getQuipPain50(), false, true);

	else if(mHealth <= 0 && lastHP > 0)
	{
		if(!saidDevQuote)
		{
			//speak(*game->getResourceManager()->get("player_death1"), true);
			saidDevQuote = true;
			//game->load();
			gpEventSystem->fireEvent(EventLose());
		}

		//mHealth = 100;
	}
}

void Player::heal(float hpAmount)
{
	mHealth = min(mHealth + hpAmount, MAX_HP);
	speak(getQuipHeal());
}

void Player::handleEvent( const Event& theEvent )
{
	switch(theEvent.getType())
	{
	case EVENT_MOUSEDOWN:
		if(game->getMenuStack()->empty())
			attack( ((EventMouseDown&)theEvent).getButton() );
		break;
	case EVENT_MOUSEMOVE:
		if(game->getMenuStack()->empty())
			handleMouse( ((EventMouseMove&)theEvent).getDelta() );
		break;		
	case EVENT_KEYUP:
		handleKey(false, ((EventKeyUp&)theEvent).getKey());
		break;
	case EVENT_KEYDOWN:
		handleKey(true, ((EventKeyDown&)theEvent).getKey());
		break;
    default:
        break;
	}
}

//Get the filename of a quote about decapitating an enemy.
std::string Player::getQuipDecap()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_decap" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_decap" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about toasting an enemy.
std::string Player::getQuipBurn()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_burn" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_burn" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about freezing an enemy.
std::string Player::getQuipFreeze()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_freeze" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_freeze" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about kicking an enemy.
std::string Player::getQuipKick()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_kick" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_kick" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about leaving some enemies behind.
std::string Player::getQuipLeaveBehind()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_leavelevel" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_leavelevel" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about returning to a level when enemies are.
std::string Player::getQuipReturnLevel()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_returnlevel" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_returnlevel" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about having cleared a level.
std::string Player::getQuipLevelCleared()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_levelclear" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_levelclear" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about going to a level we've already cleared.
std::string Player::getQuipLevelAlreadyCleared()
{
	static vector<std::string*> toBeSaid;
	
	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_oldlevel" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_oldlevel" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about killing a swamp monster.
std::string Player::getQuipKillSwampMonster()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_kill_swampmonster" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_kill_swampmonster" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a quote about killing a minotaur.
std::string Player::getQuipKillMinotaur()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_kill_minotaur" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_kill_minotaur" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;

}

//Get the filename of a quote about killing a nazi.
std::string Player::getQuipKillNazi()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_kill_nazi" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_kill_nazi" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;

}

//Get the filename of a quote about killing a cyborg.
std::string Player::getQuipKillCyborg()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_kill_cyborg" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_kill_cyborg" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;

}

//Get the filename of a quote about killing a zombie.
std::string Player::getQuipKillZombie()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_kill_zombie" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_kill_zombie" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a generic after kill quote.
std::string Player::getQuipKill()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_kill" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_kill" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a generic pain quote.
std::string Player::getQuipPain()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_pain" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_pain" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a generic 25 health quote.
std::string Player::getQuipPain25()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_25health" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_25health" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a generic 50 health quote.
std::string Player::getQuipPain50()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_50health" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_50health" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of an explosion quote.
std::string Player::getQuipExplosion()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_explosion" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_explosion" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a new level quote.
std::string Player::getQuipNewLevel()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_newlevel" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_newlevel" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a shatter quote.
std::string Player::getQuipShatter()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_shatter" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_shatter" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a healing quote.
std::string Player::getQuipHeal()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_heal" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_heal" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a selfie quote.
std::string Player::getQuipSelfie()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_selfie" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_selfie" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a responding to monster screaming quote.
std::string Player::getQuipRespondScreams()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_respond_screams" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_respond_screams" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a setting a mnoster on fire quote.
std::string Player::getQuipSetFire()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_setonfire" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_setonfire" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of an entering nazi level quote.
std::string Player::getQuipEnterNazi()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_enternazi" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_enternazi" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of an entering tech level quote.
std::string Player::getQuipEnterTech()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_entertech" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_entertech" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a head golf quote.
std::string Player::getQuipHeadGolf()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_headgolf" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_headgolf" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a Hitler taunt quip.
std::string Player::getQuipTauntHitler()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_taunt_hitler" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_taunt_hitler" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a Hitler death quip.
std::string Player::getQuipKillHitler()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_kill_hitler" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_kill_hitler" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a Hitler encounter quip.
std::string Player::getQuipEncounterHitler()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_hitler_spawn" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_hitler_spawn" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

//Get the filename of a thaw enemy quip.
std::string Player::getQuipThaw()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_thaw" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_thaw" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}


//Get the filename of a quote about taking out the goldenrod.
std::string Player::getQuipUnzip()
{
	static vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("player_unzip" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("player_unzip" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

void Player::writeSave(FILE* outfile, int pos)
{
	/*fputc(int(mPosition.X + 0.5f), outfile);
	fputc(int(mPosition.Z + 0.5f), outfile);*/

	fwrite(&mPosition.x, sizeof(mPosition.x), 1, outfile);
	fwrite(&mPosition.z, sizeof(mPosition.z), 1, outfile);

	fwrite(&mAngle.x, sizeof(mAngle.x), 1, outfile);
	fwrite(&mAngle.y, sizeof(mAngle.y), 1, outfile);
	fwrite(&mAngle.z, sizeof(mAngle.z), 1, outfile);

	fwrite(&mHealth, sizeof(mHealth), 1, outfile);
	fwrite(&mp, sizeof(mHealth), 1, outfile);
}

void Player::readSave(FILE* infile, int pos)
{
	/*mPosition.x = fgetc(infile);
	mPosition.z = fgetc(infile);*/

	fread(&mPosition.x, sizeof(mPosition.x), 1, infile);
	fread(&mPosition.z, sizeof(mPosition.z), 1, infile);

	fread(&mAngle.x, sizeof(mAngle.x), 1, infile);
	fread(&mAngle.y, sizeof(mAngle.y), 1, infile);
	fread(&mAngle.z, sizeof(mAngle.z), 1, infile);

	fread(&mHealth, sizeof(mHealth), 1, infile);
	fread(&mp, sizeof(mHealth), 1,infile);
}
