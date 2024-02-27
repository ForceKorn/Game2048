//#ToDo
// 1. Refactor code
// 2. Add fuzz tests

#include <iostream>
#include "Game.h"


int main() 
{
	std::ostream& display = std::cout;
	std::istream& keyboard = std::cin;
	
	Game game(display, keyboard);
	game.run();

	system("pause");
	return 0;
}