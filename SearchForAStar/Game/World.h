//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The World holds a player and objects and collides them against each other. 
// 
// Add a summary of your changes here:
// - Replaced entity system with more flexible entity list
// 
#pragma once

#include <d3d9.h>
#include <map>

// Forward declare
namespace Engine { class RenderItem; class Input; class TextRenderer; }

namespace SFAS
{

namespace Game                  
{          

class Entity;
class Wall;
class Bullet;
class Player;
class EntityList;

class World
{
public:
	World(LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, int w, int h);
	virtual ~World(void);

	virtual void Render( float dt );
	virtual void RenderDebug( Engine::TextRenderer * txt );
	virtual void Update( const Engine::Input * input, float dt );

	// Return a constant pointer to the player so that its state can be examined safely
	const Player * GetPlayer() const;

	// Game Setup Code  
	void NewGame();
	bool IsGameOver() const;
	void NextLevel();
	void ResetLevel();
	bool IsLevelFinished() const;
	int GetCurrentLevel() const { return m_Level; } 

	// Add an entity to the world
	void AddEntity(Entity* entity);

private:

	// None constant private helper accessor for the player
	Player * GetPlayerHelper();

	bool DoCollision( Entity * lh, Entity * rh, float dt );

	enum { kePlayerLives = 3, keNumLevels = 10, keHitScore = 100 };

	Engine::RenderItem *	m_BlueSquare;
	Engine::RenderItem *	m_RedSquare;
	Engine::RenderItem *	m_GreenSquare;
	EntityList *			m_EntityList;

	float					m_Width;
	float					m_Height;
	int						m_Level;
	int						m_NumActiveEnemies;
};
}
}