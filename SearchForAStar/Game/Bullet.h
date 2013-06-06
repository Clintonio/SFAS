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


class Bullet : public Entity
{
public:
	Bullet( );

	void Update( World * world, float dt );
	void Fire( float vx, float vy );

	bool IsPlayerControlled() const { return true; }
	
	static Engine::RenderItem * sRenderItem;
	static const Entity::EntityType kEntityType;
	
	// Get the entity type for this entity
	const Entity::EntityType GetEntityType() { return kEntityType; }
protected: 
	// Get the render item for this entity
	virtual Engine::RenderItem * GetRenderItem() { return sRenderItem; }

private: 

	WCHAR * ToString()  const { return L"Bullet"; }

	enum { kLifetime = 10, kForce = 2000 };

	static const float sSize;
	static const float sMass;
	static const float sDamping;

	void OnCollision( Entity& other );
	
};
}
}