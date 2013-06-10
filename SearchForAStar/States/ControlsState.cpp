#include "ControlsState.h"
#include "../../Core/Input.h"
#include "StartState.h"

using namespace SFAS::States;

ControlsState::ControlsState( LPDIRECT3DDEVICE9 p_dx_Device ) :
	GameStateBase( p_dx_Device )
{
	float left = 0.2f;
	float centre = 0.5f;
	float right = 0.8f;
	
	D3DXCOLOR black = D3DXCOLOR( 1, 1, 1, 1 );
	D3DXCOLOR grey = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1 );
	
	float yPos = 0.15f;
	AddText(0, L"Controls and How To Play", centre, yPos, FontSize::Large, black );
	
	yPos += 0.09f;
	AddText(0, L"Survive by shooting the enemy craft that fly at you. Walls, bullets and enemy crafts will destroy your ship.", centre, yPos, FontSize::Small, black );
	yPos += 0.07f;
	AddText(0, L"Action ", left, yPos, FontSize::Medium, black );

	AddText(0, L"Keyboard", centre, yPos, FontSize::Medium, black );
	
	AddText(0, L"360 Controller", right, yPos, FontSize::Medium, black );

	yPos += 0.07f;
	AddText(0, L"Move", left, yPos, FontSize::Small, grey );
	AddText(0, L"W/A/S/D", centre, yPos, FontSize::Small, grey );
	AddText(0, L"Left Thumbstick", right, yPos, FontSize::Small, grey );
	
	yPos += 0.04f;
	AddText(0, L"Aim", left, yPos, FontSize::Small, grey );
	AddText(0, L"Mouse", centre, yPos, FontSize::Small, grey );
	AddText(0, L"Right Thumbstick", right, yPos, FontSize::Small, grey );
	
	yPos += 0.04f;
	AddText(0, L"Fire/Select", left, yPos, FontSize::Small, grey );
	AddText(0, L"Left Mouse", centre, yPos, FontSize::Small, grey );
	AddText(0, L"Right Trigger/A", right, yPos, FontSize::Small, grey );
	
	yPos += 0.04f;
	AddText(0, L"Exit", left, yPos, FontSize::Small, grey );
	AddText(0, L"Esc", centre, yPos, FontSize::Small, grey );
	AddText(0, L"B", right, yPos, FontSize::Small, grey );

	yPos += 0.04f;
	AddText(0, L"Full Screen", left, yPos, FontSize::Small, grey );
	AddText(0, L"F11", centre, yPos, FontSize::Small, grey );
	AddText(0, L"Back", right, yPos, FontSize::Small, grey );

	yPos += 0.04f;
	AddText(0, L"Pause", left, yPos, FontSize::Small, grey );
	AddText(0, L"P", centre, yPos, FontSize::Small, grey );
	AddText(0, L"Start", right, yPos, FontSize::Small, grey );

	AddText(1, L"Exit", centre, 0.67f, FontSize::Medium, grey );
}

ControlsState::~ControlsState(void)
{
}

int ControlsState::Update( Engine::Input * input, float dt)
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

void ControlsState::OnEnteringState()
{
	
}

