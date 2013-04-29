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

using SFAS::Game::Wall;

Wall::Wall( int id, float x, float y, float width, float height ) : Entity( id, D3DXVECTOR3( x, y, 0.0f ), D3DXVECTOR3( width, height, 1.0f ), 0.0f )
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
