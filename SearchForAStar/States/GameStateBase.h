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

#include "Graphics/RenderItem.h"
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
	GameStateBase( LPDIRECT3DDEVICE9 p_dx_Device );
	virtual ~GameStateBase(void);

	// Rendering of a text overlay - can be overriden but not required
	void RenderOverlay( Engine::TextRenderer* txt, int winWidth, int winHeight );

	// Basic rendering and updating - must be implemented by inheriting classes
	virtual void Render( float dt );
	// Returns the state ID to go to next, -1 to stay the same
	virtual int Update( Engine::Input * input, float dt ) = 0;

	// Inheriting classes can override these events but not required
	virtual void OnEnteringState() {}
	virtual void OnLeavingState() {}

protected:
	enum { kNumHighScores = 20 };

	enum FontSize
	{
		Large,
		Medium,
		Small
	};

	// Everything we need to know to display a bit of text to the screen
	struct Text
	{
		int id;
		std::wstring strMsg; 
		float x; 
		float y; 
		FontSize size;
		D3DXCOLOR colour;
	};
	// Representation of a high score
	struct GameScore 
	{
		int score;
		std::string name;
	};

	// Gets the text clicked, if any
	const Text * GetTextAt( const D3DXVECTOR2 & mousePos ) const;
	// Load the highest scores from the json database
	void LoadHighScores( const std::string scoreFile );
	
	// A general add text function that uses relative positioning and size
	// rather than absolute
	void AddText(
		const int id,
		const std::wstring message, 
		const float posX, 
		const float posY, 
		FontSize size,
		const D3DXCOLOR colour 
	);
	// Like Add Text, but updates any text matching the ID
	void UpdateText(
		const int id,
		const std::wstring message 
	);
	// The cursor for this page
	Engine::RenderItem	m_Cursor;
	D3DXVECTOR2			m_PlayerMousePosition;
	// Array of all of our highest scores (if loaded)
	GameScore m_HighestScores[kNumHighScores];
private:
	// Local storage of window sizes, updated per frame
	int m_WindowHeight;
	int m_WindowWidth;

	// Dynamic list of items to display
	std::vector<Text> m_TextItems;
};
}
}