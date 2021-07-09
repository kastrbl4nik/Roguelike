#pragma once
#ifndef GAME_H
#define GAME_H

#include "Engine.h"
#include "Player.h"


class Game : public Engine {
private:
	Player* m_player;
	std::wstring m_map;
	int m_mapWidth;
	int m_mapHeight;
public:
	Game();
	~Game();

	void OnGameStarted() override;
	void Update() override;
};


#endif GAME_H // !GAME_H