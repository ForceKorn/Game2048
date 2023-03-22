#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <iosfwd>

class Game {
public:
	Game(std::ostream& os, std::istream& is);
	void run();

private:
	char getUserMoveDirection();
	void display();
	void reset();
	void handleWin();
	void handleLose();

private:
	Board m_board;
	Board m_previousMoveBoards;
	
	std::ostream& m_displayDevice;
	std::istream& m_inputDevice;
};

#endif // GAME_H