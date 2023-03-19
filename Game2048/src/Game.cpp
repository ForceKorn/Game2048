#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Game.h"

using namespace std;

Game::Game() : board(), gameOver(false), score(0) {}

void Game::run() {
	init();
	print();

	while (!gameOver) {
		handleInput();
		print();

		if (isGameOver() || isBoardFull() || board.hasTileWithValue(2048)) {
			gameOver = true;

			if (board.hasTileWithValue(2048)) {
				handleWin();
			}
			else {
				handleLose();
			}
		}
	}
}

void Game::init() {
	board.clear();
	board.addRandomTile();
	board.addRandomTile();
	score = 0;
	gameOver = false;
}

void Game::print() 
{
	system("cls"); // clear console

	cout << "Score: " << score << endl;
	cout << board << endl;
}

void Game::handleInput() {
	cout << "Enter a move (w/a/s/d): ";
	string input;
	cin >> input;

	if (input == "w") {
		score += board.moveUp();
	}
	else if (input == "a") {
		score += board.moveLeft();
	}
	else if (input == "s") {
		score += board.moveDown();
	}
	else if (input == "d") {
		score += board.moveRight();
	}
}

bool Game::isGameOver() {
	return !board.canMove();
}

bool Game::isBoardFull() {
	return board.isFull();
}

void Game::reset() {
	board.clear();
	board.addRandomTile();
	board.addRandomTile();
	score = 0;
	gameOver = false;
}

void Game::handleWin() {
	cout << "Congratulations! You have reached 2048!" << endl;
	gameOver = true;
}

void Game::handleLose() {
	cout << "Game over. You lose." << endl;
	gameOver = true;
}