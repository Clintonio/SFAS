//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The main game state that controls what happens when the player is in game
// 
// Add a summary of your changes here:
// 
// 
// 
#pragma once

#include <d3d9.h>
#include "GameStateBase.h"
#include "SearchForAStar/Game/World.h"
#include "SearchForAStar/Game/Player.h"

namespace SFAS
{

namespace States                  
{    

class MainGameState : public GameStateBase
{
public:
	MainGameState( LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h );
	~MainGameState(void);

	// Overrides
	void RenderOverlay( Engine::TextRenderer* txt );
	void Render( float dt );
	bool Update( const Engine::Input * input, float dt );
	void OnEnteringState();

	// Test whether the current game is over
	bool IsGameOver() const { return m_World.IsGameOver(); }

	// Return the world
	const Game::World* GetWorld() const { return &m_World; }

private:

	enum State
	{
		keNewLevel,
		keGamePlay,
		keLevelComplete,
		keGameComplete,

		keNumStates
	};

	enum { keMessageDisplayTime = 1 };

	State		m_GameState;
	Game::World	m_World;
	float		m_TimeSinceStateChange;
};
}
}