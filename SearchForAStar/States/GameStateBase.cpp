//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Game state base class, represents the states that the game software can be in.
// 
// Add a summary of your changes here:
// - Removed WCHAR references, simplified with wstring
// - Removed most of the original functions replaced with more versatile system
// 

#include "GameStateBase.h"
#include "Graphics/TextRenderer.h"
#include "../../Core/JSONParser.h"

using namespace SFAS::States;
using namespace Engine::JSON;

GameStateBase::GameStateBase( LPDIRECT3DDEVICE9 p_dx_Device ) :
	m_Cursor( p_dx_Device )
{
	// The crosshair for the player
	m_Cursor.Init( L"Textures/crosshair.png" );
}

GameStateBase::~GameStateBase(void)
{
}

void GameStateBase::Render( float dt )
{
	m_Cursor.Draw(
		D3DXVECTOR3(m_PlayerMousePosition.x, m_PlayerMousePosition.y, 0),
		D3DXVECTOR3(50, 50, 0)
	);
}

void GameStateBase::RenderOverlay( Engine::TextRenderer * txt, int winWidth, int winHeight )
{
	m_WindowWidth  = winWidth;
	m_WindowHeight = winHeight;
	int size = m_TextItems.size();
	for( int i = 0; i < size; i++ )
	{
		if( m_TextItems[i].size == FontSize::Medium )
		{
			txt->DrawMediumTextLine( 
				m_TextItems[i].strMsg, 
				(int) (m_TextItems[i].x * 2 * winWidth), 
				(int) (m_TextItems[i].y * winHeight), 
				m_TextItems[i].colour
			);
		}
		else if( m_TextItems[i].size == FontSize::Small )
		{
			// Quick hack because time is running out
			txt->DrawDebug(
				m_TextItems[i].strMsg, 
				(int) (m_TextItems[i].x * 2 * winWidth), 
				(int) (m_TextItems[i].y * winHeight), 
				m_TextItems[i].colour
			);
		}
		else
		{
			txt->DrawTextLine( 
				m_TextItems[i].strMsg, 
				(int) (m_TextItems[i].x * 2 * winWidth), 
				(int) (m_TextItems[i].y * winHeight), 
				m_TextItems[i].colour
			);
		}
	}
}

void GameStateBase::UpdateText (
	const int id,
	const std::wstring message
)
{
	int size = m_TextItems.size();
	for( int i = 0; i < size; i ++ )
	{
		if( m_TextItems[i].id == id ) 
		{
			m_TextItems[i].strMsg = message;
		}
	}
}

void GameStateBase::AddText( 
	const int id,
	const std::wstring message, 
	const float posX, 
	const float posY, 
	FontSize size,
	const D3DXCOLOR colour )
{
	Text text;
	text.id = id;
	text.colour = colour;
	text.x = posX;
	text.y = posY;
	text.size = size;
	text.strMsg = message;
	m_TextItems.push_back(text);
}

const GameStateBase::Text * GameStateBase::GetTextAt( const D3DXVECTOR2 & mousePos ) const
{
	const Text * out = NULL;

	int size = m_TextItems.size();
	for( int i = 0; i < size; i++ )
	{
		Text item = m_TextItems[i];
		int width = ( FontSize::Medium == item.size ? 24 : 32 ) * item.strMsg.length();
		int height = ( FontSize::Medium == item.size ? 24 : 32 );
		
		int posX = (int) (m_WindowWidth * item.x - (width / 2));
		int posY = (int) (m_WindowHeight - m_WindowHeight * item.y - 2 * height);

		if( mousePos.x > posX && mousePos.x < posX + width 
			&& mousePos.y > posY && mousePos.y < posY + height )
		{
			out = &m_TextItems[i];
		}
	}

	return out;
}

void GameStateBase::LoadHighScores( const std::string scoreFile )
{
	JSONParser parser;
	unsigned int i = 0; 
	try {
		const JSONMapNode * root = parser.ParseJSONFile( scoreFile );
		if( root != NULL )
		{
			JSONArrayNode * scores = (JSONArrayNode*) (*root)["scores"];
			for( ; i < scores->childCount && i < kNumHighScores; i++ )
			{
				JSONMapNode * scoreNode = (JSONMapNode*) scores->child[i];
				m_HighestScores[i].score = scoreNode->GetChildInt( "score" );
				m_HighestScores[i].name = scoreNode->GetChildString( "name" );
			}
		}
	}
	catch ( std::runtime_error e )
	{
		// File may not exist, ignore
	}
	// Fill in remaining scores
	for(; i < kNumHighScores; i++ )
	{
			m_HighestScores[i].score = 0;
			m_HighestScores[i].name = "";
	}
}