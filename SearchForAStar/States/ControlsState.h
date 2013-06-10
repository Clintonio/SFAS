// State for the player when they are viewing the
// controls page
#pragma once

#include <d3d9.h>
#include "GameStateBase.h"

namespace SFAS
{

namespace States                  
{    

class ControlsState : public GameStateBase
{
public:
	ControlsState( LPDIRECT3DDEVICE9 p_dx_Device );
	~ControlsState(void);

	// Overrides
	int Update( Engine::Input * input, float dt );
	void OnEnteringState();
	
	static const int kStateID = 4;
private:

};
}
}