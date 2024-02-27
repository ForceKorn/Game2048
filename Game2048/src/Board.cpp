#include "Board.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>
#include <string>

namespace
{
	constexpr int GAME_MIN_TILE_VALUE = 2;
	constexpr int DISTRIBUTION_MINIMUM_VALUE = 1;
	constexpr int DISTRIBUTION_MAXIMUM_VALUE = 100;
	constexpr int DISTRIBUTION_SMALLEST_TILE_TRESHOLD = 90;

#ifdef _DEBUG
	std::mt19937 mt{};
#else
	std::random_device rd;
	std::mt19937 mt{ rd() };
#endif

	const std::uniform_int_distribution randomizer { DISTRIBUTION_MINIMUM_VALUE, DISTRIBUTION_MAXIMUM_VALUE };

	constexpr auto tileContainsValue = [](int tile) -> bool
	{
		return tile != 0;
	};

	constexpr auto getRandomTile = [] () -> int
	{
		return GAME_MIN_TILE_VALUE + (randomizer(mt) > DISTRIBUTION_SMALLEST_TILE_TRESHOLD) * GAME_MIN_TILE_VALUE;
	};
}

std::pair<bool, int> shiftToBegin(std::vector<int>& cache);
std::pair<bool, int> shiftToEnd(std::vector<int>& cache);

Board::Board(const int winValue, const int height, const int width) :
	m_winningValue(winValue),
	m_tiles { std::vector<std::vector<int>>(height, std::vector<int>(width, 0)) }
{
	reset();
}

Board::Board(const Board& gameBoard) : 
	m_winningValue(gameBoard.m_winningValue), 
	m_tiles(gameBoard.m_tiles),
	m_score(gameBoard.m_score)
{}

Board& Board::operator=(const Board& gameBoard)
{
	if (this == &gameBoard) { return *this; }

	assert(this->m_winningValue == gameBoard.m_winningValue && "Copy of boards with different winning values");
	m_tiles = gameBoard.m_tiles;
	m_score = gameBoard.m_score;
	return *this;
}

#ifdef _DEBUG
bool operator==(const Board& lhs, const Board& rhs)
{
	return lhs.m_tiles == rhs.m_tiles;
}
#endif

void Board::reset() 
{
	for (size_t i = 0; i < getBoardHeight(); ++i) 
	{
		for (size_t j = 0; j < getBoardWidth(); ++j) 
		{
			m_tiles[i][j] = 0;
		}
	}

	// Adding two initial tiles
	addRandomTile();
	addRandomTile();
}

void drawLine(const size_t width, std::ostream& display) 
{
	display << '\n';
	for (size_t k = 0; k < width; ++k) {
		display << "-";
	}
	display << '\n';
}

void Board::display(std::ostream& display) const
{	
	const size_t maxValueWidth = std::to_string(this->m_winningValue).size();
	const size_t horizontalLineLength = 2 * getBoardWidth() * getBoardWidth() + 1;

	for (size_t i = 0; i < getBoardHeight(); ++i) 
	{
		drawLine(horizontalLineLength, display);

		display << "|";
		for (size_t j = 0; j < getBoardWidth(); ++j) 
		{
			display << std::setw(maxValueWidth);
			
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
	}
	drawLine(horizontalLineLength, display);
}

bool Board::isFull() const 
{
	for (size_t i = 0; i < getBoardHeight(); ++i) 
	{
		for (size_t j = 0; j < getBoardWidth(); ++j)
		{
			if (!tileContainsValue(m_tiles[i][j]))
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
	const size_t k_heightEdgeTreshold = getBoardHeight() - 1;
	const size_t k_widthEdgeTreshold  = getBoardWidth() - 1;
	for (size_t i = 0; i < getBoardHeight(); ++i)
	{
		for (size_t j = 0; j < getBoardWidth(); ++j)
		{
			if ((i > 1)						&& (m_tiles[i][j] == m_tiles[i - 1][j])) { return true; }
			if ((j > 1)						&& (m_tiles[i][j] == m_tiles[i][j - 1])) { return true; }
			if ((i < k_heightEdgeTreshold)  && (m_tiles[i][j] == m_tiles[i + 1][j])) { return true; }
			if ((j < k_widthEdgeTreshold)   && (m_tiles[i][j] == m_tiles[i][j + 1])) { return true; }
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

size_t Board::getBoardWidth() const
{
	return m_tiles.front().size();
}

size_t Board::getBoardHeight() const
{
	return m_tiles.size();
}

std::vector<Board::Coordinate_t> Board::getEmptyTilesCoordinates() const
{
	std::vector<Coordinate_t> points;
	for (size_t i = 0; i < getBoardHeight(); ++i)
	{
		for (size_t j = 0; j < getBoardWidth(); ++j)
		{
			if (!tileContainsValue(m_tiles[i][j]))
			{
				points.emplace_back(Coordinate_t{ i, j });
			}
		}
	}
	return points;
};

void Board::addRandomTile() 
{
	const auto k_availableTiles = getEmptyTilesCoordinates();
	if (k_availableTiles.empty()) { return; }

	std::uniform_int_distribution<size_t> indexSelector{ size_t {0}, k_availableTiles.size() - 1 };
	const auto [rowIndex, columnIndex] = k_availableTiles[indexSelector(mt)];
	m_tiles[rowIndex][columnIndex] = getRandomTile();
}

bool Board::moveLeft() 
{
	bool isMoved = false;

	std::vector<int> cache(getBoardWidth(), 0);
	for (size_t i = 0; i < getBoardHeight(); ++i)
	{
		for (size_t j = 0; j < getBoardWidth(); ++j) { cache[j] = m_tiles[i][j]; }

		const auto [moved, scored] = shiftToBegin(cache);
		isMoved |= moved;
		m_score += scored;

		for (size_t j = 0; j < getBoardWidth(); ++j) { m_tiles[i][j] = cache[j]; }
	}
	return isMoved;
}

bool Board::moveRight() 
{
	bool isMoved = false;

	std::vector<int> cache(getBoardWidth(), 0);
	for (size_t i = 0; i < getBoardHeight(); ++i)
	{
		for (size_t j = 0; j < getBoardWidth(); ++j) { cache[j] = m_tiles[i][j]; }
		const auto [moved, scored] = shiftToEnd(cache);
		isMoved |= moved;
		m_score += scored;

		for (size_t j = 0; j < getBoardWidth(); ++j) { m_tiles[i][j] = cache[j]; }
	}
	return isMoved;
}

bool Board::moveUp() 
{
	bool isMoved = false;

	std::vector<int> cache(getBoardHeight(), 0);
	for (size_t j = 0; j < getBoardWidth(); ++j)
	{
		for (size_t i = 0; i < getBoardHeight(); ++i) { cache[i] = m_tiles[i][j]; }
		
		const auto [moved, scored] = shiftToBegin(cache);
		isMoved |= moved;
		m_score += scored;

		for (size_t i = 0; i < getBoardHeight(); ++i) { m_tiles[i][j] = cache[i]; }
	}
	return isMoved;
}

bool Board::moveDown()
{
	bool isMoved = false;

	std::vector<int> cache(getBoardHeight(), 0);
	for (size_t j = 0; j < getBoardWidth(); ++j)
	{
		for (size_t i = 0; i < getBoardHeight(); ++i) { cache[i] = m_tiles[i][j]; }

		const auto [moved, scored] = shiftToEnd(cache);
		isMoved |= moved;
		m_score += scored;

		for (size_t i = 0; i < getBoardHeight(); ++i) { m_tiles[i][j] = cache[i]; }
	}
	return isMoved;
}

bool Board::reachedVictoryValue() const 
{
	for (size_t i = 0; i < getBoardHeight(); i++) 
	{
		for (size_t j = 0; j < getBoardWidth(); j++) 
		{
			if (m_tiles[i][j] == m_winningValue) 
			{
				return true;
			}
		}
	}
	return false;
}

std::pair<bool, int> shiftToBegin(std::vector<int>& cache)
{
	const auto cahcedMatrixSlice = cache.data();
	const auto columnSize = cache.size();

	// Remove in between zeros
	std::vector<int> result(columnSize, 0);
	std::copy_if(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, result.begin(), tileContainsValue);

	// Accumulate from front
	int scorePerShift = 0;
	for (auto it = result.begin() + 1; it != result.end(); ++it)
	{
		const auto prev = it - 1;
		const auto next = it + 1;
		if ((*prev == *it) && tileContainsValue(*it))
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
		if (!tileContainsValue(*prev))
		{
			auto nextNonZero = std::find_if(prev, result.end(), tileContainsValue);
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

std::pair<bool, int> shiftToEnd(std::vector<int>& cache)
{
	const auto cahcedMatrixSlice = cache.data();
	const auto columnSize = cache.size();

	// removeEmptyCells(int* cahcedMatrixSlice, int columnSize) -> std::vector<int>
	std::vector<int> result(columnSize, 0);
	std::copy_if(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, result.begin(), tileContainsValue);

	// calcScore(begin, end) -> int
	int scorePerShift = 0;
	for (auto it = result.rbegin() + 1; it != result.rend(); ++it)
	{
		// mergeWithSame(auto currentIter, auto lastIter) -> int
		const auto prev = it - 1;
		const auto next = it + 1;
		if ((*prev == *it) && tileContainsValue(*it))
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
		if (!tileContainsValue(*prev))
		{
			auto nextNonZero = std::find_if(prev, result.rend(), tileContainsValue);
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

#ifdef _DEBUG
bool Board::fuzzyEqual(const std::span<std::tuple<int, bool>> data) {
	size_t optionalMismatches = 0;
	size_t mismatches = 0;

	size_t k = 0;
	for (size_t i = 0; i < getBoardHeight(); ++i) 
	{
		for (size_t j = 0; j < getBoardWidth(); ++j)
		{
			const auto [value, required] = data[k++];
			if (required)
			{
				mismatches += m_tiles[i][j] != value;
			}
			else
			{
				optionalMismatches += m_tiles[i][j] != value;
			}
		}
	}
	return mismatches == 0 && optionalMismatches == 1;
}
#endif

// Tests only 
#ifdef _DEBUG
void Board::setBoard(const std::span<int> data)
{
	int k = -1;
	for (size_t i = 0; i < getBoardHeight(); i++)
	{
		for (size_t j = 0; j < getBoardWidth(); j++)
		{
			m_tiles[i][j] = data[++k];
		}
	}
}
#endif