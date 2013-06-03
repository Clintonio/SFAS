//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The state when the player is in the options menu
// 

#include "OptionsState.h"

using SFAS::States::OptionsState;

OptionsState::OptionsState( LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h ) : m_GameState( keNewLevel ), m_World(p_dx_Device, han_Window, w, h ), m_TimeSinceStateChange( 0 )
{
	
}

OptionsState::~OptionsState(void)
{
}

void OptionsState::RenderOverlay( Engine::TextRenderer* txt )
{
	
}

void OptionsState::Render(float dt)
{
	
}

bool OptionsState::Update( const Engine::Input * input, float dt)
{
	
}

void OptionsState::OnEnteringState()
{
	
}

