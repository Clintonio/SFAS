// An explosion entity

#pragma once

#include "Entity.h"

namespace SFAS
{
namespace Game
{

class Explosion : public Entity
{
public:
	Explosion( D3DXVECTOR3 position, D3DXVECTOR3 scale );

	// Get the entity type for this entity
	static const Entity::EntityType kEntityType;
	const Entity::EntityType GetEntityType() const { return kEntityType; }
private:
	void Update( World * world, float dt );
	bool OnCollision( Entity& other, World * world );
	void ShowExplosionStage( const int stage );

	WCHAR * ToString()  const { return L"Explosion"; }

	float m_ExplosionTime;
	int m_CurrentStage;

	static const float sExplosionLength;
	static const int sStageCount;
};
}
}