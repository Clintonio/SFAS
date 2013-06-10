//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Search For A Star application implementaton file. 
// 
// Add a summary of your changes here:
// - Fixed the fuction definition for the window procedure.
// - Added mouse capture functionality
// - Mapped windows cancel message to the escape key
// - Hidden mouse cursor during execution with an internal crosshair icon to represent it in gameplay

#include "SearchForAStar.h"
#include "States\GameStateBase.h"
#include "SearchForAStar/States/StartState.h"
#include "SearchForAStar/States/MainGameState.h"
#include "SearchForAStar/States/SummaryState.h"
#include "SearchForAStar/States/CreditsState.h"
#include "SearchForAStar/States/ControlsState.h"

#include "Graphics/RenderItem.h"
#include "Graphics/TextRenderer.h"
#include <wchar.h>
#include <windows.h>
#include <stdexcept>

#include "../Core/JSONTypes.h"

using Engine::Application;
using SFAS::SearchForAStar;
using namespace SFAS;

SearchForAStar::SearchForAStar(WNDPROC proc) : Application(L"SearchForAStar", L"SearchForAStar", keScreenWidth, keScreenHeight, proc), m_Camera( keScreenWidth, keScreenHeight )
{	
	// Create anythng that the game needs here 
	D3DXCreateFont( GetDevice(), 32, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &m_pFont );
	D3DXCreateFont( GetDevice(), 24, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &m_pFontMed );
	D3DXCreateFont( GetDevice(), 14, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial", &m_pDebugFont );
	D3DXCreateSprite( GetDevice(), &m_pSprite );
	m_TextColour = D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f );

	try {
		LPDIRECT3DDEVICE9 device = GetDevice();
		m_StateArray[0] = new States::StartState( device );
		m_StateArray[1] = new States::MainGameState( device, GetWindow(), keScreenWidth, keScreenHeight );
		m_StateArray[2] = new States::SummaryState( device );
		m_StateArray[3] = new States::CreditsState( device );
		m_StateArray[4] = new States::ControlsState( device );
		m_State = m_StateArray[0];
		m_Camera.Init( GetDevice() );
		ShowCursor( FALSE );
	} 
	catch (std::runtime_error e)
	{
		MessageBox(GetWindow(), L"Could not start game due to an error", L"Runtime Error", MB_OK);
		m_AppRunning = false;
	}
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

	if( m_pFontMed )
	{
		m_pFontMed->Release();
	}
	
	if( m_pSprite )
	{
		m_pSprite->Release();
	}
}

void SearchForAStar::Render(float dt)
{
	// Draw game components here
	m_State->Render( dt );

	// Overlay the text
	Engine::TextRenderer textRenderer( m_pFont, m_pFontMed, m_pDebugFont, m_pSprite );
	m_State->RenderOverlay( &textRenderer, GetWindowWidth(), GetWindowHeight() );
}

void SearchForAStar::Update(float dt)
{
	int newStateID;

	// Update the time in state
	m_TimeInState += dt;
	
	// This updates the matrices we need to render in 3D
	m_Camera.Update( GetDevice() );
	
	try 
	{
		Engine::Input* input = GetInput();
		if(input->JustPressed(Engine::Input::Key::kExit))
		{
			m_AppRunning = false;
		}
		// Update game logic here
		else
		{
			newStateID = m_State->Update( input, dt );
			if( newStateID > kSelfState )
			{
				// Reset the time spent in this state
				m_TimeInState = 0.0f;

				// Change state
				States::GameStateBase* oldState = m_State;
				m_State = m_StateArray[newStateID];

				m_State->OnEnteringState();
				oldState->OnLeavingState();
			} 
			else if (newStateID == kExitState) 
			{
				m_AppRunning = false;
			}
		}
	}
	catch( Engine::JSON::JSONCastException e )
	{
		MessageBox( GetWindow(), L"The game has crashed. One of the level JSON files are invalid", L"Game Has Crashed", MB_OK );
		m_AppRunning = false;
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

		case WM_COMMAND:
			// Capture the escape key
			if (LOWORD(parameter1) == IDCANCEL)
			{
				SFAS::app.OnKeyDown( VK_ESCAPE, parameter2 );
			}
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
int WINAPI WinMain( 
	_In_	 HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_	 LPSTR lpCmdLine, 
	_In_	 int nCmdShow )
{
	SFAS::app.Run();
	return 0;
}
