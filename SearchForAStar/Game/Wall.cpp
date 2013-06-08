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
#include "World.h"

using namespace SFAS::Game;

const Entity::EntityType Wall::kEntityType(19);

Wall::Wall( float x, float y, float width, float height ) : Entity( D3DXVECTOR3( x, y, 0.0f ), D3DXVECTOR3( width, height, 1.0f ), 0.0f )
{
	m_RenderItem = sTextureLoader->LoadTexturedRenderItem(L"textures/wall.png", 1.0f);
	SetMass(10000000000);
}

Wall::~Wall( void )
{
}

void Wall::Update( float dt )
{
	// Do nothing
}

bool Wall::OnCollision( Entity& other, World * world )
{
	// Do nothing
	return true;
}
