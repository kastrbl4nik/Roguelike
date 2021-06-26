#include "Game.h"

Game::Game() : Engine(200, 120, 2) {
	m_player = new Player(8, 8, 0);

	m_mapWidth = 16;
	m_mapHeight = 16;

	m_map += L"################";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#######        #";
	m_map += L"#     #        #";
	m_map += L"#     #        #";
	m_map += L"#              #";
	m_map += L"#######        #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#              #";
	m_map += L"#  #   #   #   #";
	m_map += L"#              #";
	m_map += L"################";
}

void Game::OnGameStarted() {
}
void Game::Update() {

	// Look Around
	if(abs(m_mouse.horizontalAxis) > 1)
		m_player->IncrementAngle(m_mouse.horizontalAxis * m_deltaTime / 10.0f);

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

		if (distToWall <= Player::ViewDistance / 4.0f) shadedWall = { ' ', BG_WHITE };
		else if (distToWall < Player::ViewDistance / 3.0f) shadedWall = { u'\u2591', BG_WHITE | FG_BLUE };
		else if (distToWall < Player::ViewDistance / 2.0f) shadedWall = { u'\u2592', BG_WHITE | FG_BLUE };
		else if (distToWall < Player::ViewDistance) shadedWall = { u'\u2593', BG_WHITE | FG_BLUE };
		else shadedWall = { ' ', BG_BLUE };

		for (int y = 0; y < m_screenHeight; y++) {
			if (y < distToCeiling)
				SetSymbol(x, y, { ' ', COLOR::BG_BLUE });
			else if (y >= distToCeiling && y <= distToFloor)
				SetSymbol(x, y, shadedWall);
			else
				SetSymbol(x, y, { ' ', COLOR::BG_GREEN });
		}
	}
}