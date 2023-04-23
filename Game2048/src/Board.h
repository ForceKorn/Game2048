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
	int getScore() const;

	void setBoard(const int* const data); // created for Google tests only

private:
	void addRandomTile();
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();

private:
	int m_tiles[BOARD_SIZE][BOARD_SIZE] = { 0 }; // make variable sizes
	int m_score = 0;
};

#endif // BOARD_H
