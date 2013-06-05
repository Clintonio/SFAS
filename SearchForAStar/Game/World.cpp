//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The World holds a player and objects and collides them against each other. 
// 
// Add a summary of your changes here:
// - Fixed int to floating point conversion errors
// - Fixed the update function to use pointers as per the definition in World.h on the bullet lists
// - Fixing null pointer exceptions caused by uninitialised entities in the entity list
// - Fixed attempting to delete invalid location in destructor due to <= comparison in loop
// - Fixed debug rendering attempting to access null pointer entities
// - Changed all D3DXCOLOR call parameters to floats for testing/ clarity purposes
// - Fixed onCollision being run twice on one target, and not on the other
// - Fixed game not reseting on player death
// - Fixed infinite loop caused by lack of update of iterator pointer when removing inactive bullets

#include "World.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Wall.h"
#include "Player.h"
#include "Core/Input.h"
#include "Graphics/RenderItem.h"

using SFAS::Game::World;
using SFAS::Game::Entity;
using SFAS::Game::Wall;
using SFAS::Game::Player;

World::World(LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h) : m_ActiveBullets(), m_Width( (float)w ), m_Height( (float)h ), m_Level( 0 ), m_NumActiveEnemies( INT_MAX )
{
	float wallWidth = 10.0f;
	float halfWallWidth = wallWidth * 0.5f;
	float halfAreaWidth = m_Width * 0.5f;
	float halfAreaHeight = m_Height * 0.5f;
	float windowHeight = (float) h;
	float windowWidth = (float) w;

	// Renderable Objects
	m_BlueSquare = new Engine::RenderItem( p_dx_Device, han_Window, 1.0f, D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f ) );
	m_RedSquare = new Engine::RenderItem( p_dx_Device, han_Window, 1.0f, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
	m_GreenSquare = new Engine::RenderItem( p_dx_Device, han_Window, 1.0f, D3DXCOLOR( 0.0f, 1.0, 0.0f, 1.0f ) );
	m_BlueSquare->Init();
	m_RedSquare->Init();
	m_GreenSquare->Init();

	// Player Object
	Player * player = new Player( kePlayer, kePlayerLives );
	player->SetPosition( D3DXVECTOR3( halfAreaWidth, halfAreaHeight, 0.0f ) );
	player->SetCollidable();
	m_Entities[kePlayer] = player;	

	// Create Enemies
	for( int count = 0; count < keNumEnemies; count++ )
	{
		Enemy * enemy = new Enemy( keEnemyStart + count, 
			(float)( wallWidth + ( rand() % (int)( w - ( wallWidth * 2 ) ) ) ), 
			(float)( wallWidth + ( rand() % (int)( h - ( wallWidth * 2 ) ) ) ) );
		enemy->SetActive( true );
		enemy->SetCollidable();
		m_Entities[keEnemyStart + count] = enemy;
	}

	// Create walls
	Wall * wall1 = new Wall( keWallStart, halfWallWidth, halfAreaHeight, wallWidth, windowHeight );
	Wall * wall2 = new Wall( keWallStart + 1, halfAreaWidth, halfWallWidth, windowWidth, wallWidth );
	Wall * wall3 = new Wall( keWallStart + 2, w - halfWallWidth, halfAreaHeight, wallWidth, windowHeight );
	Wall * wall4 = new Wall( keWallStart + 3, halfAreaWidth, h - halfWallWidth, windowWidth, wallWidth );

	wall1->SetActive( true );
	m_Entities[keWallStart] = wall1;

	wall2->SetActive( true );
	m_Entities[keWallStart + 1] = wall2;

	wall3->SetActive( true );
	m_Entities[keWallStart + 2] = wall3;

	wall4->SetActive( true );
	m_Entities[keWallStart + 3] = wall4;
}

World::~World(void)
{
	for( int count = 0; count < keNumEntities; count++ )
	{
		delete m_Entities[count];
	}

	delete m_BlueSquare;
}

void World::Render( float dt )
{
	// Render the player
	m_Entities[kePlayer]->Render( m_BlueSquare );

	// Render the enemies
	for( int count = keEnemyStart; count < keNumEntities; count++ )
	{
		if( m_Entities[count] != 0 )
		{
			m_Entities[count]->Render( m_RedSquare );
		}
	}

	// Render active bullets
	std::list<Bullet*>::const_iterator it = m_ActiveBullets.begin();
	std::list<Bullet*>::const_iterator end = m_ActiveBullets.end();
	while( it != end )
	{
		if( (*it)->IsActive() )
		{
			(*it)->Render( m_GreenSquare );
		}

		it++;
	}
}

void World::RenderDebug( Engine::TextRenderer * txt )
{
	// Render debug info
	for( int count = 0; count < keNumEntities; count++ )
	{
		if(m_Entities[count] != 0) {
			m_Entities[count]->RenderDebug( txt );
		}
	}
}

void World::Update( const Engine::Input * input, float dt )
{
	Player * player = GetPlayerHelper();
	
	if( player->IsActive() )
	{
		// Update the player - add bullets if neccessary
		Bullet * bullet = player->Update( input, dt );
		if( bullet != 0 )
		{
			m_ActiveBullets.push_back(bullet);
		}

		// Update active bullets and make a list of inactive bullets
		std::list<Bullet*> inactive;
		std::list<Bullet*>::const_iterator it = m_ActiveBullets.begin();
		std::list<Bullet*>::const_iterator end = m_ActiveBullets.end();
		while( it != end )
		{
			if( !(*it)->IsActive() )
			{
				inactive.push_back( *it );
			}
			else
			{
				(*it)->Update( dt );

				// Collide against walls
				for( int wall = 0; wall < keNumWalls; wall++ )
				{
					DoCollision( *it, m_Entities[keWallStart + wall], dt );
				}
			}

			it++;
		}

		// Remove inactive bullets
		it = inactive.begin();
		end = inactive.end();
		while( it != end )
		{
			m_ActiveBullets.remove( *it );
			it++;
		}

		// Collide player against walls
		for( int wall = 0; wall < keNumWalls; wall++ )
		{
			DoCollision( player, m_Entities[keWallStart + wall], dt );
		}

		m_NumActiveEnemies = 0;

		// Update enemies
		for( int count = kePlayer + 1; count < keWallStart; count++ )
		{
			// Check this enemy exists and is active 
			if( m_Entities[count] != 0 && !m_Entities[count]->IsActive() )
			{
				continue;
			}

			m_NumActiveEnemies++;

			// First update this enemy
			m_Entities[count]->Update( dt );

			// Collide against the player
			DoCollision( player, m_Entities[count], dt );

			// Collide against walls
			for( int wall = 0; wall < keNumWalls; wall++ )
			{
				DoCollision( m_Entities[count], m_Entities[keWallStart + wall], dt );
			}

			// Collide against the player bullets
			it = m_ActiveBullets.begin();
			end = m_ActiveBullets.end();
			while( it != end )
			{
				if( (*it)->IsActive() )
				{
					if( DoCollision( m_Entities[count], *it, dt ) )
					{
						GetPlayerHelper()->AddScore( keHitScore );
					}
				}

				it++;
			}

		}
	}
	else
	{
		if( m_NumActiveEnemies > 0 )
		{
			ResetLevel();
			m_Entities[kePlayer]->OnReset();
		}
	}
}

void World::NewGame()
{
	m_Level = 0;
	Player * player = GetPlayerHelper();
	player->ResetScore();
	player->ResetLives( kePlayerLives );
	m_ActiveBullets.clear();
}

bool World::IsGameOver() const
{
	if( GetPlayer()->GetLivesRemaining() == 0 )
	{
		return true;
	}

	return false;
}

void World::NextLevel()
{
	// Up the level
	m_Level++;
	ResetLevel();
	m_Entities[kePlayer]->OnReset();
	m_NumActiveEnemies = INT_MAX;
	m_ActiveBullets.clear();
}

void World::ResetLevel()
{
	float halfAreaWidth = m_Width * 0.5f;
	float halfAreaHeight = m_Height * 0.5f;

	// Reset the player position, velocity and rotation - then set to active
	m_Entities[kePlayer]->SetPosition( D3DXVECTOR3( halfAreaWidth, halfAreaHeight, 0.0f ) );
	m_Entities[kePlayer]->SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	m_Entities[kePlayer]->SetActive( true );
	m_ActiveBullets.clear();

	// Reset enemies
	for( int count = kePlayer + 1; count < keWallStart; count++ )
	{
		m_Entities[count]->SetActive( count <= m_Level );
	}
}

bool World::IsLevelFinished() const
{
	return ( m_NumActiveEnemies == 0 );
}

bool World::DoCollision( Entity * lh, Entity * rh, float dt )
{
	bool collision = false;

	if( lh != 0 && rh != 0 && lh->CheckForPossibleCollision( *rh, dt ) && lh->IsActive() && rh->IsActive() )
	{
		if( lh->CheckForCollision( *rh, dt ) )
		{
			collision = true;

			if( lh->IsCollidable() )
			{
				lh->Resolve( *rh, dt );
			}

			lh->OnCollision( *rh );
			rh->OnCollision( *lh );
		}
	}

	return collision;
}
