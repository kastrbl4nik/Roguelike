#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#ifndef UNICODE
	#error Enable unicode for your compiler
#endif

#ifndef RL_PLATFORM_WINDOWS
	#error This application only supports Windows!
#endif

#include <Windows.h>
#include <WinUser.h>
#include <thread>
#include <string>

enum COLOR {
	FG_BLACK        = 0x0000,
	FG_DARK_BLUE    = 0x0001,
	FG_DARK_GREEN   = 0x0002,
	FG_DARK_CYAN    = 0x0003,
	FG_DARK_RED     = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW  = 0x0006,
	FG_GREY         = 0x0007,
	FG_DARK_GREY    = 0x0008,
	FG_BLUE         = 0x0009,
	FG_GREEN        = 0x000A,
	FG_CYAN         = 0x000B,
	FG_RED          = 0x000C,
	FG_MAGENTA      = 0x000D,
	FG_YELLOW       = 0x000E,
	FG_WHITE        = 0x000F,
	BG_BLACK        = 0x0000,
	BG_DARK_BLUE    = 0x0010,
	BG_DARK_GREEN   = 0x0020,
	BG_DARK_CYAN    = 0x0030,
	BG_DARK_RED     = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW  = 0x0060,
	BG_GREY         = 0x0070,
	BG_DARK_GREY    = 0x0080,
	BG_BLUE         = 0x0090,
	BG_GREEN        = 0x00A0,
	BG_CYAN         = 0x00B0,
	BG_RED          = 0x00C0,
	BG_MAGENTA      = 0x00D0,
	BG_YELLOW       = 0x00E0,
	BG_WHITE        = 0x00F0,
};

class Engine {
private:
	HANDLE		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT	m_window = { 0, 0, 1, 1 };

	std::wstring m_name = L"Default Name";

	void Thread();

public:
	Engine(int screenWidth, int screenHeight, int fontSize);
	virtual ~Engine();

	void Start();

	virtual void Update() = 0;                 // Update is called once every frame
	                                           // TODO: FixedUpdate()
	virtual void OnGameStarted() = 0;

	void SetName(std::wstring title);

protected:
	CHAR_INFO* m_screen;
	int m_screenWidth;
	int m_screenHeight;

	float m_deltaTime;

	bool m_isActive = false;

	struct KeyState {
		bool pressed;
		bool released;
		bool held;
	} m_keys[256];

	struct Mouse {
		POINT cursorPosition;
		int horizontalAxis;
		int verticalAxis;
	} m_mouse;

	void SetSymbol(int x, int y, CHAR_INFO symbol);
	void FillRect(int minX, int minY, int width, int height, CHAR_INFO symbol);
};

#endif ENGINE_H // !ENGINE_H