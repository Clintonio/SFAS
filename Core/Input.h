//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The base application, creates the window and device.
// 
// Add a summary of your changes here:
// - Added mouse click function signatures
// 
// 

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

	void Update( float dt );

	enum Key
	{
		kContinue,
		kUp, 
		kDown,
		kLeft,
		kRight,
		kFireUp,
		kFireDown,
		kFireLeft,
		kFireRight,
		kExit,

		kNumInputOptions
	};

	bool JustPressed( Key key ) const;
	bool JustReleased( Key key ) const;
	bool Held( Key key ) const;
	bool PressedWithRepeat( Key key ) const;

	// Returns true if the user has clicked in the last frame
	bool HasUserClicked ( Button btn ) const;
	// Returns true if the given button is currently held down
	bool IsButtonHeld ( Button btn ) const;
	// Get the current position of the given mouse cursor
	const D3DXVECTOR2 &GetMousePosition ( Button btn ) const;
	
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

private:

	static const int sKeyCodes[kNumInputOptions];
	static const float kfButtonRepeatTime;
	static const int kClickTolerance = 5;
	
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

	int m_WindowHeight, m_WindowWidth;

	// Verify whether the given mouse event is a click
	const bool inClickRadius( MouseButton btn, int xPos, int yPos );

	MouseButton m_MouseButton1;

	KeyState mKeyStates[kNumInputOptions];
};
};
