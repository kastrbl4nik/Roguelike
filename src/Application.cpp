#include "Game.h"

int main(int argc, char** argv) {

	const char* map;
	if (argc > 1)
		map = argv[1];
	else
		map = "res/maps/lvl1.txt"; // Default map

	Game game(map);
	game.Start();
	return EXIT_SUCCESS;

}