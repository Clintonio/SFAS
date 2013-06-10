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
#include "ShipEntity.h"
#include "Level.h"

namespace Engine
{
	class Sound;
	class SoundProvider;
}

namespace SFAS
{

namespace Game                  
{          

class Enemy : public ShipEntity
{
public:
	Enemy( EnemyType & type, float x, float y );
	virtual ~Enemy(void);

	virtual void Update( World * world, float dt );

	static const float sSpeed;
	static const float sMass;
	static const float sDamping;

	// Each enemy has a different kill score
	const unsigned int GetKillScore() const 
	{
		if( m_Health < 1 )
		{
			return m_KillScore;
		}
		return 0;
	}

	static const Entity::EntityType kEntityType;
	
	// Get the entity type for this entity
	const Entity::EntityType GetEntityType() const { return kEntityType; }
private:
	enum AIRoutine 
	{
		suicide,
		avoid
	};

	WCHAR * ToString()  const { return L"Enemy"; }

	void SetAIRoutine( std::string & type );
	// Overridden collision
	bool OnCollision( Entity& other, World * world );
	// Override the sound loading
	void LoadSounds(Engine::SoundProvider* soundProvider);
	bool OnBulletHit( Entity * other );

	WeaponType*			m_WeaponType;
	AIRoutine			m_AIRoutine;
	float				m_LastFireTime;
	int					m_Health;
	unsigned int		m_KillScore;

	static Engine::Sound* sExplosionSound;
};
}
}
