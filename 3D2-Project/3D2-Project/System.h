#pragma once
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include "Input.h"
#include "Graphics.h"
#include <string>

class System
{
public:
	System();
	~System();

	bool Initialize();
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Render();
	bool Update(float deltaTime);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
	void SetWindowTitle();

	LPCSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	int screenWidth, screenHeight;
	float m_fps;
	bool firstUpdate;

	Input* m_input;
	Graphics* m_graphics;

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static System* ApplicationHandle;
};

