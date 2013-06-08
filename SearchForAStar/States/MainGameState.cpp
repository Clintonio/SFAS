//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The main game state that controls what happens when the player is in game
// 
// Add a summary of your changes here:
// - Added custom mouse cursor
// 
// 

#include "MainGameState.h"
#include "Core/Input.h"

using SFAS::States::MainGameState;

MainGameState::MainGameState( LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h ) : 
	m_GameState( keNewLevel ), 
	m_World(p_dx_Device, han_Window, w, h ), 
	m_TimeSinceStateChange( 0 ),
	m_Cursor( p_dx_Device )
{
	SetTitleText( L"" );
	SetPageText( L"" );
	SetInstructionText( L"Lives: -      Score: -      Multiplier: -      Best: -" );
	// The crosshair for the player
	m_Cursor.Init( L"Textures/crosshair.png" );
}

MainGameState::~MainGameState(void)
{
}

void MainGameState::RenderOverlay( Engine::TextRenderer* txt )
{
	// Render the normal overlay
	GameStateBase::RenderOverlay( txt );

	// Render the world debug information
	//m_World.RenderDebug( txt );
}

void MainGameState::Render(float dt)
{
	m_World.Render(dt);
	m_Cursor.Draw(
		D3DXVECTOR3(m_PlayerMousePosition.x, m_PlayerMousePosition.y, 0),
		D3DXVECTOR3(50, 50, 0)
	);
}

bool MainGameState::Update( const Engine::Input * input, float dt)
{
	bool gameOver = false;

	// Cache the old state here to check whether or not the 
	// state has been changed by the end of this function
	State old_state = m_GameState;
	WCHAR strBuffer[512];

	// Update the time in this state
	m_TimeSinceStateChange += dt;

	// Update the game world
	m_World.Update(input, 0.03f);//dt

	// Capture the current mouse position
	m_PlayerMousePosition = input->GetMousePosition(Engine::Input::Button::MouseButton1);

	// Switch the state
	switch( m_GameState )
	{
		case keNewLevel:
			if( m_TimeSinceStateChange > keMessageDisplayTime )
			{
				m_World.NextLevel();
				SetPageText( L"" );
				m_GameState = keGamePlay;
			}
			break;
		case keGamePlay:
			// If the level is over then go to level complete
			if( m_World.IsLevelFinished() )
			{
				if( m_World.GetCurrentLevel() > 0 )
				{
					SetPageText( L"Level Complete!" );
				}
				
				m_GameState = keNewLevel;
			}
			else if( m_World.IsGameOver() )
			{
				m_World.ClearLevel();
				gameOver = true;
			}
			break;
	}

	// If the state has changed then reset the timer
	if( old_state != m_GameState )
	{
		m_TimeSinceStateChange = 0.0f;
	}
	
	// Update on screen text
	const Game::Player * player = m_World.GetPlayer();
	swprintf_s( strBuffer, 512, L"Lives: %d    Score: %d    Multiplier: %d    Best: %d", player->GetLivesRemaining(), player->GetScore(), player->GetMultiplier(), player->GetBestScore() );
	SetInstructionText( strBuffer );

	return gameOver;
}

void MainGameState::OnEnteringState()
{
	m_TimeSinceStateChange = 0.0f;
	SetPageText( L"Get Ready!" );
	m_World.NewGame();
}

