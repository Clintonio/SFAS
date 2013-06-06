//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// An imovable object which can support the player. 
// 
// Add a summary of your changes here:
// - Made bullets deactivate upon collision
// - Modified bullet logic to be more encapsulated
// - Moved scoring logic to Bullet for expansion to multiple players

#include "Bullet.h"
#include "World.h"
#include "Player.h"
#include "Enemy.h" 

using namespace SFAS::Game;

const float Bullet::sSize = 1.0f;
const float Bullet::sMass = 0.5f;
const float Bullet::sDamping = 0.9999f;

Engine::RenderItem * Bullet::sRenderItem = 0;
const Entity::EntityType Bullet::kEntityType(20);

Bullet::Bullet( ) : Entity( D3DXVECTOR3(), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping )
{
	SetMass( sMass );
}

void Bullet::Update( World * world, float dt )
{
	Entity::Update( world, dt );

	if( GetTimeSpentActive() >= kLifetime )
	{
		SetActive( false );
	}
}

void Bullet::Fire( float vx, float vy, Player * owner )
{	
	OnReset();
	AddForce( D3DXVECTOR3( vx * kForce, vy * kForce, 0.0f ) );
	SetActive( true );
	m_Owner = owner;
}

void Bullet::OnCollision( Entity& other )
{
	if( Enemy::kEntityType == other.GetEntityType() )
	{
		m_Owner->AddScore(100);
	}
	SetActive( false );
}
