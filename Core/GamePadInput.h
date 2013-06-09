#pragma once

#include <Windows.h>
#include <XInput.h>
#include "Input.h"

using namespace Engine;

namespace Engine
{
class GamePadInput : public Input
{
public:
	GamePadInput(unsigned short controllerNum);
	~GamePadInput(void);

	void Update( float dt );

	// Make the gamepad rumble
	void Vibrate(float duration, float strength);
	// Check if this controller is connected
	bool IsConnected() const;
private:
	struct KeyMap 
	{
		Key key;
		int code;

		KeyMap(Key key, int code) : key(key), code(code) { }
	};
	
	static KeyMap sKeyCodes[kNumInputOptions];

	// The gamepad controller number, usually ranges from 0-3
	unsigned short m_ControllerNumber;
	// Amount of time left to vibrate
	float m_VibrateTimeRemaining;
	
	static const unsigned short kJoyStickThreshold = 3000;
	static const unsigned char kTriggerThreshold = 50;
};
};
