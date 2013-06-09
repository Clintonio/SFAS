// XBox 360 XInput gamepad input mode

#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <d3d9.h> 
#include <d3dx9.h> 

namespace Engine
{
class Input
{
public:
	Input();
	virtual ~Input(void);

	// Mouse button enum
	enum Button { 
		MouseButton1
	};

	void OnKeyDown( WPARAM parameter1, LPARAM parameter2 );
	void OnKeyUp( WPARAM parameter1, LPARAM parameter2 );

	void OnMouseDown ( Button btn, WPARAM status, LPARAM pos );
	void OnMouseUp ( Button btn, WPARAM status, LPARAM pos );
	void OnMouseMove ( Button btn, WPARAM status, LPARAM pos );

	// Provides vibration feedback if supported
	virtual void Vibrate(float duration, float strength) { }
	// Update the current state of input
	virtual void Update( float dt );
	// Disable input updating
	void Disable() { m_Enabled = false; }
	// Enable input updating
	void Enable() { m_Enabled = true; }

	enum Key
	{
		kContinue,
		kUp, 
		kDown,
		kLeft,
		kRight,
		kExit,
		kPause,

		kNumInputOptions
	};

	virtual bool JustPressed( Key key ) const;
	virtual bool JustReleased( Key key ) const;
	virtual bool Held( Key key ) const;
	virtual bool PressedWithRepeat( Key key ) const;

	// Returns true if the user has clicked in the last frame
	virtual bool HasUserClicked ( Button btn ) const;
	// Returns true if the given button is currently held down
	virtual bool IsButtonHeld ( Button btn ) const;
	// Get the current position of the given mouse cursor
	virtual const D3DXVECTOR2 &GetMousePosition ( Button btn ) const;
	
	// Set the window height for mapping mouse inputs
	void SetWindowDimensions( int width, int height );

protected:

	struct KeyState
	{
		float time;
		bool KeyDown;
		bool LastFrameKeyDown;
		bool Repeat;
	};

	struct MouseButton 
	{
		int startX;
		int startY;
		bool pressed;
		bool lastFrameClicked;
		int lastClickX;
		int lastClickY;
		D3DXVECTOR2 position;
	};
	
	KeyState m_KeyStates[kNumInputOptions];
	static const float kfButtonRepeatTime;

	MouseButton m_MouseButton1;
	
	int m_WindowHeight, m_WindowWidth;

	bool m_Enabled;
private:

	static const int sKeyCodes[kNumInputOptions];
	static const int kClickTolerance = 5;
	

	// Verify whether the given mouse event is a click
	const bool inClickRadius( MouseButton btn, int xPos, int yPos );


};
};
