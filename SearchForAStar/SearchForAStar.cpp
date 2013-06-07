//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Search For A Star application implementaton file. 
// 
// Add a summary of your changes here:
// - Fixed the fuction definition for the window procedure.
// - Added mouse capture functionality
// 
// 

#include "SearchForAStar.h"
#include "SearchForAStar/States/StartState.h"
#include "SearchForAStar/States/MainGameState.h"
#include "SearchForAStar/States/SummaryState.h"
#include "SearchForAStar/States/OptionsState.h"

#include "Graphics/RenderItem.h"
#include "Graphics/TextRenderer.h"
#include <wchar.h>
#include <windows.h>

using Engine::Application;
using SFAS::SearchForAStar;

SearchForAStar::SearchForAStar(WNDPROC proc) : Application(L"SearchForAStar", L"SearchForAStar", keScreenWidth, keScreenHeight, proc), m_Camera( keScreenWidth, keScreenHeight )
{	
	// Create anythng that the game needs here 
	D3DXCreateFont( GetDevice(), 32, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &m_pFont );
	D3DXCreateFont( GetDevice(), 12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &m_pDebugFont );
	D3DXCreateSprite( GetDevice(), &m_pSprite );
	m_TextColour = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

	m_StateArray[keStart] = new States::StartState();
	m_StateArray[keGame] = new States::MainGameState( GetDevice(), GetWindow(), keScreenWidth, keScreenHeight );
	m_StateArray[keGameOver] = new States::SummaryState();
	m_StateArray[keOptions] = new States::OptionsState();
	
	m_Camera.Init( GetDevice() );
}

SearchForAStar::~SearchForAStar(void)
{
	for( int count = 0; count < keNumStates; count++ )
	{
		delete m_StateArray[count];
	}

	// Delete anything that the game has 
	if( m_pFont )
	{
		m_pFont->Release();
	}

	if( m_pDebugFont )
	{
		m_pDebugFont->Release();
	}
	
	if( m_pSprite )
	{
		m_pSprite->Release();
	}
}

void SearchForAStar::Render(float dt)
{
	// Draw game components here
	m_StateArray[m_State]->Render( dt );

	// Overlay the text
	Engine::TextRenderer textRenderer( m_pFont, m_pDebugFont, m_pSprite );
	m_StateArray[m_State]->RenderOverlay( &textRenderer );
}

void SearchForAStar::Update(float dt)
{
	// Update the time in state
	m_TimeInState += dt;
	
	// This updates the matrices we need to render in 3D
	m_Camera.Update( GetDevice() );

	// Update game logic here
	if( m_StateArray[m_State]->Update( GetInput(), dt ) )
	{
		// Reset the time spent in this state
		m_TimeInState = 0.0f;

		// Change state
		GameState old_state = m_State;
		m_State = (GameState)( ( m_State + 1 ) % keNumStates );

		// If the game is over then update the next state
		if( m_State == keGameOver )
		{
			const Game::Player * player = reinterpret_cast<States::MainGameState*>( m_StateArray[old_state] )->GetWorld()->GetPlayer();
			reinterpret_cast<States::SummaryState*>( m_StateArray[m_State] )->Setup(player->GetScore(), player->GetBestScore() );
		}

		m_StateArray[m_State]->OnEnteringState();
		m_StateArray[old_state]->OnLeavingState();
	}
}

// Windows Messages 
LRESULT CALLBACK SFAS::GameAppWindowProcedure(HWND han_Wind,UINT uint_Message,WPARAM parameter1,LPARAM parameter2)
{
	switch(uint_Message)
	{
		case WM_DESTROY:
		case WM_QUIT:
			// Shutdown
			SFAS::app.Stop();
			return 0;

		case WM_KEYDOWN:
			// Send these to the application
			SFAS::app.OnKeyDown( parameter1, parameter2 );
			break;

		case WM_KEYUP:
			// Send these to the application
			SFAS::app.OnKeyUp( parameter1, parameter2 );
			break;

		case WM_LBUTTONDOWN:
			SFAS::app.OnMouseDown(Engine::Input::Button::MouseButton1, parameter1, parameter2);
			break;

		case WM_LBUTTONUP:
			SFAS::app.OnMouseUp(Engine::Input::Button::MouseButton1, parameter1, parameter2);
			break;

		case WM_MOUSEMOVE:
			SFAS::app.OnMouseMove(Engine::Input::Button::MouseButton1, parameter1, parameter2);
			break;
	}

	return DefWindowProc(han_Wind,uint_Message,parameter1,parameter2);
}

// Main Entry Point
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	SFAS::app.Run();
	return 0;
}
