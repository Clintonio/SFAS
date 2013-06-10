//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Summary state that displays the results of the last game. 
// 
// Add a summary of your changes here:
// 
// 
// 

#include "SummaryState.h"
#include "Core/Input.h"

using SFAS::States::SummaryState;

SummaryState::SummaryState( LPDIRECT3DDEVICE9 p_dx_Device ) :
	GameStateBase( p_dx_Device )
{
	//SetTitleText( L"GAME OVER" );
	//SetInstructionText( L"Click Or Press Any Key To Continue..." );
}

SummaryState::~SummaryState(void)
{
}

void SummaryState::Setup( int score, int best ) 
{ 
	// Update on screen text
	WCHAR strBuffer[512];
	swprintf_s( strBuffer, 512, L"This Score: %d    Best Score: %d", score, best );
	//SetPageText( strBuffer );
}

int SummaryState::Update( Engine::Input * input, float dt)
{
	//return input->JustPressed( Engine::Input::kContinue ) || input->HasUserClicked( Engine::Input::Button::MouseButton1);
	return -1;
}