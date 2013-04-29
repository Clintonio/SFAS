//
// Source files for Search For A Star competition
// Copyright (c) 2010 L. Attwood
//
// Enemy class that patrols, hurts the player.
// 
// Add a summary of your changes here:
// 
// 
// 
#pragma once

#include "Entity.h"

namespace SFAS
{

namespace Game                  
{          

class Enemy : public Entity
{
public:
	Enemy( int id, float x, float y );
	virtual ~Enemy(void);

	virtual void Update( float dt );

	static const float sSize;
	static const float sSpeed;
	static const float sMass;
	static const float sDamping;

private:

	WCHAR * ToString()  const { return L"Enemy"; }

	void OnCollision( Entity& other );
};
}
}
