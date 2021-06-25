#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <Windows.h>
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

	CHAR_INFO* m_screen;
	int m_screenWidth;
	int m_screenHeight;
	float m_deltaTime;

	std::wstring m_name = L"Default Name";

	void Thread() {

		auto frameStarted = std::chrono::steady_clock::now();

		int newKeyState[256];
		int oldKeyState[256];
		std::memset(newKeyState, 0, 256 * sizeof(short));
		std::memset(oldKeyState, 0, 256 * sizeof(short));

		while (m_isActive)
		{
			// Calculate time between frames
			std::chrono::duration<float> elapsedTime = std::chrono::steady_clock::now() - frameStarted;
			m_deltaTime = elapsedTime.count();
			frameStarted = std::chrono::steady_clock::now();

			// Handle user input
			for (int i = 0; i < 256; i++) {   

				m_keys[i].pressed = false;
				m_keys[i].released = false;

				newKeyState[i] = GetAsyncKeyState(i);

				if (newKeyState[i] != oldKeyState[i]) {
					if (newKeyState[i] & 0x8000) {
						m_keys[i].pressed = !m_keys[i].held;
						m_keys[i].held = true;
					}
					else {
						m_keys[i].released = true;
						m_keys[i].held = false;
					}
				}
				oldKeyState[i] = newKeyState[i];
			}

			// Set console title
			wchar_t title[256];
			swprintf_s(title, 256, L"%s – FPS: %3.2f", m_name.c_str(), 1.0f / m_deltaTime);
			SetConsoleTitle(title);

			// Display screen buffer
			WriteConsoleOutputW(
				m_hConsole,
				m_screen,
				{ (SHORT)m_screenWidth, (SHORT)m_screenHeight },
				{ 0,0 },
				&m_window
			);
		}
	}

public:
	Engine(int screenWidth, int screenHeight, int fontSize) {
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_screen = new CHAR_INFO[screenWidth * screenHeight];

		SetConsoleWindowInfo(m_hConsole, TRUE, &m_window);

		SetConsoleScreenBufferSize(m_hConsole, { (SHORT)screenWidth, (SHORT)screenHeight });
		SetConsoleActiveScreenBuffer(m_hConsole);

		CONSOLE_FONT_INFOEX cfi;
		CONSOLE_CURSOR_INFO cci;

		// Hide cursor
		cci.dwSize = 25;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(m_hConsole, &cci);

		// Set font and font size
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = fontSize;
		cfi.dwFontSize.Y = fontSize;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(m_hConsole, FALSE, &cfi);

		m_window = { 0, 0, (SHORT)screenWidth - 1, (SHORT)screenHeight - 1 };
		SetConsoleWindowInfo(m_hConsole, TRUE, &m_window);

		// Disable window resize
		HWND consoleWindow;
		consoleWindow = GetConsoleWindow();
		SetWindowLongPtr(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

		std::memset(m_keys, 0, 256 * sizeof(KeyState));
		std::memset(m_screen, 0, m_screenWidth * m_screenHeight * sizeof(CHAR_INFO));
	}

	~Engine() {
		delete[] m_screen;
	}

	void Start() {
		OnGameStarted();
		m_isActive = true;
		std::thread th = std::thread(&Engine::Thread, this);
		th.join();
	}

	virtual void Update() = 0;
	virtual void OnGameStarted() = 0;

	void SetName(std::wstring title) {
		m_name = title;
	}

protected:
	bool m_isActive = false;

	struct KeyState {
		bool pressed;
		bool released;
		bool held;
	} m_keys[256];

	void SetSymbol(int x, int y, CHAR_INFO symbol) {
		m_screen[x + y * m_screenWidth] = symbol;
	}

	void FillRect(int minX, int minY, int width, int height, CHAR_INFO symbol) {
		for (int y = minY; y < minY + height; y++) {
			for (int x = minX; x < minX + width; x++) {
				SetSymbol(x, y, symbol);
			}
		}
	}
};

#endif ENGINE_H