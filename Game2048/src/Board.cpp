#include "Board.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>

bool updateColumnUp(int* column, int columnSize);
bool updateColumnDown(int* column, int columnSize);
constexpr auto isNonEmpty = [] (int number) { return number != 0; };

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
			tiles[i][j] = 0;
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
			display << std::setw(4) << tiles[i][j] << "\t";
		}
		display << '\n';
	}
}

bool Board::isFull() const 
{
	for (int i = 0; i < BOARD_SIZE; ++i) 
	{
		for (int j = 0; j < BOARD_SIZE; ++j) 
		{
			if (!tiles[i][j])
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
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 1; j < BOARD_SIZE; ++j)
		{
			if (tiles[i][j - 1] == tiles[i][j]) { return true; }
		}
	}
	
	for (int i = 1; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j)
		{
			if (tiles[i - 1][j] == tiles[i][j]) { return true; }
		}
	}
	
	/*
	// Check if any adjacent tiles have the same value
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (i > 0 && tiles[i][j].getValue() == tiles[i - 1][j].getValue()) {
				return true;
			}
			if (j > 0 && tiles[i][j].getValue() == tiles[i][j - 1].getValue()) {
				return true;
			}
			if (i < BOARD_SIZE - 1 && tiles[i][j].getValue() == tiles[i + 1][j].getValue()) {
				return true;
			}
			if (j < BOARD_SIZE - 1 && tiles[i][j].getValue() == tiles[i][j + 1].getValue()) {
				return true;
			}
		}
	}
	*/
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

void Board::addRandomTile() 
{
	if (isFull()) { return; }

	int rowIndex = -1, columnIndex = -1;
	do {
		rowIndex	= rand() % BOARD_SIZE;
		columnIndex = rand() % BOARD_SIZE;
	} while (tiles[rowIndex][columnIndex]);

	tiles[rowIndex][columnIndex] = 2 + (rand() & 1) * 2;
}

bool Board::moveLeft() 
{
	bool isMoved = false;

	int cache[BOARD_SIZE] = { 0 };
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j) { cache[j] = tiles[i][j]; }
		isMoved |= updateColumnUp(cache, BOARD_SIZE);
		for (int j = 0; j < BOARD_SIZE; ++j) { tiles[i][j] = cache[j]; }
	}
	return isMoved;
}

bool Board::moveRight() 
{
	bool isMoved = false;

	int cache[BOARD_SIZE] = { 0 };
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		for (int j = 0; j < BOARD_SIZE; ++j) { cache[j] = tiles[i][j]; }
		isMoved |= updateColumnDown(cache, BOARD_SIZE);
		for (int j = 0; j < BOARD_SIZE; ++j) { tiles[i][j] = cache[j]; }
	}
	return isMoved;
}

bool Board::moveUp() 
{
	bool isMoved = false;

	int cache[BOARD_SIZE] = { 0 };
	for (int j = 0; j < BOARD_SIZE; ++j)
	{
		for (int i = 0; i < BOARD_SIZE; ++i) { cache[i] = tiles[i][j]; }
		isMoved |= updateColumnUp(cache, BOARD_SIZE);
		for (int i = 0; i < BOARD_SIZE; ++i) { tiles[i][j] = cache[i]; }
	}
	return isMoved;
}

bool Board::moveDown() 
{
	bool isMoved = false;
	
	int cache[BOARD_SIZE] = { 0 };
	for (int j = 0; j < BOARD_SIZE; ++j)
	{
		for (int i = 0; i < BOARD_SIZE; ++i) { cache[i] = tiles[i][j]; }
		isMoved |= updateColumnDown(cache, BOARD_SIZE);
		for (int i = 0; i < BOARD_SIZE; ++i) { tiles[i][j] = cache[i]; }
	}
	return isMoved;
}

bool Board::containsValue(int value) const 
{
	for (int i = 0; i < BOARD_SIZE; i++) 
	{
		for (int j = 0; j < BOARD_SIZE; j++) 
		{
			if (tiles[i][j] == value) 
			{
				return true;
			}
		}
	}
	return false;
}

bool updateColumnUp(int* cahcedMatrixSlice, int columnSize) // rename, make common ?
{
	// Remove in between zeros
	std::vector<int> result(columnSize, 0);
	std::copy_if(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, result.begin(), isNonEmpty);

	// Accumulate from front
	for (auto it = result.begin() + 1; it != result.end(); ++it)
	{
		const auto prev = it - 1;
		const auto next = it + 1;
		if ((*prev == *it) && isNonEmpty(*it))
		{
			*prev *= 2; *it = 0;
			it += (next != result.end());
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
	return isMoved;
}

bool updateColumnDown(int* cahcedMatrixSlice, int columnSize) // rename, make common ?
{
	// Remove in between zeros
	std::vector<int> result(columnSize, 0);
	std::copy_if(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, result.begin(), isNonEmpty);
				
	// Accumulate from back
	for (auto it = result.rbegin() + 1; it != result.rend(); ++it)
	{
		const auto prev = it - 1;
		const auto next = it + 1;
		if ((*prev == *it) && isNonEmpty(*it))
		{
			*prev *= 2; *it = 0;
			it += (next != result.rend());
		}
	}

	// Shift to back
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

	// Save Calculate 
	const bool isMoved = !std::equal(result.begin(), result.end(), cahcedMatrixSlice);

	// Write back
	std::fill(cahcedMatrixSlice, cahcedMatrixSlice + columnSize, 0);
	std::copy(result.begin(), result.end(), cahcedMatrixSlice);
	return isMoved;
}
