#include "GamePadInput.h"

using namespace Engine;

#pragma comment(lib,  "XInput.lib")

/* Keypad values
XINPUT_GAMEPAD_DPAD_UP          0x00000001
XINPUT_GAMEPAD_DPAD_DOWN        0x00000002
XINPUT_GAMEPAD_DPAD_LEFT        0x00000004
XINPUT_GAMEPAD_DPAD_RIGHT       0x00000008
XINPUT_GAMEPAD_START            0x00000010
XINPUT_GAMEPAD_BACK             0x00000020
XINPUT_GAMEPAD_LEFT_THUMB       0x00000040
XINPUT_GAMEPAD_RIGHT_THUMB      0x00000080
XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
XINPUT_GAMEPAD_A                0x1000
XINPUT_GAMEPAD_B                0x2000
XINPUT_GAMEPAD_X                0x4000
XINPUT_GAMEPAD_Y                0x8000
*/

const float GamePadInput::kJoyStickThreshold = 0.3f;

struct GamePadInput::KeyMap GamePadInput::sKeyCodes[kNumInputOptions] = { 
	KeyMap(Key::kContinue, INT_MAX),
	KeyMap(Key::kExit, XINPUT_GAMEPAD_B),
	KeyMap(Key::kPause, XINPUT_GAMEPAD_START),
	KeyMap(Key::kUp, XINPUT_GAMEPAD_DPAD_UP),
	KeyMap(Key::kDown, XINPUT_GAMEPAD_DPAD_DOWN),
	KeyMap(Key::kLeft, XINPUT_GAMEPAD_DPAD_LEFT),
	KeyMap(Key::kRight, XINPUT_GAMEPAD_DPAD_RIGHT)
};

GamePadInput::GamePadInput(unsigned short controllerNum) : 
	Input(),
	m_ControllerNumber(controllerNum - 1),
	m_VibrateTimeRemaining(0.0f)
{
	// Set the "mouse" to defaults
	m_MouseButton1.lastClickX = 0;
	m_MouseButton1.lastClickY = 0;
	m_MouseButton1.position = D3DXVECTOR2( 0, 0 );
	m_MouseButton1.startX = 0;
	m_MouseButton1.startY = 0;
}

bool GamePadInput::IsConnected() const
{
	XINPUT_STATE state;
	DWORD result = XInputGetState(m_ControllerNumber, &state);
	return (result == ERROR_SUCCESS);
}

void GamePadInput::Vibrate( const float time, const float strength )
{
    XINPUT_VIBRATION vibration;
    ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

    vibration.wLeftMotorSpeed  = (unsigned short) (strength * 65535);
    vibration.wRightMotorSpeed = (unsigned short) (strength * 65535);

	XInputSetState(m_ControllerNumber, &vibration);

	m_VibrateTimeRemaining = time;
}

GamePadInput::~GamePadInput(void)
{
	// Remove any possible vibration before shutdown to
	// prevent the gamepad being stuck on vibrate.
	Vibrate(0.0f,0.0f);
}

void GamePadInput::Update( float dt )
{
	if( m_Enabled )
	{
		XInputGetState(m_ControllerNumber, &m_InputState);

		for( unsigned int i = 0; i < kNumInputOptions; i++ )
		{
			Key key = sKeyCodes[i].key;
			m_KeyStates[key].time += dt;
			m_KeyStates[key].LastFrameKeyDown = m_KeyStates[key].KeyDown;
			m_KeyStates[key].KeyDown = (m_InputState.Gamepad.wButtons & sKeyCodes[i].code);

			if( m_KeyStates[key].KeyDown && m_KeyStates[key].time >= kfButtonRepeatTime )
			{
				m_KeyStates[key].Repeat = true;
				m_KeyStates[key].time = 0.0f;
			}
			else
			{
				m_KeyStates[key].Repeat = false;
			}
		}

		DoGamePadMouseEmulation();
		// In this implementation we're mapping right joystick to the mouse
		DoRightJoyStickInput();
		DoLeftJoyStickInput();

		// Update the vibration
		if ( m_VibrateTimeRemaining > 0.0f )
		{
			m_VibrateTimeRemaining -= dt;

			if ( m_VibrateTimeRemaining <= 0.0f )
			{
				m_VibrateTimeRemaining = 0.0f;
				Vibrate(0.0f, 0.0f); // Disables vibration
			}
		}
	}
}

void GamePadInput::DoGamePadMouseEmulation()
{
	m_MouseButton1.lastFrameClicked = m_MouseButton1.pressed;
	// Right shoulder and trigger are mapped to left click
	m_MouseButton1.pressed = (bool) ( ( m_InputState.Gamepad.wButtons 
		& ( XINPUT_GAMEPAD_RIGHT_SHOULDER ) ) ||
		( m_InputState.Gamepad.bRightTrigger > kTriggerThreshold ) );
	// Update our left click to simulate actual mouse behaviour
	if( !m_MouseButton1.pressed )
	{
		m_MouseButton1.startX = 0;
		m_MouseButton1.startY = 0;
	} 
	else if( !m_MouseButton1.lastFrameClicked )
	{
		m_MouseButton1.startX = (int) m_MouseButton1.position.x;
		m_MouseButton1.startY = (int) m_MouseButton1.position.y;
	}

	if( m_MouseButton1.pressed )
	{
		m_MouseButton1.lastClickX = (int) m_MouseButton1.position.x;
		m_MouseButton1.lastClickY = (int) m_MouseButton1.position.y;
	}
}

void GamePadInput::DoLeftJoyStickInput()
{
	short xDist = m_InputState.Gamepad.sThumbLX;
	short yDist = m_InputState.Gamepad.sThumbLY;

	float magnitude = sqrt( (float) xDist * xDist + yDist * yDist );

	if( magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
	{
		//determine the direction the controller is pushed
		float normalizedX = xDist / magnitude;
		float normalizedY = yDist / magnitude;
	
		if( normalizedX > kJoyStickThreshold )
		{
			DoKeyPress(Key::kRight, true);
		} 
		else if( normalizedX < -kJoyStickThreshold )
		{
			DoKeyPress(Key::kLeft, true);
		}
	
		if( normalizedY > kJoyStickThreshold )
		{
			DoKeyPress(Key::kUp, true);
		} 
		else if( normalizedY < -kJoyStickThreshold )
		{
			DoKeyPress(Key::kDown, true);
		}
	}
}

void GamePadInput::DoRightJoyStickInput()
{
	short xDist = m_InputState.Gamepad.sThumbRX;
	short yDist = m_InputState.Gamepad.sThumbRY;

	float magnitude = sqrt( (float) xDist * xDist + yDist * yDist );

	if( magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
	{
		//determine the direction the controller is pushed
		float normalizedX = xDist / magnitude;
		float normalizedY = yDist / magnitude;
		
		m_MouseButton1.position.x += 10 * normalizedX;
		m_MouseButton1.position.y += 10 * normalizedY;
		
		m_MouseButton1.position.x = min( m_WindowWidth, max( 0, m_MouseButton1.position.x ) );
		m_MouseButton1.position.y = min( m_WindowHeight, max( 0, m_MouseButton1.position.y ) );
	}
}

void GamePadInput::DoKeyPress(const Key key, const bool pressed)
{
	m_KeyStates[key].KeyDown |= pressed;

	if( m_KeyStates[key].KeyDown && m_KeyStates[key].time >= kfButtonRepeatTime )
	{
		m_KeyStates[key].Repeat = true;
		m_KeyStates[key].time = 0.0f;
	}
}