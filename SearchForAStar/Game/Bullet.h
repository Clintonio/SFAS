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
#include <map>
#include "WeaponType.h"

namespace Engine
{
	class Sound;
}

namespace SFAS
{

namespace Game                  
{          
class ShipEntity;

class Bullet : public Entity
{
public:
	Bullet( WeaponType* weaponType );

	void Update( World * world, float dt );
	void Fire( D3DXVECTOR3 direction, ShipEntity * owner );

	bool IsPlayerControlled() const;
	// Set the target ship for targetting weapons
	void SetTarget( const ShipEntity * other );
	// Get the owner of this bullet, if any
	const ShipEntity* GetOwner() const { return m_Owner; }
	
	static const Entity::EntityType kEntityType;
	// Get the entity type for this entity
	const Entity::EntityType GetEntityType() const { return kEntityType; }
private: 

	WCHAR * ToString()  const { return L"Bullet"; }

	static const float sMass;
	static const float sDamping;

	// The available bullet sounds
	static std::map<std::string, Engine::Sound*> sBulletSound;
	// The available bullet textures
	static std::map<std::string, Engine::RenderItem*> sRenderItem;
	// The player that fired this bullet
	ShipEntity * m_Owner;
	// Weapon type descriptor
	WeaponType* m_WeaponType;
	// Whether the sound has played or not
	bool m_SoundPlayed;
	// The target ship for targetting type weapons
	const ShipEntity * m_TargetShip;

	bool OnCollision( Entity& other, World * world );
	void LoadSounds(Engine::SoundProvider* soundProvider);
};
}
}