//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Starting state that the player always sees before gameplay starts.  
// 
// Add a summary of your changes here:
// 
// 
// 

#include "StartState.h"
#include "Core/Input.h"

using SFAS::States::StartState;

StartState::StartState(void)
{
	SetTitleText( L"SEARCH FOR A STAR!" );
	SetPageText( L"" );
	SetInstructionText( L"Press Any Key To Continue..." );
}

StartState::~StartState(void)
{
}

void StartState::Render(float dt)
{

}

bool StartState::Update( const Engine::Input * input, float dt)
{
	return input->JustPressed( Engine::Input::kContinue );
}