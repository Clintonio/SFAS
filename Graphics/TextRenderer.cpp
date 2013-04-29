//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Text rendering helper class.  
// 
// Add a summary of your changes here:
// 
// 
// 

#include "TextRenderer.h"

using Engine::TextRenderer;

TextRenderer::TextRenderer(ID3DXFont* font, ID3DXFont* debug_font, ID3DXSprite* sprite)
{
	m_pFont = font;
	m_pDebugFont = debug_font;
	m_pSprite = sprite;

	m_pSprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
}

TextRenderer::~TextRenderer(void)
{
	m_pSprite->End();
}

HRESULT TextRenderer::DrawTextLine( const WCHAR* strMsg, int x, int y, D3DXCOLOR colour )
{
    HRESULT hr;
    RECT rc;
    SetRect( &rc, x, y, 0, 0 );
    hr = m_pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_CENTER | DT_NOCLIP, colour );
	return hr;
}

HRESULT TextRenderer::DrawDebug( const WCHAR* strMsg, int x, int y, D3DXCOLOR colour )
{
    HRESULT hr;
    RECT rc;
    SetRect( &rc, x, y, 0, 0 );
    hr = m_pDebugFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_CENTER | DT_NOCLIP, colour );
	return hr;
}

