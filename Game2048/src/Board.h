#ifndef BOARD_H
#define BOARD_H

#include <iosfwd>

inline constexpr int BOARD_SIZE = 4;

class Board 
{
public:
	Board();

	void reset();
	void display(std::ostream& display) const;

	bool canMove() const;
	bool isFull() const;
	bool move(char direction);
	bool containsValue(int value) const;

private:
	void addRandomTile();
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();

private:
	int tiles[BOARD_SIZE][BOARD_SIZE];
};

#endif // BOARD_H
