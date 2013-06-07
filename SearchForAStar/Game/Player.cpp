//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Player gets input through Message function and updates player state based on it.
// 
// Add a summary of your changes here:
// - Made bullets reset to inactive on player reset
// - Added mouse support for firing bullets
// - Modified bullet logic so that bullet timings are handled internally
// - Made bullets appear in front of player so that they do not collide with player

#include "Player.h"
#include "Bullet.h"
#include "float.h"
#include "World.h"
#include "Core/Input.h"
#include "Entity.h"

using Engine::Input;
using namespace SFAS::Game;

const float Player::sMoveForce	= 8000.0f;
const float Player::sSize		= 20.0f;
const float Player::sMass		= 100.0f;
const float Player::sDamping	= 0.39f;
const float Player::sFireDelay	= 0.1f;

Engine::RenderItem * Player::sRenderItem = 0;
const Entity::EntityType Player::kEntityType(1);

Player::Player( int lives ) : Entity( D3DXVECTOR3(), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping ), 
	m_Lives( lives ), m_Score( 0 ), m_Multiplier( 1 ), m_Best( 0 ), m_TimeSinceLastFire( 0 )
{
	SetMass( sMass );

	for( int count = 0; count < kNumBullets; count++ )
	{
		m_Bullets[count] = new Bullet( );
	}

	m_RenderItem = sTextureLoader->LoadTexturedRenderItem(L"textures/player.png", 1.0f);
}

Player::~Player(void)
{
	for( int count = 0; count < kNumBullets; count++ )
	{
		delete m_Bullets[count];
		m_Bullets[count] = 0;
	}
}

void Player::DoInput(World * world, const Engine::Input * input )
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
	if( input->IsButtonHeld( Input::Button::MouseButton1 ) ){
		const D3DXVECTOR2 mouseCoords = input->GetMousePosition(Input::Button::MouseButton1);
		const D3DXVECTOR2 playerCoords = GetPosition();

		D3DXVECTOR2 direction = (mouseCoords - playerCoords);
		D3DXVec2Normalize( &direction, &direction);

		bullet = Fire( direction.x, direction.y );
	}
	else if( input->Held( Input::kFireUp ) )
	{
		bullet = Fire( 0.0f, 1.0f );
	}
	else if( input->Held( Input::kFireLeft ) )
	{
		bullet = Fire( -1.0f, 0.0f );
	}
	else if( input->Held( Input::kFireRight ) )
	{
		bullet = Fire( 1.0f, 0.0f );
	}
	else if( input->Held( Input::kFireDown ) )
	{
		bullet = Fire( 0.0f, -1.0f );
	}

	if( bullet != NULL )
	{
		if( CanFire() )
		{
			world->AddEntity(bullet);
			m_TimeSinceLastFire = 0;
		} 
		else 
		{
			bullet->SetActive(false);
		}
	}
}

void Player::Update( World * world, float dt ) 
{
	m_TimeSinceLastFire += dt;

	Entity::Update( world, dt );
}

inline bool Player::CanFire() const
{
	return ( sFireDelay < m_TimeSinceLastFire );
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
	for (int count = 0; count < kNumBullets; count++) 
	{
		m_Bullets[count]->SetActive(false);
	}
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
		bullet->SetPosition( GetPosition() + D3DXVECTOR3(vx, vy, 0.0) * sSize );
		bullet->Fire( vx, vy, this );
	}

	return bullet;
}

