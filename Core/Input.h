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

	void OnKeyDown( WPARAM parameter1, LPARAM parameter2 );
	void OnKeyUp( WPARAM parameter1, LPARAM parameter2 );

	void OnMouseDown ( const short btn, WPARAM status, LPARAM pos );
	void OnMouseUp ( const short btn, WPARAM status, LPARAM pos );

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

		kNumInputOptions
	};

	// Mouse button enum
	enum Button { 
		MouseButton1
	};

	bool JustPressed( Key key ) const;
	bool JustReleased( Key key ) const;
	bool Held( Key key ) const;
	bool PressedWithRepeat( Key key ) const;

	// Returns true if the user has clicked in the last frame
	bool HasUserClicked ( Button btn ) const;
	

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
	};

	// Verify whether the given mouse event is a click
	const bool inClickRadius( MouseButton btn, int xPos, int yPos );

	MouseButton m_MouseButton1;

	KeyState mKeyStates[kNumInputOptions];
};
};
