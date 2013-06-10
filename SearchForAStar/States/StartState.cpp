//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Starting state that the player always sees before gameplay starts.  
// 
// Add a summary of your changes here:
// - Made it possible to click to go to the next screen
// 
// 

#include "StartState.h"
#include "Core/Input.h"
#include "MainGameState.h"
#include "ControlsState.h"
#include "CreditsState.h"
#include "HighScoresState.h"

using SFAS::States::StartState;

StartState::StartState( LPDIRECT3DDEVICE9 p_dx_Device ) :
	GameStateBase( p_dx_Device )
{
	// The crosshair for the player
	m_Cursor.Init( L"Textures/crosshair.png" );

	float x = 0.5f;
	float y = 0.15f;
	AddText(0, L"Search For A Star!", 
		x, y + 0.00f, FontSize::Large, D3DXCOLOR( 1, 1, 0.4f, 1 ) );
	AddText(1, L"Start Story Mode", 
		x, y + 0.15f, FontSize::Medium, D3DXCOLOR( 1, 1, 1, 1 ) );
	AddText(2, L"Start Survival Mode", 
		x, y + 0.22f, FontSize::Medium, D3DXCOLOR( 1, 1, 1, 1 ) );
	AddText(3, L"Credits", 
		x, y + 0.32f, FontSize::Medium, D3DXCOLOR( 1, 1, 1, 1 ) );
	AddText(6, L"High Scores", 
		x, y + 0.39f, FontSize::Medium, D3DXCOLOR( 1, 1, 1, 1 ) );
	AddText(4, L"Controls/ How To Play", 
		x, y + 0.46f, FontSize::Medium, D3DXCOLOR( 1, 1, 1, 1 ) );
	AddText(5, L"Exit", 
		x, y + 0.65f, FontSize::Medium, D3DXCOLOR( 1, 1, 1, 1 ) );
}

StartState::~StartState(void)
{
}

void StartState::Render(float dt)
{
	m_Cursor.Draw(
		D3DXVECTOR3(m_PlayerMousePosition.x, m_PlayerMousePosition.y, 0),
		D3DXVECTOR3(50, 50, 0)
	);
}

int StartState::Update( Engine::Input * input, float dt)
{
	m_PlayerMousePosition = input->GetMousePosition();

	if( input->HasUserClicked( Engine::Input::Button::MouseButton1 ) )
	{
		const Text * t = GetTextAt( m_PlayerMousePosition );
		if( t != NULL )
		{
			if( t->id == 5 )
			{
				return -2; // Exit state
			}
			else if( t->id == 1 )
			{
				return MainGameState::GetStateID();
			}
			else if( t->id == 3 )
			{
				return CreditsState::kStateID;
			}
			else if( t->id == 4 )
			{
				return ControlsState::kStateID;
			}
			else if( t->id == 2 )
			{

			}
			else if( t->id == 6 )
			{
				return HighScoresState::kStateID;
			}
		}
	}
	return -1;
}
