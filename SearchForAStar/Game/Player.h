//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Player gets input through Message function and updates player state base on it.
// 
// Add a summary of your changes here:
// 
// 
// 

#pragma once

#include "Entity.h"
#include <list>
#include "Audio/Sound.h"

// Forward declare
namespace Engine { 
	class Input;
	class SoundProvider;
} 



namespace SFAS
{

namespace Game                  
{
	class Bullet;
	class World;
 
class Player : public SFAS::Game::Entity
{
public:

	Player( int lives );
	virtual ~Player(void);

	// Rendering, Updating and Messaging
	void Player::Update( World world, float dt );

	// Scoring
	void ResetScore() { m_Score = 0; }
	void AddScore( int score ) { m_Score += ( score * m_Multiplier++); }
	int GetScore() const { return m_Score; }
	int GetBestScore() const { return m_Best; }
	int GetMultiplier() const { return m_Multiplier; }

	void DoInput(World * world, const Engine::Input * input);

	// Lives
	int GetLivesRemaining() const { return m_Lives; }
	void ResetLives( int lives ) { m_Lives = lives; }


	void OnReset();
	virtual void Update( World * world, float dt );

	bool IsPlayerControlled() const { return true; }

	static const Entity::EntityType kEntityType;

	// Get the entity type for this entity
	const Entity::EntityType GetEntityType() { return kEntityType; }
private:
	// Override called when a collision is dectected
	void OnCollision( Entity& other, World * world );
	// Override the sound loading
	void LoadSounds(Engine::SoundProvider* soundProvider);
	WCHAR * ToString()  const { return L"Player"; }
	Bullet * Fire( float vx, float vy );
	
	Engine::Sound* m_BulletSound;
	Engine::Sound* m_ExplosionSound;

	inline bool Player::CanFire() const;

	enum { keScore = 100, kNumBullets = 50 };

	static const float sMoveForce;
	static const float sSize;
	static const float sMass;
	static const float sDamping;
	static const float sFireDelay;

	// Player Data
	Bullet * m_Bullets[kNumBullets];	// List of bullets for the player to shoot
	int m_Lives;						// The number of lives this place has remaining
	int m_Score;						// The current score
	int m_Multiplier;					// The current multiplier
	int m_Best;							// This players personal best score
	float m_TimeSinceLastFire;			// Remembers the time of the last fire
};

}

}
