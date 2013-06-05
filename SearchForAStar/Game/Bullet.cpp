//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// An imovable object which can support the player. 
// 
// Add a summary of your changes here:
// - Made bullets deactivate upon collision
// 
// 

#include "Bullet.h"

using SFAS::Game::Bullet;

const float Bullet::sSize = 1.0f;
const float Bullet::sMass = 0.5f;
const float Bullet::sDamping = 0.9999f;

Bullet::Bullet( int id ) : Entity( id, D3DXVECTOR3(), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping )
{
	SetActive( false );
	SetMass( sMass );
}

Bullet::~Bullet( void )
{
}

void Bullet::Update( float dt )
{
	Entity::Update( dt );

	if( GetTimeSpentActive() >= kLifetime )
	{
		SetActive( false );
	}
}

void Bullet::Fire( float vx, float vy )
{	
	AddForce( D3DXVECTOR3( vx * kForce, vy * kForce, 0.0f ) );
	SetActive( true );
}

void Bullet::SetActive( bool active )
{
	// Test comment to test check-ins
	Entity::SetActive( active );
	SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
}

void Bullet::OnCollision( Entity& other )
{
	SetActive( false );
}
