#pragma once
#ifndef GAME_H
#define GAME_H

#include "Engine.h"


class Game : public Engine {
private:
public:
	Game() : Engine(50, 30, 8) {

	}
	void OnGameStarted() override {
		CHAR_INFO white = { u' ', COLOR::BG_WHITE };
		CHAR_INFO red = { u' ', COLOR::BG_RED };
		FillRect(0, 0, 50, 10, white);
		FillRect(0, 10, 50, 10, red);
		FillRect(0, 20, 50, 10, white);
	}
	// Update is called once every frame
	void Update() override {
		
	}
};


#endif GAME_H