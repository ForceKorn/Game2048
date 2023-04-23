#include "Board.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

namespace
{
	constexpr int DISTRIBUTION_MINIMUM_VALUE = 1;
	constexpr int DISTRIBUTION_MAXIMUM_VALUE = 100;
	constexpr int DISTRIBUTION_SMALLEST_TILE_TRESHOLD = 90;

	std::mt19937 mt {};
	std::uniform_int_distribution randomizer { DISTRIBUTION_MINIMUM_VALUE, DISTRIBUTION_MAXIMUM_VALUE };
}

std::pair<bool, int> shiftToBegin(int* column, int columnSize);
std::pair<bool, int> shiftToEnd(int* column, int columnSize);
constexpr auto isNonEmpty = [] (int number) { return number != 0; };
constexpr auto getRandomTile = [] () -> int
{
	return 2 + (randomizer(mt) > DISTRIBUTION_SMALLEST_TILE_TRESHOLD) * 2;
};

Board::Board() 
{
	reset();
}

void Board::reset() 
{
	for (int i = 0; i < BOARD_SIZE; ++i) 
	{
		for (int j = 0; j < BOARD_SIZE; ++j) 
		{
			m_tiles[i][j] = 0;
		}
	}

	// Add two initial tiles
	addRandomTile();
	addRandomTile();
}

void Board::display(std::ostream& display) const
{	
	for (int i = 0; i < BOARD_SIZE; ++i) 
	{
		for (int j = 0; j < BOARD_SIZE; ++j) 
		{
			display << std::setw(4);
			
			const int value = m_tiles[i][j];
			if (value)
			{
				display << value;
			}
			else
			{
				display << ' ';
			}
			display << "\t|";
		}
		display << '\n';
		for (int k = 0; k < 2 * BOARD_SIZE * BOARD_SIZE; ++k) { display << "-"; }
		display << '\n';
	}
}

bool Board::isFull() const 
{
	for (int i = 0; i < BOARD_SIZE; ++i) 
	{
		for (int j = 0; j < BOARD_SIZE; ++j) 
		{
			if (!m_tiles[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

bool Board::canMove() const 
{
	// Check if there are any empty tiles
	if (!isFull()) { return true; }

	// Check if still has move on full board
	const int k_edgeTreshold = BOARD_SIZE - 1;
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 1; j < BOARD_SIZE; ++j)
		{
			if ((i > 1)				 && (m_tiles[i][j] == m_tiles[i - 1][j])) { return true; }
			if ((j > 1)			     && (m_tiles[i][j] == m_tiles[i][j - 1])) { return true; }
			if ((i < k_edgeTreshold) && (m_tiles[i][j] == m_tiles[i + 1][j])) { return true; }
			if ((j < k_edgeTreshold) && (m_tiles[i][j] == m_tiles[i][j + 1])) { return true; }
		}
	}
	return false;
}

bool Board::move(char direction) 
{
	bool isContentMoved = false;
	switch (direction)
	{
	case 'a': // Move left
		isContentMoved = moveLeft();
		break;
	case 'd': // Move right
		isContentMoved = moveRight();
		break;
	case 'w': // Move up
		isContentMoved = moveUp();
		break;
	case 's': // Move down
		isContentMoved = moveDown();
		break;
	}

	if (isContentMoved) { addRandomTile(); }
	return isContentMoved;
}

int Board::getScore() const
{
	return m_score;
}

// random device
void Board::addRandomTile() 
{
	if (isFull()) { return; }

	int rowIndex = -1, columnIndex = -1;
	do {
		rowIndex	= rand() % BOARD_SIZE;
		columnIndex = rand() % BOARD_SIZE;
	} while (m_tiles[rowIndex][columnIndex]);

	m_tiles[rowIndex][columnIndex] = getRandomTile();
}

bool Board::moveLeft() 
{
	bool isMoved = false;

	int cache[BOARD_SIZE] = { 0 };
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j) { cache[j] = m_tiles[i][j]; }

		const auto [moved, scored] = shiftToBegin(cache, BOARD_SIZE);
		isMoved |= moved;
		m_score += scored;

		for (int j = 0; j < BOARD_SIZE; ++j) { m_tiles[i][j] = cache[j]; }
	}
	return isMoved;
}

bool Board::moveRight() 
{
	bool isMoved = false;

	int cache[BOARD_SIZE] = { 0 };
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j) { cache[j] = m_tiles[i][j]; }
		const auto [moved, scored] = shiftToEnd(cache, BOARD_SIZE);
		isMoved |= moved;
		m_score += scored;

		for (int j = 0; j < BOARD_SIZE; ++j) { m_tiles[i][j] = cache[j]; }
	}
	return isMoved;
}

bool Board::moveUp() 
{
	bool isMoved = false;

	int cache[BOARD_SIZE] = { 0 };
	for (int j = 0; j < BOARD_SIZE; ++j)
	{
		for (int i = 0; i < BOARD_SIZE; ++i) { cache[i] = m_tiles[i][j]; }
		
		const auto [moved, scored] = shiftToBegin(cache, BOARD_SIZE);
		isMoved |= moved;
		m_score += scored;

		for (int i = 0; i < BOARD_SIZE; ++i) { m_tiles[i][j] = cache[i]; }
	}
	return isMoved;
}

bool Board::moveDown() 
{
	bool isMoved = false;
	
	int cache[BOARD_SIZE] = { 0 };
	for (int j = 0; j < BOARD_SIZE; ++j)
	{
		for (int i = 0; i < BOARD_SIZE; ++i) { cache[i] = m_tiles[i][j]; }

		const auto [moved, scored] = shiftToEnd(cache, BOARD_SIZE);
		isMoved |= moved;
		m_score += scored;

		for (int i = 0; i < BOARD_SIZE; ++i) { m_tiles[i][j] = cache[i]; }
	}
	return isMoved;
}

bool Board::containsValue(int value) const 
{
	for (int i = 0; i < BOARD_SIZE; i++) 
	{
		for (int j = 0; j < BOARD_SIZE; j++) 
		{
			if (m_tiles[i][j] == value) 
			{
				return true;
			}
		}
	}
	return false;
}

std::pair<bool, int> shiftToBegin(int* cahcedMatrixSlice, int columnSize) // make common implementation
{
	// Remove in between zeros
	std::vector<int> result(columnSize, 0);
	std::copy_if(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, result.begin(), isNonEmpty);

	// Accumulate from front
	int scorePerShift = 0;
	for (auto it = result.begin() + 1; it != result.end(); ++it)
	{
		const auto prev = it - 1;
		const auto next = it + 1;
		if ((*prev == *it) && isNonEmpty(*it))
		{
			*prev *= 2; *it = 0;
			it += (next != result.end());
			scorePerShift += *prev;
		}
	}

	// Shift to back
	for (auto it = result.begin() + 1; it != result.end(); ++it)
	{
		const auto prev = it - 1;
		if (!isNonEmpty(*prev))
		{
			auto nextNonZero = std::find_if(prev, result.end(), isNonEmpty);
			if (nextNonZero == result.end()) { break; }

			std::iter_swap(prev, nextNonZero);
		}
	}

	// Save Calculate 
	const bool isMoved = !std::equal(result.begin(), result.end(), cahcedMatrixSlice);

	// Write back
	std::fill(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, 0);
	std::copy(result.begin(), result.end(), cahcedMatrixSlice);
	return { isMoved, scorePerShift };
}

std::pair<bool, int> shiftToEnd(int* cahcedMatrixSlice, int columnSize) // make common implementation
{
	// removeEmptyCells(int* cahcedMatrixSlice, int columnSize) -> std::vector<int>
	std::vector<int> result(columnSize, 0);
	std::copy_if(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, result.begin(), isNonEmpty);
				
	// calcScore(begin, end) -> int
	int scorePerShift = 0;
	for (auto it = result.rbegin() + 1; it != result.rend(); ++it)
	{
		// mergeWithSame(auto currentIter, auto lastIter) -> int
		const auto prev = it - 1;
		const auto next = it + 1;
		if ((*prev == *it) && isNonEmpty(*it))
		{
			*prev *= 2; *it = 0;
			it += (next != result.rend());
			scorePerShift += *prev;
		}
	}

	// shiftToSide(auto begin, auto end) -> void
	for (auto it = result.rbegin() + 1; it != result.rend(); ++it)
	{
		const auto prev = it - 1;
		if (!isNonEmpty(*prev))
		{
			auto nextNonZero = std::find_if(prev, result.rend(), isNonEmpty);
			if (nextNonZero == result.rend()) { break; }

			std::iter_swap(prev, nextNonZero);
		}
	}

	// 
	const bool isMoved = !std::equal(result.begin(), result.end(), cahcedMatrixSlice);

	// copyRow Write back
	std::fill(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, 0);
	std::copy(result.begin(), result.end(), cahcedMatrixSlice);
	return { isMoved, scorePerShift };
}

void Board::setBoard(const int* const data)
{
	int k = -1;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			m_tiles[i][j] = data[++k];
		}
	}
}