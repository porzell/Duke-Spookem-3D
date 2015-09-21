#pragma once
#include "Entity.h"
class Item :
	public Entity
{
public:
	Item(Animation &animation);
	~Item(){}
protected:
	int value;
};

