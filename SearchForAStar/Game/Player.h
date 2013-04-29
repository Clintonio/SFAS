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

// Forward declare
namespace Engine { class Input; } 

namespace SFAS
{

namespace Game                  
{
	class Bullet;
 
class Player : public SFAS::Game::Entity
{
public:

	Player( int id, int lives );
	virtual ~Player(void);

	// Rendering, Updating and Messaging
	void Render( Engine::RenderItem* drw );
	Bullet * Update( const Engine::Input * input, float dt );

	// Scoring
	void ResetScore() { m_Score = 0; }
	void AddScore( int score ) { m_Score += ( score * m_Multiplier++); }
	int GetScore() const { return m_Score; }
	int GetBestScore() const { return m_Best; }
	int GetMultiplier() const { return m_Multiplier; }

	// Lives
	int GetLivesRemaining() const { return m_Lives; }
	void ResetLives( int lives ) { m_Lives = lives; }

	// Override called when a collision is dectected
	void OnCollision( Entity& other );

	void OnReset();

	bool IsPlayerControlled() const { return true; }

private:

	WCHAR * ToString()  const { return L"Player"; }

	Bullet * Fire( float vx, float vy );

	enum { keScore = 100, kNumBullets = 50 };

	static const float sMoveForce;
	static const float sSize;
	static const float sMass;
	static const float sDamping;

	// Player Data
	Bullet * m_Bullets[kNumBullets];	// List of bullets for the player to shoot
	int m_Lives;						// The number of lives this place has remaining
	int m_Score;						// The current score
	int m_Multiplier;					// The current multiplier
	int m_Best;							// This players personal best score
};

}

}
