#pragma once
#ifndef GAME_H
#define GAME_H

#define ASSERT(x) if (!(x)) __debugbreak();

#include "Engine.h"
#include "Player.h"


class Game : public Engine {
private:
	std::unique_ptr<Player> m_player;
	std::wstring m_map;
	int m_mapWidth;
	int m_mapHeight;
public:
	Game(const char* map);
	~Game();

	void OnGameStarted() override;
	void Update() override;
};


#endif GAME_H // !GAME_H