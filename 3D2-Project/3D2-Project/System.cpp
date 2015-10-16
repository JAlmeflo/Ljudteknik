#include "System.h"
System* System::ApplicationHandle = 0;


System::System()
{
	m_input = 0;
	m_graphics = 0;
}


System::~System()
{
}

bool System::Initialize()
{
	bool result;

	screenWidth = 0;
	screenHeight = 0;
	InitializeWindows(screenWidth, screenHeight);
	firstUpdate = true;

	//Input
	m_input = new Input();
	result = m_input->Initialize();
	if (!result)
	{
		return false;
	}

	//Graphics
	m_graphics = new Graphics();
	result = m_graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void System::Shutdown()
{
	//Graphics
	m_graphics->Shutdown();
	delete m_graphics;
	m_graphics = 0;

	//Input
	delete m_input;
	m_input = 0;

	ShutdownWindows();
}

void System::Run()
{
	MSG msg;
	bool done, result;
	__int64 prevTimeStamp = 0;
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;
	
	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;
			m_fps = 1 / dt;
			SetWindowTitle();
			if (firstUpdate)
				firstUpdate = false;
			else
			{
				result = Update(dt);
				if (!result)
				{
					done = true;
				}
				result = Render();
				if (!result)
				{
					done = true;
				}
			}

			prevTimeStamp = currTimeStamp;
		}
	}
}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		case WM_KEYDOWN:
		{
			m_input->KeyDown((unsigned int)wparam);
			return 0;
		}
		case WM_KEYUP:
		{
			m_input->KeyUp((unsigned int)wparam);
			return 0;
		}
		default:
		{
		   return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

bool System::Render()
{
	if (!m_graphics->Frame())
	{
		return false;
	}

	return true;
}

bool System::Update(float deltaTime)
{
	m_graphics->Update(deltaTime);

	m_input->Update(deltaTime);

	if (m_input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	LPRECT Rect = new RECT();
	GetWindowRect(m_hwnd, Rect);
	LPPOINT mousePos = new POINT();
	GetCursorPos(mousePos);

	int centerX = (Rect->left + Rect->right) * 0.5f;
	int centerY = (Rect->top + Rect->bottom) * 0.5f;
	D3DXVECTOR2 mouseMovement = D3DXVECTOR2(mousePos->x - centerX, centerY - mousePos->y);

	m_input->UpdateMouse(mouseMovement.x, mouseMovement.y, deltaTime);
	
	SetCursorPos(centerX, centerY);
	return true;
}

void System::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;

	int posX, posY;
	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = "3D2 project";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.

	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 1600;
		screenHeight = 900;
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
	LPRECT Rect = new RECT();
	GetWindowRect(m_hwnd, Rect);
	int centerX = (Rect->left + Rect->right) * 0.5f;
	int centerY = (Rect->top + Rect->bottom) * 0.5f;
	SetCursorPos(centerX, centerY);
	
}

void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
}

LRESULT CALLBACK System::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_QUIT:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			 return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

void System::SetWindowTitle()
{
	//LPCSTR text = "FPS:";
	std::string text;
	text = "FPS:   ";
	text.append(std::to_string((int)m_fps));

	
	//SetWindowTextA(m_hwnd, (LPCSTR)text.c_str());
}