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
// - Replaced Entity array system with more flexible entity list
// - Added helper utilities for entities to find other entities
// - Added level loading from level descriptor format

#include "World.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Wall.h"
#include "Player.h"
#include "Core/Input.h"
#include "Graphics/RenderItem.h"
#include "EntityList.h"
#include "Level.h"

// Binary level includes
#include "Levels/Level1.h"

using SFAS::Game::World;
using SFAS::Game::Entity;
using SFAS::Game::Wall;
using SFAS::Game::Player;

World::World(LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h) : m_Width( (float)w ), m_Height( (float)h ), m_Level( 0 ), m_NumActiveEnemies( INT_MAX )
{
	float wallWidth = 10.0f;
	float halfWallWidth = wallWidth * 0.5f;
	float halfAreaWidth = m_Width * 0.5f;
	float halfAreaHeight = m_Height * 0.5f;
	float windowHeight = (float) h;
	float windowWidth = (float) w;

	m_EntityList = new EntityList;

	// Renderable Objects
	m_BlueSquare = new Engine::RenderItem( p_dx_Device, han_Window, 1.0f, D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f ) );
	m_RedSquare = new Engine::RenderItem( p_dx_Device, han_Window, 1.0f, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
	m_GreenSquare = new Engine::RenderItem( p_dx_Device, han_Window, 1.0f, D3DXCOLOR( 0.0f, 1.0, 0.0f, 1.0f ) );
	m_BlueSquare->Init();
	m_RedSquare->Init();
	m_GreenSquare->Init();
	
	Player::sRenderItem = m_BlueSquare;
	Wall::sRenderItem = m_RedSquare;
	Enemy::sRenderItem = m_RedSquare;
	Bullet::sRenderItem = m_GreenSquare;

	// Player Object
	Player * player = new Player(  kePlayerLives );
	player->SetPosition( D3DXVECTOR3( halfAreaWidth, halfAreaHeight, 0.0f ) );
	player->SetCollidable();
	m_EntityList->insert(player);

	// Create walls
	Wall * wall1 = new Wall( halfWallWidth, halfAreaHeight, wallWidth, windowHeight );
	Wall * wall2 = new Wall( halfAreaWidth, halfWallWidth, windowWidth, wallWidth );
	Wall * wall3 = new Wall( w - halfWallWidth, halfAreaHeight, wallWidth, windowHeight );
	Wall * wall4 = new Wall( halfAreaWidth, h - halfWallWidth, windowWidth, wallWidth );
	
	wall1->SetActive( true );
	wall2->SetActive( true );
	wall3->SetActive( true );
	wall4->SetActive( true );
	m_EntityList->insert(wall1);
	m_EntityList->insert(wall2);
	m_EntityList->insert(wall3);
	m_EntityList->insert(wall4);
}

World::~World(void)
{
	delete m_EntityList;

	delete m_RedSquare;
	delete m_BlueSquare;
	delete m_GreenSquare;
}

void World::AddEntity(Entity* entity) 
{
	m_EntityList->insert(entity);
}

void World::Render( float dt )
{
	EntityList::iterator it;
	// Render all items
	for(it = m_EntityList->begin(); m_EntityList->end() != it; it++)
	{
		((*it).second)->Render();
	}
}

void World::RenderDebug( Engine::TextRenderer * txt )
{
	EntityList::iterator it;
	// Render all items
	for(it = m_EntityList->begin(); m_EntityList->end() != it; it++)
	{
		((*it).second)->RenderDebug(txt);
	}
}

void World::Update( const Engine::Input * input, float dt )
{
	Player * player = GetPlayerHelper();
	
	if( player->IsActive() )
	{
		player->DoInput(this, input);

		EntityList::iterator it;
		std::vector<EntityList::iterator> eraseLocations;
		// Update all items and remove inactive ones
		for(it = m_EntityList->begin(); m_EntityList->end() != it; it++)
		{
			Entity * entity = (*it).second;
			if( entity->IsActive() )
			{
				entity->Update(this, dt);
			} 
			else
			{
				eraseLocations.push_back(it);
			}
		}

		// Erase all inactive entities
		for(std::vector<EntityList::iterator>::iterator eraseIt = eraseLocations.begin();
			eraseIt != eraseLocations.end(); eraseIt++)
		{
			m_EntityList->erase(*eraseIt);
		}

		// Do all collisions
		for(it = m_EntityList->begin(); m_EntityList->end() != it; it++)
		{
			EntityList::iterator innerIt;
			for(innerIt = m_EntityList->begin(); m_EntityList->end() != innerIt; innerIt++)
			{
				if(innerIt != it)
				{
					DoCollision((*it).second, (*innerIt).second, dt);
				}
			}
		}
	}
	else
	{
		if( m_NumActiveEnemies > 0 )
		{
			ResetLevel();
			player->OnReset();
		}
	}
}

void World::NewGame()
{
	m_Level = 0;
	Player * player = GetPlayerHelper();
	player->ResetScore();
	player->ResetLives( kePlayerLives );
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
	Player * player = GetPlayerHelper();
	// Up the level
	m_Level++;
	ResetLevel();
	player->OnReset();
	m_NumActiveEnemies = INT_MAX;
	OpenLevel(m_Level);
}

void World::OpenLevel( int level )
{
	Level l = getLevel1();

	for(unsigned int i = 0; i < l.enemyCount; i++)
	{
		D3DXVECTOR3 pos = l.enemies[i].pos;
		Enemy* enemy = new Enemy(pos.x, pos.y);
		AddEntity(enemy);
	}
}

void World::ResetLevel()
{
	float halfAreaWidth = m_Width * 0.5f;
	float halfAreaHeight = m_Height * 0.5f;
	
	EntityList::iterator it;
	// Reset all items
	for(it = m_EntityList->begin(); m_EntityList->end() != it; it++)
	{
		((*it).second)->OnReset();
	}

	Player * player = GetPlayerHelper();
	// Reset the player position, velocity and rotation - then set to active
	player->SetPosition( D3DXVECTOR3( halfAreaWidth, halfAreaHeight, 0.0f ) );
	player->SetVelocity( D3DXVECTOR3( 0.0f, 0.0f, 0.0f ) );
	player->SetActive( true );
}

bool World::IsLevelFinished() const
{
	return ( m_NumActiveEnemies == 0 );
}

bool World::DoCollision( Entity * lh, Entity * rh, float dt )
{
	bool collision = false;

	if( lh != 0 && rh != 0 && lh->CheckForPossibleCollision( *rh, dt ) )
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

const Player * World::GetPlayer() const 
{ 
		return ((Player*) (*m_EntityList->GetAllEntitiesOfType(Player::kEntityType).begin()));
}

Player * World::GetPlayerHelper() 
{ 
		return ((Player*) (*m_EntityList->GetAllEntitiesOfType(Player::kEntityType).begin()));
}

const Entity* World::FindNearestEntityOfType( const Entity * origin, const Entity::EntityType &typeID ) const 
{
	std::vector<Entity*> entities = m_EntityList->GetAllEntitiesOfType(typeID);
	Entity* closest = NULL;
	float minDist = FLT_MAX;
	for(std::vector<Entity*>::iterator it = entities.begin(); it < entities.end(); it++)
	{
		float curDist = origin->DistanceTo(*it);
		if((NULL == closest) || (curDist < minDist))
		{
			minDist = curDist;
			closest = *it;
		}
	}

	return closest;
}