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
// - Added a skybox

#include "World.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Wall.h"
#include "Player.h"
#include "Core/Input.h"
#include "Graphics/RenderItem.h"
#include "EntityList.h"
#include "Level.h"
#include "Graphics/SkyBox.h"
#include "Explosion.h"
#include "Audio/SoundProvider.h"
#include "Audio/Sound.h"

// Binary level includes
#include "Levels/Level1.h"

using SFAS::Game::World;
using SFAS::Game::Entity;
using SFAS::Game::Wall;
using SFAS::Game::Player;

World::World(LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h) 
	: m_Width( (float)w ), m_Height( (float)h ), m_Level( 0 ), m_NumActiveEnemies( INT_MAX )
{
	m_SoundProvider = new Engine::SoundProvider();
	m_SoundProvider->Init(han_Window);

	m_SkyBox = new Engine::SkyBox(p_dx_Device, w, h);
	m_EntityList = new EntityList;
	
	m_Player = new Player( kePlayerLives );
	m_Player->SetCollidable();

	Entity::Init(p_dx_Device);

	Engine::Sound* sound = m_SoundProvider->CreateSoundBufferFromFile("Sound/menumusic.wav");
	sound->PlaySoundFromStart();
	
	m_LevelMusic = m_SoundProvider->CreateSoundBufferFromFile("Sound/level1.wav");
}

World::~World(void)
{
	delete m_EntityList;
	delete m_SkyBox;
	delete m_SoundProvider;
	delete m_Player;
	if( m_LevelMusic != 0 )
	{
		delete m_LevelMusic;
	}
}

void World::AddEntity(Entity* entity) 
{
	m_EntityList->insert(entity);
	entity->LoadSounds(m_SoundProvider);
}

void World::Render( float dt )
{
	m_SkyBox->Render();
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

void World::Update( Engine::Input * input, float dt )
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
			for(innerIt = it; m_EntityList->end() != innerIt; innerIt++)
			{
				if(it != innerIt)
				{
					DoCollision((*it).second, (*innerIt).second, dt);
				}
			}
		}

		// Determine number of remaining enemies
		m_NumActiveEnemies = m_EntityList->count(Enemy::kEntityType);
	}
	else
	{
		if( m_NumActiveEnemies > 0 )
		{
			float vibrateStrength = 1.0f;
			float vibrateTime = 0.5f;
			input->Vibrate(vibrateTime, vibrateStrength);
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
	if( m_LevelMusic != 0 )
	{
		m_LevelMusic->PlaySoundFromStart();
	}
}

bool World::IsGameOver() const
{
	if( GetPlayer()->GetLivesRemaining() <= 0 )
	{
		return true;
	}

	return false;
}

void World::NextLevel()
{
	OpenLevel(m_Level);
	// Up the level
	m_Level++;
	ResetLevel();
	m_Player->OnReset();
	m_NumActiveEnemies = INT_MAX;
}

void World::OpenLevel( int level )
{
	Level * l = getLevel1();
	
	// Player Setup
	D3DXVECTOR3 start = l->player.startPos;

	m_Player->SetSpawnPosition( D3DXVECTOR3( m_Width * start.x, m_Height * start.y, 0 * start.z ) );
	m_Player->UpdateWithDescriptor( l->player );
	m_Player->SetActive( true );
	AddEntity(m_Player);

	// This section loads the enemies
	for(unsigned int i = 0; i < l->enemyCount; i++)
	{
		Level::Enemy curEnemy = l->enemies[i];
		D3DXVECTOR3 pos = curEnemy.pos;
		Enemy* enemy = new Enemy(l->enemyTypes[curEnemy.type], pos.x, pos.y);
		AddEntity(enemy);
	}
	
	// This section loads a skybox
	m_SkyBox->Init(l->skyboxTextureFile);

	float wallWidth = 10.0f;
	float halfWallWidth = wallWidth * 0.5f;
	float halfAreaWidth = m_Width * 0.5f;
	float halfAreaHeight = m_Height * 0.5f;
	float windowHeight = (float) m_Height;
	float windowWidth = (float) m_Width;
	
	// Create walls
	Wall * wall1 = new Wall( halfWallWidth, halfAreaHeight, wallWidth, windowHeight );
	Wall * wall2 = new Wall( halfAreaWidth, halfWallWidth, windowWidth, wallWidth );
	Wall * wall3 = new Wall( m_Width - halfWallWidth, halfAreaHeight, wallWidth, windowHeight );
	Wall * wall4 = new Wall( halfAreaWidth, m_Height - halfWallWidth, windowWidth, wallWidth );
	
	wall1->SetActive( true );
	wall2->SetActive( true );
	wall3->SetActive( true );
	wall4->SetActive( true );
	AddEntity(wall1);
	AddEntity(wall2);
	AddEntity(wall3);
	AddEntity(wall4);

	m_CurrentLevel = l;
}

void World::ClearLevel()
{
	m_EntityList->clear();
	if( m_LevelMusic != 0 )
	{
		m_LevelMusic->Stop();
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

	m_Player->Spawn();
}

bool World::IsLevelFinished() const
{
	return ( m_NumActiveEnemies <= 0 );
}

bool World::DoCollision( Entity * lh, Entity * rh, float dt )
{
	bool collision = false;

	if( lh->CheckForPossibleCollision( *rh, dt ) )
	{
		if( lh->CheckForCollision( *rh, dt ) )
		{
			collision = true;

			if( lh->IsCollidable() && rh->IsCollidable() )
			{
				bool physics = lh->OnCollision( *rh, this );
				physics &= rh->OnCollision( *lh, this );
				if( physics ) 
				{
					lh->Resolve( *rh, dt );
				}
			}
		}
	}

	return collision;
}

const Player * World::GetPlayer() const 
{ 
	return m_Player;
}

Player * World::GetPlayerHelper() 
{ 
	return m_Player;
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