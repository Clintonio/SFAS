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

#include "Enemy.h"
#include "Player.h"
#include "World.h"

using namespace SFAS::Game;

const float Enemy::sSize = 10.0f;
const float Enemy::sSpeed = 10000.0f;
const float Enemy::sMass = 100.0f;
const float Enemy::sDamping = 0.6f;

Engine::RenderItem * Enemy::sRenderItem = 0;
const Entity::EntityType Enemy::kEntityType(2);

Enemy::Enemy( float x, float y ) : Entity( D3DXVECTOR3( x, y, 0.0f ), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping )
{
	SetMass( sMass );
	SetActive(true);
	m_RenderItem = sTextureLoader->LoadTexturedRenderItem(L"textures/enemy.png", 1.0f);
}

Enemy::~Enemy(void)
{
}

void Enemy::Update( World * world, float dt )
{
	if( IsActive() )
	{
		switch( rand() % 4 )
		{
		case 0:
			AddForce( D3DXVECTOR3( sSpeed, 0.0f, 0.0f ) );
			break;
		case 1:
			AddForce( D3DXVECTOR3( 0.0f, sSpeed, 0.0f ) );
			break;
		case 2:
			AddForce( D3DXVECTOR3( -sSpeed, 0.0f, 0.0f ) );
			break;
		case 3:
			AddForce( D3DXVECTOR3( 0.0f, -sSpeed, 0.0f ) );
			break;
		}
		Player * player = (Player*) world->FindNearestEntityOfType( this, Player::kEntityType );
		if(player != NULL)
		{
			AddForce( sSpeed * DirectionToEntity ( player ));
		}
	}


	Entity::Update( world, dt);
}

void Enemy::OnCollision( Entity& other )
{
	if( other.IsPlayerControlled() )
	{
		// Might be the player might be a player bullet - either way, die.
		SetActive( false );
	}
}
