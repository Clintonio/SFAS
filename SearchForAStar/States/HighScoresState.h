//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The state when the player is in the options menu
// 
// 
#pragma once

#include <d3d9.h>
#include "GameStateBase.h"

namespace SFAS
{

namespace States                  
{    

class HighScoresState : public GameStateBase
{
public:
	HighScoresState( LPDIRECT3DDEVICE9 p_dx_Device );
	~HighScoresState(void);

	// Overrides
	int Update( Engine::Input * input, float dt );
	void OnEnteringState();

	static const int kStateID = 5;
private:

};
}
}