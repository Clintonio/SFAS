//
// Source files for Search For A Star competition
// Copyright (c) 2012 L. Attwood
//
// An imovable object which can support the player. 
// 
// Add a summary of your changes here:
// 
// 
// 

#include "Wall.h"

using namespace SFAS::Game;

Engine::RenderItem * Wall::sRenderItem = 0;
const Entity::EntityType Wall::kEntityType(19);

Wall::Wall( float x, float y, float width, float height ) : Entity( D3DXVECTOR3( x, y, 0.0f ), D3DXVECTOR3( width, height, 1.0f ), 0.0f )
{
}

Wall::~Wall( void )
{
}

void Wall::Update( float dt )
{
	// Do nothing
}

void Wall::OnCollision( Entity& other )
{
	// Do nothing
}
