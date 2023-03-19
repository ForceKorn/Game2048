#include "Board.h"
#include <iostream>
#include <cstdlib>

Board::Board() {
	reset();
}

void Board::reset() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			tiles[i][j].setValue(0);
		}
	}

	score = 0;

	// Add two initial tiles
	addRandomTile();
	addRandomTile();
}

void Board::print() const {
	std::cout << "Score: " << score << std::endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			std::cout << tiles[i][j].getValue() << "\t";
		}
		std::cout << std::endl;
	}
}

bool Board::isFull() const {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (tiles[i][j].isEmpty()) {
				return false;
			}
		}
	}

	return true;
}

bool Board::canMove() const {
	// Check if there are any empty tiles
	if (!isFull()) {
		return true;
	}

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

	return false;
}

bool Board::isGameOver() const {
	return !canMove();
}

bool Board::move(char direction) {
	bool moved = false;

	switch (direction) {
	case 'a': // Move left
		moved = moveLeft();
		break;
	case 'd': // Move right
		moved = moveRight();
		break;
	case 'w': // Move up
		moved = moveUp();
		break;
	case 's': // Move down
		moved = moveDown();
		break;
	}

	if (moved) {
		addRandomTile();
	}

	return moved;
}

int Board::getScore() const {
	return score;
}

void Board::addRandomTile() {
	if (isFull()) {
		return;
	}

	int value = (rand() % 100 < 90) ? 2 : 4;
	int row, col;
	do {
		row = rand() % BOARD_SIZE;
		col = rand() % BOARD_SIZE;
	} while (!tiles[row][col].isEmpty());

	tiles[row][col].setValue(value);
}

bool Board::moveLeft() {
	bool moved = false;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 1; j < BOARD_SIZE; j++) {
			if (tiles[i][j].isEmpty()) {
				continue;
			}

			Tile& currentTile = tiles[i][j];
			Tile* prevTile = &tiles[i][j - 1];

			while (prevTile >= &tiles[i][0] && prevTile->isEmpty()) {
				prevTile--;
			}

			if (prevTile >= &tiles[i][0] && mergeTiles(currentTile, *prevTile)) {
				moved = true;
			}
		}
	}

	return moved;
}

bool Board::moveRight() {
	bool moved = false;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = BOARD_SIZE - 2; j >= 0; j--) {
			if (tiles[i][j].isEmpty()) {
				continue;
			}

			Tile& currentTile = tiles[i][j];
			Tile* prevTile = &tiles[i][j + 1];

			while (prevTile <= &tiles[i][BOARD_SIZE - 1] && prevTile->isEmpty()) {
				prevTile++;
			}

			if (prevTile <= &tiles[i][BOARD_SIZE - 1] && mergeTiles(currentTile, *prevTile)) {
				moved = true;
			}
		}
	}

	return moved;
}

bool Board::moveUp() {
	bool moved = false;

	for (int i = 1; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (tiles[i][j].isEmpty()) {
				continue;
			}

			Tile& currentTile = tiles[i][j];
			Tile* prevTile = &tiles[i - 1][j];

			while (prevTile >= &tiles[0][j] && prevTile->isEmpty()) {
				prevTile--;
			}

			if (prevTile >= &tiles[0][j] && mergeTiles(currentTile, *prevTile)) {
				moved = true;
			}
		}
	}

	return moved;
}

bool Board::moveDown() {
	bool moved = false;

	for (int i = BOARD_SIZE - 2; i >= 0; i--) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (tiles[i][j].isEmpty()) {
				continue;
			}

			Tile& currentTile = tiles[i][j];
			Tile* prevTile = &tiles[i + 1][j];

			while (prevTile <= &tiles[BOARD_SIZE - 1][j] && prevTile->isEmpty()) {
				prevTile++;
			}

			if (prevTile <= &tiles[BOARD_SIZE - 1][j] && mergeTiles(currentTile, *prevTile)) {
				moved = true;
			}
		}
	}

	return moved;
}

bool Board::mergeTiles(Tile& first, Tile& second) {
	if (first.getValue() == 0) {
		return false;
	}

	if (first.getValue() == second.getValue()) {
		second.setValue(second.getValue() * 2);
		score += second.getValue();
		first.setValue(0);
		return true;
	}

	return false;
}

bool Board::hasTileWithValue(int value) const {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (tiles[i][j].getValue() == value) {
				return true;
			}
		}
	}
	return false;
}

void Board::clear() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			tiles[i][j].setValue(0);
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Board& board) {
	for (int i = 0; i < board.m_rows; i++) {
		for (int j = 0; j < board.m_cols; j++) {
			os << std::setw(4) << board.m_grid[i][j];
		}
		os << std::endl;
	}
	return os;
}