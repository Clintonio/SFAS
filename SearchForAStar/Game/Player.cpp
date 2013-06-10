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
// - Made player face direction of mouse
// - Made player explode on death
// - Removed player keyboard controls

#include "Player.h"
#include "Bullet.h"
#include "float.h"
#include "World.h"
#include "Core/Input.h"
#include "Entity.h"
#include "Explosion.h"
#include "Audio/SoundProvider.h"
#include "Enemy.h"
#include "Level.h"

using Engine::Input;
using namespace SFAS::Game;

const float Player::sMoveForce	= 8000.0f;
const float Player::sSize		= 20.0f;
const float Player::sMass		= 100.0f;
const float Player::sDamping	= 0.39f;

const Entity::EntityType Player::kEntityType(1);

Player::Player( ) : 
	ShipEntity( D3DXVECTOR3(), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping ), 
	m_Score( 0 ), 
	m_Multiplier( 1 ), 
	m_Best( 0 ), 
	m_TimeSinceLastFire( 0 ),
	m_Lives( 3 )
{
	SetMass( sMass );

	// Initialise sounds so that they're not played incorrectly
	m_ExplosionSound = 0;
}

Player::~Player(void)
{
	if( m_ExplosionSound != 0 )
	{
		delete m_ExplosionSound;
	}
}

void Player::Spawn()
{
	SetPosition( m_SpawnPosition );
	SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	SetActive( true );
}

void Player::UpdateWithDescriptor( const Level::Player & player )
{
	m_WeaponType = player.weapon;
	m_RenderItem = sTextureLoader->LoadTexturedRenderItem(player.textureFile, 1.0f);
}

void Player::DoInput(World * world, const Engine::Input * input )
{
	Bullet * bullet = 0;
	const D3DXVECTOR2 mouseCoords = input->GetMousePosition();
	const D3DXVECTOR2 playerCoords = GetPosition();
	D3DXVECTOR2 direction;

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

	direction = (mouseCoords - playerCoords);
	D3DXVec2Normalize( &direction, &direction);
	FaceDirection(direction);
	
	if( input->IsButtonHeld( Input::Button::MouseButton1 ) ){
		bullet = Fire( direction.x, direction.y );
		if( bullet != NULL )
		{
			// CanFire is an expensive operation, so it's faster to load a bullet
			// first before checking this
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

}

void Player::LoadSounds(Engine::SoundProvider* soundProvider)
{
	Engine::Sound* explosionSound = soundProvider->CreateSoundBufferFromFile("Sound/player-explosion.wav");

	if( explosionSound != 0 )
	{
		m_ExplosionSound = explosionSound;
		m_ExplosionSound->SetVolume(0.7f);
	}
}

void Player::Update( World * world, float dt ) 
{
	m_TimeSinceLastFire += dt;

	Entity::Update( world, dt );
}

inline bool Player::CanFire() const
{
	return ( m_WeaponType.fireDelay < m_TimeSinceLastFire );
}

bool Player::OnCollision( Entity& other, World * world )
{
	if( other.IsMoveable() || !other.IsPlayerControlled() )
	{
		// Lost a life
		m_Lives--;

		// Lose multiplier
		m_Multiplier = 1;

		// Default behavour is to die
		SetActive( false );

		Explosion* e = new Explosion( GetPosition(), GetScale() );
		world->AddEntity( e );

		if( m_ExplosionSound != 0 )
		{
			m_ExplosionSound->PlaySoundFromStart();
		}
	}

	return true;
}

void Player::OnReset()
{
	
}

Bullet * Player::Fire( float vx, float vy )
{
	Bullet * bullet = new Bullet( &m_WeaponType );
	bullet->Fire( D3DXVECTOR3(vx, vy, 0.0), this );

	return bullet;
}


bool Player::OnBulletHit( Entity * other ) {
	if( Enemy::kEntityType == other->GetEntityType() )
	{
		AddScore(((Enemy*) other)->GetKillScore());
	}

	return true;
}