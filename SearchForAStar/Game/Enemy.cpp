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

using namespace SFAS::Game;

const float Enemy::sSize = 20.0f;
const float Enemy::sSpeed = 10000.0f;
const float Enemy::sMass = 100.0f;
const float Enemy::sDamping = 0.6f;

const Entity::EntityType Enemy::kEntityType(2);
Engine::Sound* Enemy::sExplosionSound = 0;

Enemy::Enemy( float x, float y ) : Entity( D3DXVECTOR3( x, y, 0.0f ), D3DXVECTOR3( sSize, sSize, 0.0f ), sDamping )
{
	SetMass( sMass );
	SetActive(true);
	m_RenderItem = sTextureLoader->LoadTexturedRenderItem(L"textures/enemy.png", 1.0f);
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
	D3DXVECTOR2 direction;
	Player * player = (Player*) world->FindNearestEntityOfType( this, Player::kEntityType );

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

	if(player != NULL)
	{
		AddForce( sSpeed * DirectionToEntity ( player ));
	}

	FaceDirection(DirectionToEntity ( player ));

	Entity::Update( world, dt);
}

void Enemy::OnCollision( Entity& other, World * world )
{
	if( other.IsPlayerControlled() )
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
