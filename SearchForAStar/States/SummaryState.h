//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Summary state that displays the results of the last game. 
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

class SummaryState : public GameStateBase
{
public:
	SummaryState( LPDIRECT3DDEVICE9 p_dx_Device );
	~SummaryState(void);

	int Update( Engine::Input *input, float dt );

	static const int kStateID = 2;
};
}
}