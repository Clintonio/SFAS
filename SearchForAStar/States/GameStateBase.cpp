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

#include "GameStateBase.h"
#include "Graphics/TextRenderer.h"

using SFAS::States::GameStateBase;

GameStateBase::GameStateBase(void)
{
	wcscpy_s(	m_Text[keTitleText].strMsg, L"NO TITLE TEXT HAS BEEN SET ON THIS GAME STATE\0" );
	m_Text[keTitleText].x = 800;
	m_Text[keTitleText].y = 200;
	m_Text[keTitleText].colour = D3DXCOLOR( 0.2f, 0.8f, 0.7f, 1 );

	wcscpy_s(	m_Text[kePageText].strMsg, L"NO PAGE TEXT HAS BEEN SET ON THIS GAME STATE\0" );
	m_Text[kePageText].x = 800;
	m_Text[kePageText].y = 300;
	m_Text[kePageText].colour = D3DXCOLOR( 0.2f, 0.8f, 0.7f, 1 );

	wcscpy_s(	m_Text[keInstructionText].strMsg, L"NO INSTRUCTIONS TEXT HAS BEEN SET ON THIS GAME STATE\0" );
	m_Text[keInstructionText].x = 800;
	m_Text[keInstructionText].y = 500;
	m_Text[keInstructionText].colour = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1 );
}

GameStateBase::~GameStateBase(void)
{
}

void GameStateBase::RenderOverlay( Engine::TextRenderer * txt )
{
	for( int count = 0; count < keNumTextToDisplay; count++ )
	{
		txt->DrawTextLine( m_Text[count].strMsg, m_Text[count].x, m_Text[count].y, m_Text[count].colour );
	}
}

void GameStateBase::SetTitleText( const WCHAR* text )
{
	wcscpy_s(m_Text[keTitleText].strMsg, text);
}

void GameStateBase::SetPageText( const WCHAR* text )
{
	wcscpy_s(	m_Text[kePageText].strMsg, text );
}

void GameStateBase::SetInstructionText( const WCHAR* text )
{
	wcscpy_s(	m_Text[keInstructionText].strMsg, text );
}

