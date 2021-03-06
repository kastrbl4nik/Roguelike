#include "Game.h"
#include <fstream>

Game::Game(const char* map) : Engine(100, 60, 8) {

	std::wifstream levelData(map);
	ASSERT(levelData);				// Map file wasn't open
	
	std::wstring line;

	int y = 0;
	bool playerFound = false;
	int playerX, playerY;

	while (levelData >> line)
	{
		y++;
		if (!playerFound)
		{
			size_t foundPos = line.find(L'P');
			if (foundPos != std::wstring::npos)
			{
				playerFound = true;
				playerX = foundPos;
				playerY = y;
				line.replace(foundPos, 1, L"_");
			}
		}
		m_map += line;
	}
	m_mapWidth = line.length();
	m_mapHeight = y;

	ASSERT(playerFound) // Player not found, try adding 'P' symbol inside your map, where player should be spawned

	m_player = std::make_unique<Player>(playerX, playerY, 0);
}

Game::~Game()
{
}

void Game::OnGameStarted() {
}

void Game::Update() {

	// Look Around
	if(abs(m_mouse.horizontalAxis) > 1)
		m_player->IncrementAngle(m_mouse.horizontalAxis * m_deltaTime / 5.0f);

	// Walk
	float deltaMove = m_deltaTime * Player::Speed;
	if(m_keys['W'].held) {
		m_player->Walk(deltaMove, 1, 0);
	}
	if (m_keys['A'].held) {
		m_player->Walk(deltaMove, 0, -1);
	}
	if (m_keys['S'].held) {
		m_player->Walk(deltaMove, -1, 0);
	}
	if (m_keys['D'].held) {
		m_player->Walk(deltaMove, 0, 1);
	}


	for (int x = 0; x < m_screenWidth; x++) {
		float rayAngle = (m_player->GetAngle() - Player::FOV / 2) + ((float)x / (float)m_screenWidth * Player::FOV);
		float distToWall = 0;
		bool hitWall = false;

		float dirX = sinf(rayAngle);
		float dirY = cosf(rayAngle);

		while(!hitWall && distToWall < Player::ViewDistance) {
			distToWall += 0.01f;

			int testX = (int)(m_player->GetX() + dirX * distToWall);
			int testY = (int)(m_player->GetY() + dirY * distToWall);

			if(testX < 0 || testX >= m_mapWidth || testY < 0 || testY > m_mapHeight) {
				hitWall = true;
				distToWall = Player::ViewDistance;
			}
			else {
				if(m_map[testX + testY * m_mapWidth] == '#') {
					hitWall = true;
				}
			}
		}

		int distToCeiling = (float)(m_screenHeight / 2) - m_screenHeight / (float)distToWall;
		int distToFloor = m_screenHeight - distToCeiling;

		CHAR_INFO shadedWall;

		if (distToWall <= Player::ViewDistance / 4.0f) shadedWall		= { ' ', BG_WHITE };
		else if (distToWall < Player::ViewDistance / 3.0f) shadedWall	= { u'\u2591', BG_WHITE | FG_BLUE };
		else if (distToWall < Player::ViewDistance / 2.0f) shadedWall	= { u'\u2592', BG_WHITE | FG_BLUE };
		else if (distToWall < Player::ViewDistance) shadedWall			= { u'\u2593', BG_WHITE | FG_BLUE };
		else shadedWall													= { ' ', BG_BLUE };

		for (int y = 0; y < m_screenHeight; y++) {
			if (y < distToCeiling)
				SetSymbol(x, y, { ' ', COLOR::BG_BLUE });
			else if (y > distToCeiling && y <= distToFloor)
				SetSymbol(x, y, shadedWall);
			else {
				float brightness = 1.0f - (((float)y - m_screenHeight / 2.0f) / ((float)m_screenHeight / 2.0f));
				CHAR_INFO shadedFloor;

				if (brightness < 0.25f)
					shadedFloor = { ' ', BG_GREEN };
				else if (brightness < 0.5f)
					shadedFloor = { u'\u2591', BG_GREEN | FG_BLUE };
				else if (brightness < 0.7f)
					shadedFloor = { u'\u2592', BG_GREEN | FG_BLUE };
				else if (brightness < 0.9f)
					shadedFloor = { u'\u2593', BG_GREEN | FG_BLUE };
				else
					shadedFloor = { ' ', BG_BLUE };

				SetSymbol(x, y, shadedFloor);
			}
		}

		for (int y = 0; y < m_mapHeight; y++) 
		{
			for (int x = 0; x < m_mapWidth; x++)
			{
				if(m_map[x + y * m_mapWidth] == L'#')
					SetSymbol(x, y, { L' ',  COLOR::BG_CYAN });
				else
					SetSymbol(x, y, { L' ',  COLOR::BG_BLACK });
			}
		}

		SetSymbol(m_player->GetX(), m_player->GetY(), { u'\u25A0',  COLOR::BG_BLACK | COLOR::FG_DARK_MAGENTA });
	}
}