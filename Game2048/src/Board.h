#ifndef BOARD_H
#define BOARD_H

#include <iosfwd>
#include <vector>
#include <array>
#include <span>
#include <tuple>

class Board 
{
public:
	Board(const int winValue, const int height, const int width);
	Board(const Board& gameBoard);
	Board& operator=(const Board& gameBoard);

public:
	void reset();
	void display(std::ostream& display) const;
	bool canMove() const;
	bool isFull() const;
	bool move(char direction);
	bool reachedVictoryValue() const;
	int getScore() const;

#ifdef _DEBUG
public: // For Google Tests
	void setBoard(const std::span<int> data);
	bool fuzzyEqual(const std::span<std::tuple<int, bool>> data);
	friend bool operator==(const Board& lhs, const Board& rhs);
#endif

private:
	using Coordinate_t = std::array<size_t, 2>;

private:
	std::vector<Coordinate_t> getEmptyTilesCoordinates() const;
	size_t getBoardWidth() const;
	size_t getBoardHeight() const;
	void addRandomTile();
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();

private:
	const int m_winningValue;

private:
	std::vector<std::vector<int>> m_tiles;
	int m_score = 0;
};

#endif // BOARD_H
