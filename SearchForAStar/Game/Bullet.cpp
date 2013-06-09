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
#include "WeaponType.h"

using namespace SFAS::Game;

const float Bullet::sMass = 0.5f;
const float Bullet::sDamping = 0.9999f;

std::map<std::string, Engine::Sound*> Bullet::sBulletSound;
std::map<std::string, Engine::RenderItem*> Bullet::sRenderItem;

const Entity::EntityType Bullet::kEntityType(20);

Bullet::Bullet( const WeaponType* weaponType ) : 
	Entity( D3DXVECTOR3(), D3DXVECTOR3( 1, 1, 0.0f ), sDamping ), 
	m_SoundPlayed(false)
{
	SetMass( sMass );
	SetType( weaponType );
}

void Bullet::LoadSounds(Engine::SoundProvider* soundProvider)
{
	if( sBulletSound[m_WeaponType->name] == NULL )
	{
		Engine::Sound* bulletSound = soundProvider->CreateSoundBufferFromFile( m_WeaponType->soundFile );
		if( bulletSound != 0 )
		{
			sBulletSound[m_WeaponType->name] = bulletSound;
		}
	}
}

void Bullet::SetType( const WeaponType * type )
{
	m_WeaponType = type;
	if( m_WeaponType != NULL ) 
	{
		SetScale( m_WeaponType->dimensions );
		if( sRenderItem[m_WeaponType->name] == NULL )
		{
			sRenderItem[m_WeaponType->name] = sTextureLoader->LoadTexturedRenderItem(m_WeaponType->textureFile, 1.0f);
		}

		m_RenderItem = sRenderItem[m_WeaponType->name];
	}
}

void Bullet::SetTarget( const ShipEntity * other )
{
	m_TargetShip = other;
}

void Bullet::Update( World * world, float dt )
{
	if( !m_SoundPlayed && ( sBulletSound[m_WeaponType->name] != NULL ) ) 
	{
		sBulletSound[m_WeaponType->name]->PlaySoundFromStart();
		m_SoundPlayed = true;
	}
	
	if( m_WeaponType->weaponAIType == "target" )
	{
		D3DXVECTOR3 targetDir = DirectionToEntity( m_TargetShip );
		FaceDirection( targetDir );
		SetVelocity( targetDir * m_WeaponType->speed );
		SetForce( D3DXVECTOR3( 0, 0, 0 ) );
	}

	if( GetTimeSpentActive() >= m_WeaponType->lifetime )
	{
		SetActive( false );
	}

	Entity::Update( world, dt );
}

void Bullet::Fire( D3DXVECTOR3 direction, ShipEntity * owner )
{
	OnReset();
	m_SoundPlayed = false;
	SetPosition( owner->GetPosition() + direction * max(owner->GetScale().x, owner->GetScale().y) );
	AddForce( direction * m_WeaponType->speed );
	SetActive( true );
	m_Owner = owner;
}

bool Bullet::OnCollision( Entity& other, World * world )
{
	if( m_Owner->OnBulletHit(&other) ) 
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
