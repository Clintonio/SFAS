//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The state when the player is in the options menu
// 

#include "CreditsState.h"
#include "../../Core/Input.h"
#include "StartState.h"

using namespace SFAS::States;

CreditsState::CreditsState( LPDIRECT3DDEVICE9 p_dx_Device ) :
	GameStateBase( p_dx_Device )
{

	AddText(0, L"Credits",
		0.5f, 0.2f, FontSize::Large, D3DXCOLOR( 1, 1, 1, 1 )
	);
	
	AddText(0, L"Game Development",
		0.5f, 0.3f, FontSize::Medium, D3DXCOLOR( 1, 1, 1, 1 )
	);
	AddText(0, L"Clinton Alexander",
		0.5f, 0.37f, FontSize::Medium, D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1 )
	);
	
	AddText(0, L"Sound",
		0.5f, 0.47f, FontSize::Medium, D3DXCOLOR( 1, 1, 1, 1 )
	);
	AddText(0, L"freesound.org",
		0.5f, 0.54f, FontSize::Medium, D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1 )
	);

	AddText(1, L"Exit",
		0.5f, 0.67f, FontSize::Medium, D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1 )
	);
}

CreditsState::~CreditsState(void)
{
}

int CreditsState::Update( Engine::Input * input, float dt)
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

void CreditsState::OnEnteringState()
{
	
}

