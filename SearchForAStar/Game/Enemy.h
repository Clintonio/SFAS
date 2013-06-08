//
// Source files for Search For A Star competition
// Copyright (c) 2010 L. Attwood
//
// Enemy class that patrols, hurts the player.
// 
// Add a summary of your changes here:
// 
// 
// 
#pragma once

#include "Entity.h"
#include "World.h"

namespace Engine
{
	class Sound;
	class SoundProvider;
}

namespace SFAS
{

namespace Game                  
{          

class Enemy : public Entity
{
public:
	Enemy( float x, float y );
	virtual ~Enemy(void);

	virtual void Update( World * world, float dt );

	static const float sSize;
	static const float sSpeed;
	static const float sMass;
	static const float sDamping;

	static const Entity::EntityType kEntityType;
	
	// Get the entity type for this entity
	const Entity::EntityType GetEntityType() { return kEntityType; }
private:
	WCHAR * ToString()  const { return L"Enemy"; }

	void OnCollision( Entity& other, World * world );

	// Override the sound loading
	void LoadSounds(Engine::SoundProvider* soundProvider);

	static Engine::Sound* sExplosionSound;
};
}
}
