//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The state when the player is in the options menu
// 

#include "HighScoresState.h"
#include "../../Core/Input.h"
#include "StartState.h"
#include <sstream>

using namespace SFAS::States;

HighScoresState::HighScoresState( LPDIRECT3DDEVICE9 p_dx_Device ) :
	GameStateBase( p_dx_Device )
{
	// Loads the high scores
	LoadHighScores( "scores.json" );

	AddText(0, L"High Scores",
		0.5f, 0.1f, FontSize::Large, D3DXCOLOR( 1, 1, 1, 1 )
	);
	
	D3DXCOLOR grey(0.7f, 0.7f, 0.7f, 1);
	float yPos = 0.17f;
	for( int i = 0; i < kNumHighScores; i++ )
	{
		std::wstringstream buf;
		buf << (i + 1);
		buf << ": ";
		buf << m_HighestScores[i].score;
		AddText(0, buf.str(), 0.5f, yPos, FontSize::Small, grey);
		yPos += 0.025f;
	}

	AddText(1, L"Exit",
		0.5f, 0.70f, FontSize::Medium, D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1 )
	);
}

HighScoresState::~HighScoresState(void)
{
}

int HighScoresState::Update( Engine::Input * input, float dt)
{
	m_PlayerMousePosition = input->GetMousePosition();

	if( input->HasUserClicked( Engine::Input::Button::MouseButton1 ) )
	{
		const Text * t = GetTextAt( m_PlayerMousePosition );
		if( t != NULL )
		{
			if( t->id == 1 )
			{
				return StartState::kStateID;
			}
		}
	}
	return -1;
}

void HighScoresState::OnEnteringState()
{
	
}

