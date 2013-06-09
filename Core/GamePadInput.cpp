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

}

void GamePadInput::Update( float dt )
{
	XINPUT_STATE state;
	XInputGetState(m_ControllerNumber, &state);

	for( unsigned int i = 0; i < kNumInputOptions; i++ )
	{
		Key key = sKeyCodes[i].key;
		m_KeyStates[key].time += dt;
		m_KeyStates[key].LastFrameKeyDown = m_KeyStates[key].KeyDown;
		m_KeyStates[key].KeyDown = (state.Gamepad.wButtons & sKeyCodes[i].code);

		if( m_KeyStates[key].KeyDown && m_KeyStates[key].time >= kfButtonRepeatTime )
		{
			m_KeyStates[key].Repeat = true;
			m_KeyStates[key].time = 0.0f;
		}
		else
		{
			m_KeyStates[i].Repeat = false;
		}
	}

	// Update our mouse "positions"
	if( abs(state.Gamepad.sThumbLX) > kJoyStickThreshold )
	{
		m_MouseButton1.position.x += (short) 10 * (state.Gamepad.sThumbLX / SHRT_MAX);
		m_MouseButton1.position.x = min(max( m_MouseButton1.position.x, 0 ), m_WindowWidth );
	}
	if( abs(state.Gamepad.sThumbLY) > kJoyStickThreshold )
	{
		m_MouseButton1.position.y += (short) 10 * (state.Gamepad.sThumbLY / SHRT_MAX);
		m_MouseButton1.position.y = min(max( m_MouseButton1.position.y, 0 ), m_WindowHeight );
	}
	
	m_MouseButton1.lastFrameClicked = m_MouseButton1.pressed;
	// Right shoulder and trigger, as well as A are mapped to left click
	m_MouseButton1.pressed = (bool) ( ( state.Gamepad.wButtons 
		& ( XINPUT_GAMEPAD_RIGHT_SHOULDER | XINPUT_GAMEPAD_A ) ) ||
		( state.Gamepad.bRightTrigger > kTriggerThreshold ) );
	// Update our left click to simulate actual mouse behaviour
	if( !m_MouseButton1.pressed )
	{
		m_MouseButton1.startX = 0;
		m_MouseButton1.startY = 0;
	} 
	else if( !m_MouseButton1.lastFrameClicked )
	{
		m_MouseButton1.startX = m_MouseButton1.position.x;
		m_MouseButton1.startY = m_MouseButton1.position.y;
	}

	if( m_MouseButton1.pressed )
	{
		m_MouseButton1.lastClickX = m_MouseButton1.position.x;
		m_MouseButton1.lastClickY = m_MouseButton1.position.y;
	}

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