//
// Source files for Search For A Star competition
// Copyright (c) 2010 L. Attwood
//
// Enemy class that patrols, hurts the player.
// 
// Add a summary of your changes here:
// - Made enemy face player
// - Made enemy explode on death
// 

#include "Enemy.h"
#include "Player.h"
#include "World.h"
#include "Explosion.h"
#include "Audio/SoundProvider.h"
#include "Audio/Sound.h"
#include "Bullet.h"

using namespace SFAS::Game;

const float Enemy::sSize = 20.0f;
const float Enemy::sSpeed = 10000.0f;
const float Enemy::sMass = 100.0f;
const float Enemy::sDamping = 0.6f;

const Entity::EntityType Enemy::kEntityType(2);
Engine::Sound* Enemy::sExplosionSound = 0;

Enemy::Enemy( Level::EnemyType & type, float x, float y ) : 
	ShipEntity( D3DXVECTOR3( x, y, 0.0f ), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping ),
	m_LastFireTime(0.0f)
{
	SetScale( type.dimensions );
	SetMass( sMass );
	SetActive(true);
	SetWeapon( type.weapon );
	SetAIRoutine( type.aiType );
	m_Health = type.health;
	m_RenderItem = sTextureLoader->LoadTexturedRenderItem(type.textureFile, 1.0f);
}

Enemy::~Enemy(void)
{
}

void Enemy::LoadSounds(Engine::SoundProvider* soundProvider)
{
	if( sExplosionSound == 0 )
	{
		Engine::Sound* explosionSound = soundProvider->CreateSoundBufferFromFile("Sound/enemy-explosion.wav");
		if( explosionSound != 0 )
		{
			sExplosionSound = explosionSound;
		}
	}
}

void Enemy::Update( World * world, float dt )
{
	Player * player = (Player*) world->FindNearestEntityOfType( this, Player::kEntityType );
	D3DXVECTOR3 directionToPlayer = DirectionToEntity( player );

	if( m_AIRoutine == AIRoutine::avoid )
	{
		float playerDistance = DistanceTo(player);
		if( playerDistance < 200.0f )
		{
			AddForce( sSpeed * -directionToPlayer);
		} 
		else if( playerDistance > 210.0f )
		{
			AddForce( sSpeed * directionToPlayer);
		}
	} 
	else
	{
		if(player != NULL)
		{
			AddForce( sSpeed * directionToPlayer);
		}
	}

	m_LastFireTime += dt;
	if( m_WeaponType == Weapon::laser && ( m_LastFireTime > 1.0f ) )
	{
		Bullet * bullet = new Bullet();
		bullet->Fire(directionToPlayer, this);
		world->AddEntity(bullet);
		m_LastFireTime = 0.0f;
	}

	FaceDirection( DirectionToEntity ( player ) );

	Entity::Update( world, dt );
}

void Enemy::SetAIRoutine( std::string & type )
{
	if( type == "avoid" )
	{
		m_AIRoutine = AIRoutine::avoid;
	} 
	else 
	{
		m_AIRoutine = AIRoutine::suicide;
	}
}

void Enemy::SetWeapon( std::string & type )
{
	if( type == "laser" )
	{
		m_WeaponType = Weapon::laser;
	} 
	else
	{
		m_WeaponType = Weapon::none;
	}
}

bool Enemy::OnCollision( Entity& other, World * world )
{
	if( other.IsPlayerControlled() )
	{
		m_Health--;
		if( m_Health <= 0 ) 
		{
			// Might be the player might be a player bullet - either way, die.
			SetActive( false );

			Explosion* explosion = new Explosion(GetPosition(), GetScale() );
			world->AddEntity( explosion );

			if( sExplosionSound != 0 )
			{
				sExplosionSound->PlaySoundFromStart();
			}
		}
	}

	return true;
}

bool Enemy::OnBulletHit( Entity &other ) 
{
	return !( other.GetEntityType() == Enemy::kEntityType );
}