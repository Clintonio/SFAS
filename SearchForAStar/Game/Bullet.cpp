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
#include "Audio/SoundProvider.h"

using namespace SFAS::Game;

const float Bullet::sSize = 2.0f;
const float Bullet::sMass = 0.5f;
const float Bullet::sDamping = 0.9999f;

Engine::Sound* Bullet::sBulletSound = 0;

const Entity::EntityType Bullet::kEntityType(20);

Bullet::Bullet( ) : Entity( D3DXVECTOR3(), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping ), m_SoundPlayed(false)
{
	SetMass( sMass );
	m_RenderItem = sTextureLoader->LoadTexturedRenderItem(L"textures/bullet.png", 1.0f);
}

void Bullet::LoadSounds(Engine::SoundProvider* soundProvider)
{
	if( sBulletSound == 0 )
	{
		Engine::Sound* bulletSound = soundProvider->CreateSoundBufferFromFile("Sound/laser1.wav");
		if( bulletSound != 0 )
		{
			sBulletSound = bulletSound;
		}
	}
}


void Bullet::Update( World * world, float dt )
{
	Entity::Update( world, dt );
	
	if( !m_SoundPlayed && ( sBulletSound != 0 ) ) 
	{
		sBulletSound->PlaySoundFromStart();
		m_SoundPlayed = true;
	}

	if( GetTimeSpentActive() >= kLifetime )
	{
		SetActive( false );
	}
}

void Bullet::Fire( D3DXVECTOR3 direction, ShipEntity * owner )
{
	OnReset();
	m_SoundPlayed = false;
	SetPosition( owner->GetPosition() + direction * max(owner->GetScale().x, owner->GetScale().y) );
	AddForce( direction * kForce );
	SetActive( true );
	m_Owner = owner;
}

bool Bullet::OnCollision( Entity& other, World * world )
{
	if( m_Owner->OnBulletHit(other) ) 
	{
		SetActive( false );
		return true;
	} 
	else
	{
		return false;
	}
}

bool Bullet::IsPlayerControlled() const
{ 
	return ( m_Owner->GetEntityType() == Player::kEntityType );
}
