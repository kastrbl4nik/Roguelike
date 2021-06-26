#include "Game.h"

Game::Game() : Engine(200, 120, 4) {
	m_player = new Player(8, 8, 0);

	m_mapWidth = 16;
	m_mapHeight = 16;

	m_map += L"################";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"################";
}

void Game::OnGameStarted() {
}
void Game::Update() {

	if(abs(m_mouse.horizontalAxis) > 1)
		m_player->IncrementAngle(m_mouse.horizontalAxis * m_deltaTime / 10.0f);


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

		for (int y = 0; y < m_screenHeight; y++) {
			if (y < distToCeiling)
				SetSymbol(x, y, { ' ', COLOR::BG_BLUE });
			else if (y >= distToCeiling && y <= distToFloor)
				SetSymbol(x, y, { ' ', COLOR::BG_DARK_GREY });
			else
				SetSymbol(x, y, { ' ', COLOR::BG_GREY });
		}
	}
}