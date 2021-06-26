#include "Engine.h"

Engine::Engine(int screenWidth, int screenHeight, int fontSize) {
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
Engine::~Engine() {
	delete[] m_screen;
}

void Engine::Start() {
	OnGameStarted();
	m_isActive = true;
	std::thread th = std::thread(&Engine::Thread, this);
	th.join();
}
void Engine::Thread() {

	auto frameStarted = std::chrono::steady_clock::now();

	int newKeyState[256];
	int oldKeyState[256];
	std::memset(newKeyState, 0, 256 * sizeof(short));
	std::memset(oldKeyState, 0, 256 * sizeof(short));

	GetCursorPos(&m_mouse.cursorPosition);
	int oldMouseX = m_mouse.cursorPosition.x;
	int oldMouseY = m_mouse.cursorPosition.y;

	while (m_isActive)
	{
		// Calculate time between frames
		std::chrono::duration<float> elapsedTime = std::chrono::steady_clock::now() - frameStarted;
		m_deltaTime = elapsedTime.count();
		frameStarted = std::chrono::steady_clock::now();

		// Handle keyboard input
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

		// Handle mouse input
		oldMouseX = m_mouse.cursorPosition.x;
		oldMouseY = m_mouse.cursorPosition.y;
		GetCursorPos(&m_mouse.cursorPosition);
		m_mouse.horizontalAxis = m_mouse.cursorPosition.x - oldMouseX;
		m_mouse.verticalAxis = m_mouse.cursorPosition.y - oldMouseY;

		// Set console title
		wchar_t title[256];
		swprintf_s(title, 256, L"%s – FPS: %3.2f", m_name.c_str(), 1.0f / m_deltaTime);
		SetConsoleTitle(title);

		Update();
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

void Engine::SetName(std::wstring title) {
	m_name = title;
}
void Engine::SetSymbol(int x, int y, CHAR_INFO symbol) {
	m_screen[x + y * m_screenWidth] = symbol;
}
void Engine::FillRect(int minX, int minY, int width, int height, CHAR_INFO symbol) {
	for (int y = minY; y < minY + height; y++) {
		for (int x = minX; x < minX + width; x++) {
			SetSymbol(x, y, symbol);
		}
	}
}