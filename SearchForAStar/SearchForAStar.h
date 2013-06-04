//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Search For A Star application implementaton file. 
// 
// Add a summary of your changes here:
// - Added keOptions to fix bug where it didn't exist
// 
// 
#pragma once

#include "Core/Application.h"
#include "Graphics/Camera.h"

namespace SFAS
{

// Forward declare to avoid including everything in the header
// GameStateBase is used in this class only with a pointer so
// it only needs to be declared it doesn't need to be defined 
// untill we get into the *.cpp file.
namespace States { class GameStateBase; }

// This is the main controller of our game, it inherits from 
// Application which gives it the ability to create a new 
// window and crate a DirectX object and device. In addition 
// to this we add code which is specific to SearchForAStar 
// to this game specific class.
class SearchForAStar : public Engine::Application
{
public:
	// We declare our own Window Procedure specifically for 
	// Search for a Star messaging below and pass it into
	// a single instance of this application
	SearchForAStar(WNDPROC proc);
	~SearchForAStar(void);

private:

	void Render( float dt );		// Draw the current game components
	void Update( float dt );		// Update the game with the delta time

	enum { keScreenWidth = 800 , keScreenHeight = 600 };

	enum GameState
	{
		keStart,					// Start Screen
		keGame,						// Gameplay
		keGameOver,					// Summary
		keOptions,					// Options Screen
									// New states here... 

		keNumStates					// The number of states that exist, 
									// this is automatically correct as long 
									// as you add in new states before it
	};

	GameState		m_State;		// Where we are in the game
	Engine::Camera	m_Camera;		// The camera for the main view
	float			m_TimeInState;	// The time spent in the current state
	ID3DXFont*      m_pFont;        // Font for drawing text
	ID3DXFont*		m_pDebugFont;	// Font for drawing debug text
	ID3DXSprite*    m_pSprite;      // Sprite for draw calls
    D3DXCOLOR		m_TextColour;	// The colour to render the text
	States::GameStateBase*  m_StateArray[keNumStates]; // Some things we don't want to be on screen 
									// all the time - we divide them into states
									// and only render 1 at a time. 	
};

// This is our Window Procedure that will handle messages that we care about 
// passing them back to the instance of SearchForAStar below.

LRESULT CALLBACK GameAppWindowProcedure(HWND han_Wind,UINT uint_Message,UINT parameter1,WPARAM parameter2);
static SearchForAStar app = SearchForAStar(GameAppWindowProcedure);

}