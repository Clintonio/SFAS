//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Summary state that displays the results of the last game. 
// 
// Add a summary of your changes here:
// - Replaced text with newer text and clickable buttons 
// 
// 

#include "SummaryState.h"
#include "Core/Input.h"
#include "StartState.h"
#include "MainGameState.h"
#include "HighScoresState.h"
#include <string>
#include <sstream>
#include "../Game/Player.h"

using SFAS::States::SummaryState;

SummaryState::SummaryState( LPDIRECT3DDEVICE9 p_dx_Device ) :
	GameStateBase( p_dx_Device )
{
	D3DXCOLOR white(1,1,1,1);
	AddText( 0, L"GAME OVER", 0.5f, 0.3f, FontSize::Large, D3DXCOLOR(1,1,0.7f, 1));
	AddText( 1, L"Try Again", 0.5f, 0.6f, FontSize::Medium, white );
	AddText( 2, L"Main Menu", 0.5f, 0.67f, FontSize::Medium,  white );
	AddText( 4, L"View High Scores", 0.5f, 0.74f, FontSize::Medium,  white );
	AddText( 3, L"", 0.5f, 0.4f, FontSize::Medium, white );
}

SummaryState::~SummaryState(void)
{
}

int SummaryState::Update( Engine::Input * input, float dt)
{
	m_PlayerMousePosition = input->GetMousePosition();

	std::wstringstream buf;
	buf << "Your score was ";
	buf << MainGameState::sLastScore;
	buf << " and the high score is ";
	buf << MainGameState::sHighestScore;
	UpdateText( 3, buf.str() );

	if( input->HasUserClicked( Engine::Input::Button::MouseButton1 ) )
	{
		const Text * t = GetTextAt( m_PlayerMousePosition );
		if( NULL != t )
		{
			if( 2 == t->id )
			{
				return StartState::kStateID;
			}
			if( 1 == t->id )
			{
				return MainGameState::GetStateID();
			}
			if( 4 == t->id )
			{
				return HighScoresState::kStateID;
			}
		}
	}
	return -1;
}