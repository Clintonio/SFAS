//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// An imovable object which can support the player. 
// 
// Add a summary of your changes here:
// 
// 
//

#pragma once

#include "Entity.h"


namespace SFAS
{

namespace Game                  
{          
class Player;

class Bullet : public Entity
{
public:
	Bullet( );

	void Update( World * world, float dt );
	void Fire( float vx, float vy, Player * owner );

	bool IsPlayerControlled() const { return true; }
	
	static const Entity::EntityType kEntityType;
	
	// Get the entity type for this entity
	const Entity::EntityType GetEntityType() { return kEntityType; }
private: 

	WCHAR * ToString()  const { return L"Bullet"; }

	enum { kLifetime = 10, kForce = 2000 };

	static const float sSize;
	static const float sMass;
	static const float sDamping;

	// The player that fired this bullet
	Player * m_Owner;

	void OnCollision( Entity& other, World * world );
	
};
}
}