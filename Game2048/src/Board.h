#ifndef BOARD_H
#define BOARD_H

#include "Tile.h"

inline constexpr int BOARD_SIZE = 4;

class Board {
public:
	Board();

	void reset();
	void print() const;
	bool isFull() const;
	bool canMove() const;
	bool isGameOver() const;
	bool move(char direction);
	int getScore() const;
	bool hasTileWithValue(int value) const;

	void addRandomTile();
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();
	bool mergeTiles(Tile& first, Tile& second);
	void clear();

private:
	Tile tiles[BOARD_SIZE][BOARD_SIZE];
	int score;
};

std::ostream& operator<<(std::ostream& os, const Board& board);

#endif // BOARD_H