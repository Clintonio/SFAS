//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The base application, creates the window and device.
// 
// Add a summary of your changes here:
// - Added mouse capture function signatures
// - Made it possible for child classes to exit the application
// 

#pragma once

#include <Windows.h>
#include <d3d9.h> 
#include <d3dx9.h> 
#include "Input.h"

namespace Engine
{

class Application
{
public:
	Application(LPCTSTR str_Title, LPCTSTR str_Class, int int_Width, int int_Height, WNDPROC proc);
	virtual ~Application(void);

	// Stop and Run are the only methods that need to be exposed
	inline static void Stop() { m_AppRunning = 0; }	
	void Run();

	// Pass on key down and on key up events onto the input manager
	void OnKeyDown( WPARAM parameter1, LPARAM parameter2 );
	void OnKeyUp( WPARAM parameter1, LPARAM parameter2 );

	// Pass mouse events to input manager
	void Application::OnMouseDown( Input::Button btn, WPARAM parameter1, LPARAM parameter2 );
	void Application::OnMouseUp( Input::Button btn, WPARAM parameter1, LPARAM parameter2 );
	void Application::OnMouseMove( Input::Button btn, WPARAM parameter1, LPARAM parameter2 );

protected:

	inline HWND GetWindow() { return m_Window; } 
	inline LPDIRECT3DDEVICE9 GetDevice() { return m_Device; }	
	inline Input * GetInput() const { return m_Input; }
	
	void ToggleFullScreen();
	
	static bool m_AppRunning;
private:

	// The game specific code is required to define it's own draw and update methods
	virtual void Render( float dt ) = 0;
	virtual void Update( float dt ) = 0;

	void CreateApplicationWindow(int int_XPos, int int_YPos, WNDPROC proc);
	void InitializeApplicationDevice();
	void InitialiseInput();

	LARGE_INTEGER GetCurrentTime();

	HWND				m_Window;
	Input *				m_Input;
	LPDIRECT3DDEVICE9	m_Device;
	LPDIRECT3D9			m_dx_Object;
	LPCTSTR				m_Title;
	LPCTSTR				m_Class;
	int					m_Width;
	int					m_Height;

    bool m_bPaused;
    LARGE_INTEGER m_llLastTime;
	// This variable monitors whether we're currently in full screen or not
	bool m_FullScreen;
};
};
