//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Starting state that the player always sees before gameplay starts.  
// 
// Add a summary of your changes here:
// 
// 
// 
#pragma once

#include "GameStateBase.h"

namespace SFAS
{

namespace States                  
{    

class StartState : public GameStateBase
{
public:
	StartState( LPDIRECT3DDEVICE9 p_dx_Device );
	~StartState(void);

	void Render( float dt );
	int Update( Engine::Input * input, float dt );

	static const int kStateID = 0;
};
}
}