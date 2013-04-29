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

SummaryState::SummaryState(void)
{
	SetTitleText( L"GAME OVER" );
	SetInstructionText( L"Press Any Key To Continue..." );
}

SummaryState::~SummaryState(void)
{
}

void SummaryState::Setup( int score, int best ) 
{ 
	// Update on screen text
	WCHAR strBuffer[512];
	swprintf_s( strBuffer, 512, L"This Score: %d    Best Score: %d", score, best );
	SetPageText( strBuffer );
}

bool SummaryState::Update( const Engine::Input * input, float dt)
{
	return input->JustPressed( Engine::Input::kContinue );
}