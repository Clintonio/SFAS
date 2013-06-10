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

	// Emulates mouse input from the gamepad using the left joystick
	void DoGamePadMouseEmulation();
	// Get input from the right joystick and map it somewhere
	void DoRightJoyStickInput();
	// Get input from the left joystick and map it somewhere
	void DoLeftJoyStickInput();
	// Do a keypress from a virtual source, must be used after the main
	// keypress loop in the update method
	void DoKeyPress(const Key key, const bool pressed);
	// Set the sensitivity of the aim
	inline void SetSensitivity( const float sensitivity ) { m_Sensitivity = sensitivity; }
	
	static KeyMap sKeyCodes[kNumInputOptions];

	// The gamepad controller number, usually ranges from 0-3
	unsigned short m_ControllerNumber;
	// Amount of time left to vibrate
	float m_VibrateTimeRemaining;
	// Sensitivity, for Gamepad in range [0,1]
	float m_Sensitivity;
	
	static const float kJoyStickThreshold;
	static const unsigned char kTriggerThreshold = 50;

	XINPUT_STATE m_InputState;
};
};
