#pragma once
#ifndef GAME_H
#define GAME_H

#include "Engine.h"


class Game : public Engine {
private:
public:
	Game() : Engine(50, 50, 8) {

	}
	void OnGameStarted() override {
		CHAR_INFO symb = { u' ', COLOR::BG_BLUE };
		SetSymbol(25, 25, symb);
	}
	// Update is called once every frame
	void Update() override {
		
	}
};


#endif GAME_H