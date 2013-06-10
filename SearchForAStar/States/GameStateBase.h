//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Game state base class, represents the states that the game software can be in.
// 
// Add a summary of your changes here:
// - Replaced WCHAR with wstring for easier text rendering
// 
// 

#pragma once

#include <windows.h>
#include <wchar.h>
#include <d3dx9.h>
#include <vector>
#include <string>


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
	virtual bool Update( Engine::Input * input, float dt ) = 0;

	// Inheriting classes can override these events but not required
	virtual void OnEnteringState() {}
	virtual void OnLeavingState() {}

protected:

	// The inheriting classes to change on screen text
	void SetTitleText( const std::wstring text );
	void SetPageText( const std::wstring text );
	void SetInstructionText( const std::wstring text );

	void AddText( const std::wstring message, const int posX, const int posY, const D3DXCOLOR colour );

private:

	// Everything we need to know to display a bit of text to the screen
	struct Text
	{
		std::wstring strMsg; 
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
	// Dynamic list of items to display
	std::vector<Text> m_TextItems;
};
}
}