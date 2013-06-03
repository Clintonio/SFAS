//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The state when the player is in the options menu
// 
// 
#pragma once

#include <d3d9.h>
#include "GameStateBase.h"

namespace SFAS
{

namespace States                  
{    

class OptionsState : public GameStateBase
{
public:
	OptionsState( LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h );
	~OptionsState(void);

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

};
}
}