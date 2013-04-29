//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Player gets input through Message function and updates player state based on it.
// 
// Add a summary of your changes here:
// 
// 
// 

#include "Player.h"
#include "Bullet.h"
#include "float.h"
#include "Core/Input.h"

using Engine::Input;
using SFAS::Game::Player;
using SFAS::Game::Bullet;

const float Player::sMoveForce = 8000.0f;
const float Player::sSize = 10.0f;
const float Player::sMass = 100.0f;
const float Player::sDamping = 0.39f;

Player::Player( int id, int lives ) : Entity( id, D3DXVECTOR3(), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping ), m_Lives( lives ), m_Score( 0 ), m_Multiplier( 1 ), m_Best( 0 )
{
	SetMass( sMass );

	for( int count = 0; count < kNumBullets; count++ )
	{
		m_Bullets[count] = new Bullet( count );
	}
}

Player::~Player(void)
{
	for( int count = 0; count < kNumBullets; count++ )
	{
		delete m_Bullets[count];
		m_Bullets[count] = 0;
	}
}

void Player::Render( Engine::RenderItem* drw )
{
	Entity::Render(drw);
}

Bullet * Player::Update( const Engine::Input * input, float dt )
{
	// Check for personal best
	if( m_Score > m_Best )
	{
		m_Best = m_Score;
	}

	D3DXVECTOR3 deltaForce( 0.0f, 0.0f, 0.0f );

	if( input->Held( Input::kUp ) )
	{
		deltaForce.y += sMoveForce;
	}
	if( input->Held( Input::kDown ) )
	{
		deltaForce.y += -sMoveForce;
	}
	if( input->Held( Input::kLeft ) )
	{
		deltaForce.x = -sMoveForce;
	}
	if( input->Held( Input::kRight ) )
	{
		deltaForce.x = sMoveForce;
	}

	AddForce(deltaForce);

	Bullet * bullet = 0;
	if( input->PressedWithRepeat( Input::kFireUp ) )
	{
		bullet = Fire( 0.0f, 1.0f );
	}
	else if( input->PressedWithRepeat( Input::kFireLeft ) )
	{
		bullet = Fire( -1.0f, 0.0f );
	}
	else if( input->PressedWithRepeat( Input::kFireRight ) )
	{
		bullet = Fire( 1.0f, 0.0f );
	}
	else if( input->PressedWithRepeat( Input::kFireDown ) )
	{
		bullet = Fire( 0.0f, -1.0f );
	}

	Entity::Update(dt);

	return bullet;
}

void Player::OnCollision( Entity& other )
{
	if( other.IsMoveable() && !other.IsPlayerControlled() )
	{
		// Lost a life
		m_Lives--;

		// Lose multiplier
		m_Multiplier = 1;

		// Default behavour is to die
		SetActive( false );
	}
}

void Player::OnReset()
{
}

Bullet * Player::Fire( float vx, float vy )
{
	Bullet * bullet = 0;

	for( int count = 0; count < kNumBullets; count++ )
	{
		if( !m_Bullets[count]->IsActive() )
		{
			bullet = m_Bullets[count];
			break;
		}
	}

	if( bullet != 0 )
	{
		bullet->SetPosition( GetPosition() );
		bullet->Fire( vx, vy );
	}

	return bullet;
}

