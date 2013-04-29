//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// An imovable object which can support the player. 
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


class Bullet : public Entity
{
public:
	Bullet( int id );
	virtual ~Bullet(void);

	void Update( float dt );
	void Fire( float vx, float vy );

	void SetActive( bool active );
	
	bool IsPlayerControlled() const { return true; }

private: 

	WCHAR * ToString()  const { return L"Bullet"; }

	enum { kLifetime = 10, kForce = 2000 };

	static const float sSize;
	static const float sMass;
	static const float sDamping;

	void OnCollision( Entity& other );
};
}
}