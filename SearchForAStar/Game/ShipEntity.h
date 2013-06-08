// We wish to differenciate between ships and non-ships
// since ships are a core concept to the game
// and are shared by enemies and player

#pragma once

#include "Entity.h"

using namespace SFAS::Game;

namespace SFAS
{
namespace Game
{
class ShipEntity : public Entity
{
public:
	ShipEntity( const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, float damping ) : Entity(pos, scale, damping) {} 

	virtual bool OnBulletHit( Entity &other ) { return true; }
};
}
}