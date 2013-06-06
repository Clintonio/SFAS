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

using SFAS::Game::Enemy;
using SFAS::Game::Entity;

const float Enemy::sSize = 10.0f;
const float Enemy::sSpeed = 10000.0f;
const float Enemy::sMass = 100.0f;
const float Enemy::sDamping = 0.6f;

Enemy::Enemy( int id, float x, float y ) : Entity( id, D3DXVECTOR3( x, y, 0.0f ), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping )
{
	SetMass( sMass );
}

Enemy::~Enemy(void)
{
}

void Enemy::Update( float dt )
{
	if( IsActive() )
	{
		switch( rand() % 5 )
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
		case 4:
			//Player player = FindNearestEntityOfType(Player::kEntityType);
			//AddForce( DirectionToEntity ( player ));
			break;
		}
	}

	Entity::Update(dt);
}

void Enemy::OnCollision( Entity& other )
{
	if( other.IsPlayerControlled() )
	{
		// Might be the player might be a player bullet - either way, die.
		SetActive( false );
	}
}
