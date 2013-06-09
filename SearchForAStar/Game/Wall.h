//
// Source files for Search For A Star competition
// Copyright (c) 2012 L. Attwood
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
class World;

class Wall : public Entity
{
public:
	Wall( float x, float y, float width, float height );
	virtual ~Wall(void);

	virtual void Update( float dt );
	
	// Get the entity type for this entity
	const Entity::EntityType GetEntityType() const { return kEntityType; }
private: 

	WCHAR * ToString()  const { return L"Wall"; }

	bool OnCollision( Entity& other, World* world );
	
	static const Entity::EntityType kEntityType;
};
}
}