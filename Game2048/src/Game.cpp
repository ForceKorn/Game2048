#include "Game.h"

#include <iostream>
#include <string>

Game::Game(std::ostream& display)
	: board(), score(0), m_displayDevice(display) {}

void Game::run() 
{
	reset();
	display();

	while (board.canMove())
	{
		const int direction = handleInput();
		board.move(direction);
		display();

		const bool isFull = board.isFull();
		const bool isVictory = board.containsValue(2048);
		if (isVictory)
		{
			handleWin();
			return;
		}

		if (isFull)
		{
			handleLose();
			return;
		}
	}
}

void Game::display()
{
	system("cls"); // clear console

	m_displayDevice << "Score: " << score << '\n';
	board.display(m_displayDevice);
}

char Game::handleInput()
{
	m_displayDevice << "Enter a move (w/a/s/d): ";
	
	std::string input;
	std::getline(std::cin, input);
	return input.empty() ? 0 : input.front();
}

void Game::reset() 
{
	board.reset();
	score = 0;
}

void Game::handleWin() 
{
	m_displayDevice << "Congratulations! You have reached 2048!\n";
}

void Game::handleLose() 
{
	m_displayDevice << "Game over. You lose.\n";
}