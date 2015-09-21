#pragma once
#ifndef _ENTITY_H
#define _ENTITY_H

#include "VectorMath.h"

#include "Trackable.h"

#include <vector>
#include <map>
#include <string>

#include "Header.h"

#include "Sprite.h"

#include "ISaveWritable.h"

class Animation;

enum entity_types {ENTITY_GENERIC, ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_ATTACK_PLAYER, ENTITY_ATTACK_ENEMY, ENTITY_PLAYER_DECAPITATABLE, ENTITY_PLAYER_KICK, ENTITY_HEALTH_POTION, ENTITY_GIB, ENTITY_PLAYER_PISS};

class Entity : public Trackable, public ISaveWritable
{
	private:
		std::string mID;
		//bool mHasOwner;
		
	protected:
		std::map<std::string, Entity*> mAttachments;

		bool mIsFrozen;

		Animation *mpAnim;
		Vec3d mPosition;

		bool mPaused;

		bool mSeenPlayer;

		bool mShouldDelete;

		bool mShouldDeleteAnim;

		entity_types mType;
		float size;
		
		float mHealth;

		//Set entity ID.
		inline void setID(std::string &id) { mID = id; };

	public:
		Entity();

		//Creates an entity with a provided Animation.
		Entity(Animation &animation);

		//Creates an entity with a provided Animation.
		Entity(Animation *animation, bool shouldDeleteAnim = true);

		//Creates an entity with a provided Animation and position.
		Entity(Animation &animation, Vec3d &position);

		//Creates an entity with a provided Animation and position.
		Entity(Animation *animation, Vec3d &position);

		//Creates an entity with a provided Animation, position, and scale.
		Entity(Animation *animation, Vec3d &position, Vec2d scale);

		Entity(const Entity& entity);

		virtual ~Entity();

		//Draws the entity.
		virtual void draw();

		//Handle collision.
		virtual void collide(Entity* other) {}

		//Handle death.
		virtual void kill() {};

		//Handle damage.
		virtual void takeDamage(float hp = 0) { mHealth -= hp; }

		virtual std::vector<std::pair<int, int>> getSections();

		//Add an entity attachment to this entity.
		inline virtual void addAttachment(Entity *attachment) { mAttachments.emplace(attachment->getID(), attachment); /*attachment->setOwned(true);*/ }

		//Remove a specific attachment.
		inline virtual void removeAttachment(Entity *toBeRemoved) { toBeRemoved->setShouldDelete(true); mAttachments.erase(toBeRemoved->getID()); }

		//Remove a specific attachment.
		inline virtual void removeAttachment(std::string key)
		{
			Entity *tmp = NULL;
			tmp = mAttachments[key];
			
			if(tmp)
			{
				tmp->setShouldDelete(true);
				mAttachments.erase(key);
			}
		}

		//Remove all attachments
		virtual void removeAllAttachments();

		//Get attachments map.
		std::map<std::string, Entity*>* getAttachments() { return &mAttachments; }

		//Get animation.
		Animation& getAnimation();

		//Gets whether paused.
		inline bool getPaused() { return mPaused; }

		//Sets pause.
		inline void setPaused(const bool pause) { mPaused = pause; }

		//Toggles animation pause.
		inline void togglePause() { mPaused = !mPaused; }

		//Update entity (run AI routines).
		virtual void think(const double mElapsedTime);

		//Get entity position.
		inline Vec3d getPosition() { return mPosition; }

		//Set entity position.
		inline void setPosition(Vec3d &position) { mPosition = position; }

		//See if entity deletion flag is set.
		inline bool shouldBeDeleted() { return mShouldDelete; };

		//Set whether entity's deletion flag is set.
		inline void setShouldDelete(bool shouldDelete) { mShouldDelete = shouldDelete; };
		
		//Get collision size.
		inline float getSize() {return size;}

		//Set collision size
		void setSize(float s) {size = s;}

		//Get entity type.
		inline entity_types getType(){return mType;}

		//Check if entity has owner.
		//inline bool hasOwner() { return mHasOwner; }

		//Set if entity has an owner.
		//inline void setOwned(bool owned) { mHasOwner = owned; }

		//Get center of entity;
		//inline Vec2d getCenter() { return (mPosition + mpAnim->getCurrentSprite()->getSizeAndScale()/2); };

		//Checks if a given Vec2d is within the bounds of the entity.
		//bool isWithinBounds(Vec2d point);

		//Use box-to-box (AABB) collision on another entity.
		bool isColliding(Entity *other);
		
		//Get entity ID.
		std::string getID() { return mID; };

		//Write entity savedata to file.
		virtual void writeSave(FILE* outfile, unsigned pos);
		
		//Read entity savedata from file.
		virtual void readSave(FILE* infile, unsigned pos);
		
		//Set entity as frozen.
		inline virtual void setFrozen(bool frozen) { mIsFrozen = frozen; }

		//Check if entity is frozen.
		inline virtual bool getFrozen() { return mIsFrozen; };

		//Spawn Gibs.
		inline virtual void spawnGib(float velocity) { };
		
		//Get entity health.
		virtual float getHealth() { return mHealth; }

		//Set entity health.
		virtual void setHealth(float hp) { mHealth = hp; }

		//Heal entity.
		virtual void heal(float hp) { mHealth += hp; }

		//Quiet all sounds.
		virtual void quiet() {};

		//Unquiet.
		virtual void unQuiet() {};

		friend class EntityManager;
};

#endif
