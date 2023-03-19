#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <iosfwd>

class Game {
public:
	Game(std::ostream& os);
	void run();

private:
	char handleInput();
	void display();
	void reset();
	void handleWin();
	void handleLose();

private:
	Board board;
	int score = 0;
	std::ostream& m_displayDevice;
};

#endif // GAME_H