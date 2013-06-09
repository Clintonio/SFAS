//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The base application, creates the window and device.
// 
// Add a summary of your changes here:
// - Added ability to track whether the mouse has been clicked by the user in the last frame
// 
// 

#include "Input.h"

using Engine::Input;

const int Input::sKeyCodes[kNumInputOptions] = { 32, 87, 83, 65, 68, VK_ESCAPE, 0x50 };
const float Input::kfButtonRepeatTime = 0.2f;

Input::Input() : m_Enabled( true )
{
	for( int count = 0; count < kNumInputOptions; count++ )
	{
		m_KeyStates[count].time = 0.0f;
		m_KeyStates[count].KeyDown = false;
		m_KeyStates[count].LastFrameKeyDown = false;
	}

	m_MouseButton1.lastFrameClicked = false;
	m_MouseButton1.pressed = false;
}

Input::~Input(void)
{
}

void Input::SetWindowDimensions( int width, int height ) 
{
	m_WindowHeight = height;
	m_WindowWidth = width;
}

void Input::OnKeyDown( WPARAM parameter1, LPARAM parameter2 )
{
	for( int count = 0; count < kNumInputOptions; count++ )
	{
		if( sKeyCodes[count] == parameter1 )
		{
			// This key is now down
			m_KeyStates[count].time = 0.0f;
			m_KeyStates[count].KeyDown = true;
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
			m_KeyStates[count].time = 0.0f;
			m_KeyStates[count].KeyDown = false;
		}
	}
}

void Input::OnMouseDown ( Button btn, WPARAM status, LPARAM pos )
{
	if ( Input::Button::MouseButton1 == btn ) 
	{
		m_MouseButton1.pressed = true;
		m_MouseButton1.startX = GET_X_LPARAM(pos);
		m_MouseButton1.startY = GET_Y_LPARAM(pos);
	}
}

void Input::OnMouseUp( Button btn, WPARAM status, LPARAM pos )
{
	if ( Input::Button::MouseButton1 == btn ) 
	{
		int newX = GET_X_LPARAM(pos);
		int newY = GET_Y_LPARAM(pos);

		m_MouseButton1.lastFrameClicked = inClickRadius( m_MouseButton1, newX, newY );
		m_MouseButton1.lastClickX = m_MouseButton1.startX;
		m_MouseButton1.lastClickY = m_MouseButton1.startY;

		m_MouseButton1.pressed = false;
		m_MouseButton1.startX = 0;
		m_MouseButton1.startY = 0;
	}
}

void Input::OnMouseMove( Button btn, WPARAM status, LPARAM pos ) 
{
	if ( Input::Button::MouseButton1 == btn ) 
	{
		float newX = (float) GET_X_LPARAM(pos);
		float newY = (float) GET_Y_LPARAM(pos);
		// Mouse coordinates are based in top left, graphics coordinates bottom left
		// so translate the coordinates to match
		m_MouseButton1.position = D3DXVECTOR2(newX, m_WindowHeight - newY);
	}
}

const bool Input::inClickRadius( MouseButton btn, int xPos, int yPos ) 
{
	return ((xPos < btn.startX + Input::kClickTolerance) && (xPos > btn.startX - Input::kClickTolerance)
		&& (yPos < btn.startY + Input::kClickTolerance) && (yPos > btn.startY - Input::kClickTolerance));
}

void Input::Update( float dt )
{
	if( m_Enabled )
	{
		for( int count = 0; count < kNumInputOptions; count++ )
		{
			m_KeyStates[count].time += dt;
			m_KeyStates[count].LastFrameKeyDown = m_KeyStates[count].KeyDown;

			if( m_KeyStates[count].KeyDown && m_KeyStates[count].time >= kfButtonRepeatTime )
			{
				m_KeyStates[count].Repeat = true;
				m_KeyStates[count].time = 0.0f;
			}
			else
			{
				m_KeyStates[count].Repeat = false;
			}

			// Reset the mouse clicks
			m_MouseButton1.lastFrameClicked = false;
			m_MouseButton1.lastClickX = 0;
			m_MouseButton1.lastClickY = 0;

		}
	}
}

bool Input::JustPressed( Key key ) const
{
	return ( m_Enabled && m_KeyStates[key].KeyDown && !m_KeyStates[key].LastFrameKeyDown );
}

bool Input::JustReleased( Key key ) const
{
	return ( m_Enabled && !m_KeyStates[key].KeyDown && m_KeyStates[key].LastFrameKeyDown );
}

bool Input::Held( Key key ) const
{
	return ( m_Enabled && m_KeyStates[key].KeyDown && m_KeyStates[key].LastFrameKeyDown );
}

bool Input::PressedWithRepeat( Key key ) const
{
	return ( m_Enabled && m_KeyStates[key].KeyDown && m_KeyStates[key].Repeat );
}

bool Input::HasUserClicked( Button btn ) const
{
	if ( m_Enabled && btn == Button::MouseButton1 && m_MouseButton1.lastFrameClicked)
	{
		return true;
	}

	return false;
}

bool Input::IsButtonHeld ( Button btn ) const 
{
	if ( m_Enabled && btn == Button::MouseButton1 && m_MouseButton1.pressed ) 
	{
		return true;
	}

	return false;
}

const D3DXVECTOR2 &Input::GetMousePosition ( Button btn ) const {
	if ( btn == Button::MouseButton1 ) {
		return m_MouseButton1.position;
	}

	return D3DXVECTOR2(0,0);
}