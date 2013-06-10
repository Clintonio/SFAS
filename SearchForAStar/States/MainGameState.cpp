//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The main game state that controls what happens when the player is in game
// 
// Add a summary of your changes here:
// - Added custom mouse cursor
// - Added loading of high scores
// 

#include "MainGameState.h"
#include "Core/Input.h"
#include "SummaryState.h"
#include <sstream>
#include <fstream>
#include <string>

using namespace SFAS::States;

const int kStateID = 1;

MainGameState::MainGameState( LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h ) : 
	GameStateBase( p_dx_Device ),
	m_GameState( keNewLevel ), 
	m_World(p_dx_Device, han_Window, w, h ),
	m_GamePaused( false )
{
	// The crosshair for the player
	m_Cursor.Init( L"Textures/crosshair.png" );
	// Text at the bottom of the screen
	AddText( 1, L"", 0.5f, 0.85f, FontSize::Large, D3DXCOLOR(1,1,1,0.5f));
	// Text in the middle of the screen
	AddText( 2, L"", 0.5f, 0.45f, FontSize::Large, D3DXCOLOR(1,1,1,1));
	// Level intro text
	AddText( 3, L"", 0.5f, 0.5f, FontSize::Small, D3DXCOLOR(1,1,1,1));
	// Level Title text
	AddText( 4, L"", 0.5f, 0.2f, FontSize::Large, D3DXCOLOR(1,1,0.7f,1));
	
	// Loads the high scores
	LoadHighScores( "scores.json" );
}

MainGameState::~MainGameState(void)
{
}

void MainGameState::Render(float dt)
{
	m_World.Render(dt);

	if( m_DrawCursor )
	{
		m_Cursor.Draw(
			D3DXVECTOR3(m_PlayerMousePosition.x, m_PlayerMousePosition.y, 0),
			D3DXVECTOR3(50, 50, 0)
		);
	}
}

int MainGameState::Update( Engine::Input * input, float dt)
{
	bool gameOver = false;

	if( input->JustPressed( Engine::Input::Key::kPause ) )
	{
		m_GamePaused = !m_GamePaused;
		if( m_GamePaused )
		{
			UpdateText( 2, L"Game Paused" );
		} 
		else
		{
			UpdateText( 2, L"" );
		}
	}

	if( !m_GamePaused )
	{
		WCHAR strBuffer[512];

		if( m_GameState != keIntroText )
		{
			// Update the game world
			m_World.Update(input, 0.03f);//dt
		}

		// Capture the current mouse position
		m_PlayerMousePosition = input->GetMousePosition();

		// Switch the state
		switch( m_GameState )
		{
			case keNewLevel:
				input->SetSensitivity( 1.0f );
				m_GameState = keIntroText;
				m_World.NextLevel();
				input->Enable();
				m_DrawCursor = true;
				break;
			case keIntroText:
				if( input->HasUserClicked( Engine::Input::Button::MouseButton1) )
				{
					m_GameState = keGamePlay;
					UpdateText( 3, L"" );
					UpdateText( 4, L"" );
				}
				else
				{
					UpdateText( 1, L"Press Fire To Continue" );
					UpdateText( 3, m_World.GetCurrentLevelDescriptor()->introText );
					UpdateText( 4, m_World.GetCurrentLevelDescriptor()->name );
				}
				break;
			case keGamePlay:
				// If the level is over then go to level complete
				if( m_World.IsLevelFinished() )
				{
					if( m_World.GetCurrentLevel() > 0 )
					{
						//SetPageText( L"Level Complete!" );
					}
					
					input->Disable();
					m_DrawCursor = false;
					m_GameState = keNewLevel;
					m_World.ClearLevel();
				}
				else if( m_World.IsGameOver() )
				{
					input->SetSensitivity( 0.1f );
					UpdateHighScores( m_World.GetPlayer()->GetScore() );
					m_World.ClearLevel();
					gameOver = true;
				}
				break;
		}
		
		if( m_GameState != keIntroText )
		{
			// Update on screen text
			const Game::Player * player = m_World.GetPlayer();
			swprintf_s( strBuffer, 512, L"Lives: %d    Score: %d    Multiplier: %d    Best: %d", 
				player->GetLivesRemaining(), 
				player->GetScore(), 
				player->GetMultiplier(), 
				m_HighestScores[0] 
			);
			UpdateText( 1, strBuffer );
		}
	}

	if( gameOver )
	{
		return SummaryState::kStateID;
	}
	else
	{
		return -1;
	}
}

void MainGameState::OnEnteringState()
{
	m_World.NewGame();
}

void MainGameState::UpdateHighScores( int newScore ) {
	bool updating = false;
	int tmp1 = 0;
	int tmp2 = newScore;
	// This section slides down all lower scores
	for( int i = 0; i < kNumHighScores; i++ )
	{
		if( updating || newScore > m_HighestScores[i].score )
		{
			tmp1 = m_HighestScores[i].score;
			m_HighestScores[i].score = tmp2;
			tmp2 = tmp1;
			updating = true;
		}
	}

	std::ofstream buffer("scores.json");
	buffer << "{ \"scores\" : [\n";

	for( int i = 0; i < kNumHighScores; i++ )
	{
		if( i > 0 )
		{
			buffer << ",";
		}
		buffer << "{\n";
		buffer << "\"score\": ";
		buffer << m_HighestScores[i].score;
		buffer << ",\n \"name\": \"\"";
		buffer << "}\n";
	}

	buffer << "]}";
}