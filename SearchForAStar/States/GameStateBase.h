//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Game state base class, represents the states that the game software can be in.
// 
// Add a summary of your changes here:
// 
// 
// 

#pragma once

#include <windows.h>
#include <wchar.h>
#include <d3dx9.h>


// Forward declare 
namespace Engine { class TextRenderer; class Input; }      

namespace SFAS
{

namespace States                  
{          

class GameStateBase
{
public:
	GameStateBase(void);
	virtual ~GameStateBase(void);

	// Rendering of a text overlay - can be overriden but not required
	virtual void RenderOverlay( Engine::TextRenderer* txt );

	// Basic rendering and updating - must be implemented by inheriting classes
	virtual void Render( float dt ) = 0;
	virtual bool Update( const Engine::Input * input, float dt ) = 0;

	// Inheriting classes can override these events but not required
	virtual void OnEnteringState() {}
	virtual void OnLeavingState() {}

protected:

	// The inheriting classes to change on screen text
	void SetTitleText( const WCHAR* text );
	void SetPageText( const WCHAR* text );
	void SetInstructionText( const WCHAR* text );

private:

	// Maximum size of a string within a text object
	// currently stored in the associated buffer.

	enum { keMaxStringLength = 32 };

	// Everything we need to know to display a bit of text to the screen
	struct Text
	{
		WCHAR strMsg[keMaxStringLength]; 
		int x; 
		int y; 
		D3DXCOLOR colour;
	};

	// The only text that a state will typically need to display
	enum TextDisplayed
	{
		keTitleText,
		kePageText,
		keInstructionText,
		keNumTextToDisplay	
	};

	Text m_Text[keNumTextToDisplay];
};
}
}