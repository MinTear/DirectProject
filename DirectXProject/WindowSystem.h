#pragma once
#ifndef _WINDOWSYSTEM_H_
#define __WINDOWSYSTEM_H_

#define WIN32_LEAN_AND_MEAN

#include "stdafx.h"
#include "DirectXProject.h"

#include "WindowGraphics.h"
#include "KeyInput.h"

class WindowSystem
{
public:
	WindowSystem();
	WindowSystem(const WindowSystem&);
	~WindowSystem();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static WindowSystem* ApplicationHandle = 0;

#endif