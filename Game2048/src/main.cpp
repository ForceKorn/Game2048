//#ToDo
// 1. Refactor code
// 1.1 Convert Board cache from int[] to std::array
// 1.2
// 2. Add fuzz tests
// 3. Add sanitizers
// 4. 
// 

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