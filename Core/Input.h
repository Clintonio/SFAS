//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The base application, creates the window and device.
// 
// Add a summary of your changes here:
// 
// 
// 

#pragma once

#include <Windows.h>
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

	bool JustPressed( Key key ) const;
	bool JustReleased( Key key ) const;
	bool Held( Key key ) const;
	bool PressedWithRepeat( Key key ) const;

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

	KeyState mKeyStates[kNumInputOptions];
};
};
