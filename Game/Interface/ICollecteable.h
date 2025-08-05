#pragma once

#include "Actor/Actor.h"

class ICollecteable
{
public:
	virtual void OnCollected(class Player* player) abstract;
};