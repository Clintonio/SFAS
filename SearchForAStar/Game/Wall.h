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


class Wall : public Entity
{
public:
	Wall( float x, float y, float width, float height );
	virtual ~Wall(void);

	virtual void Update( float dt );
	
	static Engine::RenderItem * sRenderItem;
	// Get the entity type for this entity
	const Entity::EntityType GetEntityType() { return kEntityType; }
protected: 
	virtual Engine::RenderItem * GetRenderItem() { return sRenderItem; }

private: 

	WCHAR * ToString()  const { return L"Wall"; }

	void OnCollision( Entity& other );
	
	static const Entity::EntityType kEntityType;
};
}
}