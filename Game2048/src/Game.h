#ifndef GAME_H
#define GAME_H

#include "Board.h"

class Game {
public:
	Game();
	void run();

private:
	void init();
	void print();
	void handleInput();
	bool isGameOver();
	bool isBoardFull();
	void reset();
	void handleWin();
	void handleLose();

	Board board;
	bool gameOver;
	int score;
};

#endif // GAME_H