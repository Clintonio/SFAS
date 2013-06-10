//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Text rendering helper class.  
// 
// Add a summary of your changes here:
// - Fixed spelling errors on names the two ID3DXFont private variables
// - Using wstring instead of WCHAR
// 
#pragma once

#include <windows.h>
#include <d3dx9.h>
#include <string>

namespace Engine
{

class TextRenderer
{
public:
	TextRenderer(ID3DXFont* font, ID3DXFont* fontMedium, ID3DXFont* debug_font, ID3DXSprite* sprite);
	~TextRenderer(void);
	
	HRESULT DrawTextLine( const std::wstring & strMsg, int x, int y, D3DXCOLOR colour );
	HRESULT DrawMediumTextLine( const std::wstring & strMsg, int x, int y, D3DXCOLOR colour );
	HRESULT DrawDebug( const std::wstring & strMsg, int x, int y, D3DXCOLOR colour );

private:
	
	ID3DXFont*      m_pFont;        // Font for drawing text
	ID3DXFont*      m_pFontMedium;  // Font for drawing text in medium size
	ID3DXFont*		m_pDebugFont;   // Font for drawing debug text
	ID3DXSprite*    m_pSprite;      // Sprite for draw calls
};
}