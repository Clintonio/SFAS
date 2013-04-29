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

#include "Input.h"

using Engine::Input;

const int Input::sKeyCodes[kNumInputOptions] = { 32, 87, 83, 65, 68, 73, 75, 74, 76 };
const float Input::kfButtonRepeatTime = 0.2f;

Input::Input()
{
	for( int count = 0; count < kNumInputOptions; count++ )
	{
		mKeyStates[count].time = 0.0f;
		mKeyStates[count].KeyDown = false;
		mKeyStates[count].LastFrameKeyDown = false;
	}
}

Input::~Input(void)
{
}

void Input::OnKeyDown( WPARAM parameter1, LPARAM parameter2 )
{
	for( int count = 0; count < kNumInputOptions; count++ )
	{
		if( sKeyCodes[count] == parameter1 )
		{
			// This key is now down
			mKeyStates[count].time = 0.0f;
			mKeyStates[count].KeyDown = true;
		}
	}
}

void Input::OnKeyUp( WPARAM parameter1, LPARAM parameter2 )
{
	for( int count = 0; count < kNumInputOptions; count++ )
	{
		if( sKeyCodes[count] == parameter1 )
		{
			// This key is now down
			mKeyStates[count].time = 0.0f;
			mKeyStates[count].KeyDown = false;
		}
	}
}

void Input::Update( float dt )
{
	for( int count = 0; count < kNumInputOptions; count++ )
	{
		mKeyStates[count].time += dt;
		mKeyStates[count].LastFrameKeyDown = mKeyStates[count].KeyDown;

		if( mKeyStates[count].KeyDown && mKeyStates[count].time >= kfButtonRepeatTime )
		{
			mKeyStates[count].Repeat = true;
			mKeyStates[count].time = 0.0f;
		}
		else
		{
			mKeyStates[count].Repeat = false;
		}
	}
}

bool Input::JustPressed( Key key ) const
{
	return ( mKeyStates[key].KeyDown && !mKeyStates[key].LastFrameKeyDown );
}

bool Input::JustReleased( Key key ) const
{
	return ( !mKeyStates[key].KeyDown && mKeyStates[key].LastFrameKeyDown );
}

bool Input::Held( Key key ) const
{
	return ( mKeyStates[key].KeyDown && mKeyStates[key].LastFrameKeyDown );
}

bool Input::PressedWithRepeat( Key key ) const
{
	return ( mKeyStates[key].KeyDown && mKeyStates[key].Repeat );
}