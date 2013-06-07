#include "Explosion.h"
#include <iostream>
#include <sstream>

using namespace SFAS::Game;

const Entity::EntityType Explosion::kEntityType(3);

const float Explosion::sExplosionLength = 0.15f;
const int Explosion::sStageCount = 7;

Explosion::Explosion( D3DXVECTOR3 position, D3DXVECTOR3 scale ) : Entity( position, scale, 0 ), 
	m_ExplosionTime(0), m_CurrentStage(0)
{
	SetActive(true);
	ShowExplosionStage(m_CurrentStage);
	SetMass( 1.0f );
	SetCollidable( false );
}

void Explosion::ShowExplosionStage( const int stage )
{
	// Load the next texture stage
	std::wstringstream os; 
	os << L"textures/explosion" << stage << L".png";
	m_RenderItem = sTextureLoader->LoadTexturedRenderItem(os.str(), 1.0f);
	
	m_ExplosionTime = 0.0f;
	SetScale( GetScale() * 1.10f );
}

void Explosion::Update( World * world, float dt )
{
	m_ExplosionTime += dt;
	if(m_ExplosionTime > sExplosionLength)
	{
		m_CurrentStage++;
		if(m_CurrentStage < sStageCount)
		{
			ShowExplosionStage(m_CurrentStage);
		}
		else
		{
			SetActive( false );
		}
	}

}

void Explosion::OnCollision( Entity& other, World * world )
{
	// Explosions aren't affected by other entities
}