#include <iostream>
#include "Game.h"


int main() 
{
	std::ostream& display = std::cout;
	Game game(display);
	game.run();

	system("pause");
	return 0;
}