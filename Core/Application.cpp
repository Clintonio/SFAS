//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The base application, creates the window and device.
// 
// Add a summary of your changes here:
// - Added mouse capture functions
// - Added game pad input
// 

#include "Application.h"
#include "GamePadInput.h"

using Engine::Application;

bool Application::m_AppRunning = true;

Application::Application(LPCTSTR str_Title, LPCTSTR str_Class, int int_Width, int int_Height, WNDPROC proc) : m_Window(), m_Device(0), m_Title(str_Title), m_Class(str_Class), m_Width(int_Width), m_Height(int_Height)
{
	CreateApplicationWindow(10, 10, proc);
	InitializeApplicationDevice();
	InitialiseInput();
}

Application::~Application(void)
{
	delete m_Input;
}

void Application::Run()
{
	MSG msg_Message;
	while(m_AppRunning)
	{
		// Timing
	    LARGE_INTEGER llThisTime;
	    LARGE_INTEGER frequency;
		QueryPerformanceCounter((LARGE_INTEGER *)&llThisTime);
		QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
		double dt = ((double)(llThisTime.QuadPart - m_llLastTime.QuadPart) / frequency.QuadPart);
		float fDT = static_cast<float>( dt );

		// Start Draw 
		m_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
		m_Device->BeginScene();

		// Render the application
		Render( fDT );

		// End Draw
		m_Device->EndScene();
		m_Device->Present(NULL, NULL, NULL, NULL);

		// Update Stuff
		Update( fDT );

		// Update input
		m_Input->Update( fDT );

		if(PeekMessage(&msg_Message, m_Window, 0, 0, PM_REMOVE))
		{
			if(!IsDialogMessage(m_Window,&msg_Message))
			{
				DispatchMessage(&msg_Message);
			}
		}

		// Timing
		m_llLastTime = llThisTime; 
	}

	m_Device->Release();
	m_dx_Object->Release();
	DestroyWindow(m_Window);
}

void Application::OnKeyDown( WPARAM parameter1, LPARAM parameter2 )
{
	if( m_Input != 0 )
	{
		m_Input->OnKeyDown( parameter1, parameter2 );
	}
}

void Application::OnKeyUp( WPARAM parameter1, LPARAM parameter2 )
{
	if( m_Input != 0 )
	{
		m_Input->OnKeyUp( parameter1, parameter2 );
	}
}

void Application::OnMouseDown( Engine::Input::Button btn, WPARAM parameter1, LPARAM parameter2 )
{
	if( m_Input != 0) 
	{
		m_Input->OnMouseDown(btn, parameter1, parameter2);
	}
}

void Application::OnMouseUp( Engine::Input::Button btn, WPARAM parameter1, LPARAM parameter2 )
{
	if( m_Input != 0) 
	{
		m_Input->OnMouseUp(btn, parameter1, parameter2);
	}
}

void Application::OnMouseMove( Engine::Input::Button btn, WPARAM parameter1, LPARAM parameter2 )
{
	if( m_Input != 0) 
	{
		m_Input->OnMouseMove(btn, parameter1, parameter2);
	}
}

void Application::InitialiseInput()
{
	GamePadInput * gpadInput = new GamePadInput(1);
	if( gpadInput->IsConnected() )
	{
		m_Input = gpadInput;
	} 
	else
	{
		delete gpadInput;
		m_Input = new Input();
	}
	m_Input->SetWindowDimensions(m_Width, m_Height);
}

void Application::CreateApplicationWindow(int int_XPos, int int_YPos, WNDPROC proc)
{
	WNDCLASSEX wnd_Structure;

	wnd_Structure.cbSize = sizeof(WNDCLASSEX);
	wnd_Structure.style = CS_HREDRAW | CS_VREDRAW;
	wnd_Structure.lpfnWndProc = proc;
	wnd_Structure.cbClsExtra = 0;
	wnd_Structure.cbWndExtra = 0;
	wnd_Structure.hInstance = GetModuleHandle(NULL);
	wnd_Structure.hIcon = NULL;
	wnd_Structure.hCursor = NULL;
	wnd_Structure.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wnd_Structure.lpszMenuName = NULL;
	wnd_Structure.lpszClassName = m_Class;
	wnd_Structure.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	
	RegisterClassEx(&wnd_Structure);
 
	m_Window = CreateWindowEx(WS_EX_CONTROLPARENT, m_Class, m_Title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, int_XPos, int_YPos, m_Width, m_Height, NULL, NULL, GetModuleHandle(NULL), NULL); 
}

void Application::InitializeApplicationDevice()
{
	m_dx_Object = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_dx_Object == NULL)
	{
		MessageBox(m_Window,L"DirectX Runtime library not installed!",L"InitializeDevice()",MB_OK);
	}

	D3DPRESENT_PARAMETERS dx_PresParams;

	ZeroMemory( &dx_PresParams, sizeof(dx_PresParams) );
	dx_PresParams.Windowed = TRUE;
	dx_PresParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dx_PresParams.BackBufferFormat = D3DFMT_X8R8G8B8;
	dx_PresParams.BackBufferCount = 1;
	dx_PresParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	dx_PresParams.MultiSampleQuality = 0;
	dx_PresParams.Flags = NULL;
	dx_PresParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	dx_PresParams.EnableAutoDepthStencil = TRUE;
	dx_PresParams.AutoDepthStencilFormat = D3DFMT_D16;
	
	if (FAILED(m_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_Window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_PresParams, &m_Device)))
	{
		if (FAILED(m_dx_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dx_PresParams, &m_Device)))
		{
			MessageBox(m_Window,L"Failed to create even the reference device!",L"InitializeDevice()",MB_OK);
		}
	}
}